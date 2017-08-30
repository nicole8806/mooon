// Writed by yijian (eyjian@qq.com, eyjian@gmail.com)
#include "db_proxy_handler.h"
#include "config_loader.h"
#include "sql_logger.h"
#include <mooon/observer/observer_manager.h>
#include <mooon/sys/datetime_utils.h>
#include <mooon/sys/log.h>
#include <mooon/utils/args_parser.h>
#include <mooon/utils/format_string.h>
#include <mooon/utils/string_utils.h>
#include <thrift/TApplicationException.h>
#include <vector>

INTEGER_ARG_DECLARE(int32_t, cache_number);
namespace mooon { namespace db_proxy {

CDbProxyHandler::CDbProxyHandler()
{
    atomic_set(&_cached_number, 0);

    reset();
    mooon::observer::IObserverManager* observer_mananger = mooon::observer::get();
    if (observer_mananger != NULL)
        observer_mananger->register_observee(this);
}

CDbProxyHandler::~CDbProxyHandler()
{
    mooon::observer::IObserverManager* observer_mananger = mooon::observer::get();
    if (observer_mananger != NULL)
        observer_mananger->deregister_objservee(this);
}

void CDbProxyHandler::cleanup_cache()
{
    time_t now = time(NULL);
    sys::WriteLockHelper write_lock(_cache_table_lock);
    for (CacheTable::iterator iter=_cache_table.begin(); iter!=_cache_table.end();)
    {
        const struct CachedData& cached_data_ref = iter->second;

        if (now < cached_data_ref.cached_seconds+cached_data_ref.timestamp)
        {
            ++iter;
        }
        else
        {
            _cache_table.erase(iter++);
            atomic_dec(&_cached_number);
        }
    }
}

void CDbProxyHandler::query(DBTable& _return, const std::string& sign, const int32_t seq, const int32_t query_index, const std::vector<std::string> & tokens, const int32_t limit, const int32_t limit_start)
{
    CConfigLoader* config_loader = CConfigLoader::get_singleton();
    struct QueryInfo query_info;
    std::vector<std::string> escaped_tokens;

    if (!config_loader->get_query_info(query_index, &query_info))
    {
        MYLOG_ERROR("query_index[%d] not exists\n", query_index);
        throw apache::thrift::TApplicationException(utils::CStringUtils::format_string("query_index(%d) not exists", query_index));
    }
    if (sign != query_info.sign)
    {
        MYLOG_ERROR("sign[%s] error: %s\n", sign.c_str(), query_info.sign.c_str());
        throw apache::thrift::TApplicationException("sign error");
    }

    try
    {
        sys::CMySQLConnection* db_connection = config_loader->get_db_connection(query_info.database_index);
        if (NULL == db_connection)
        {
            MYLOG_ERROR("database_index[%d] not exists or cannot connect\n", query_info.database_index);
            throw apache::thrift::TApplicationException(utils::CStringUtils::format_string("database_index(%d) not exists or cannot connect", query_info.database_index));
        }
        else if (tokens.size() > utils::FORMAT_STRING_SIZE)
        {
            MYLOG_ERROR("[%d]too big: %d\n", seq, (int)tokens.size());
            throw apache::thrift::TApplicationException("tokens too many");
        }
        else
        {
            std::vector<std::string> escaped_tokens;
            escape_tokens(db_connection, tokens, &escaped_tokens);
            std::string sql = utils::format_string(query_info.sql_template.c_str(), escaped_tokens);

            if (sql.empty())
            {
                MYLOG_ERROR("error number of tokens or template: %s\n", query_info.str().c_str());
                throw apache::thrift::TApplicationException(utils::CStringUtils::format_string("error number of tokens or invalid template(%s)", query_info.str().c_str()));
            }
            else
            {
                MYLOG_DEBUG("%s LIMIT %d,%d\n", sql.c_str(), limit_start, limit);

                int32_t limit_ = limit;
                if (limit_ > MAX_LIMIT)
                    limit_ = MAX_LIMIT;
                if (limit_start < 0) // limit_start是从0开始而不是1
                    sql = utils::CStringUtils::format_string("%s LIMIT %d", sql.c_str(), limit_);
                else
                    sql = utils::CStringUtils::format_string("%s LIMIT %d,%d", sql.c_str(), limit_start, limit_);

                if (query_info.cached_seconds < 1)
                {
                    MYLOG_DEBUG("not cache: %s", sql.c_str());
                }
                else
                {
                    if (get_data_from_cache(_return, sql))
                    {
                        return; // 如果缓存中有，则直接返回
                    }
                }

                db_connection->query(_return, "%s", sql.c_str());
                if (_return.empty())
                {
                    MYLOG_DEBUG("number of rows: %zd, number of columns: %d\n", _return.size(), 0);
                }
                else
                {
                    MYLOG_DEBUG("number of rows: %zd, number of columns: %zd\n", _return.size(), _return[0].size());
                }

                ++_num_query_success;
                if ((query_info.cached_seconds > 0) && !_return.empty())
                {
                    add_data_to_cache(_return, sql, query_info.cached_seconds);
                }
            }
        }
    }
    catch (sys::CDBException& db_ex)
    {
        --_num_query_failure;
        MYLOG_ERROR("[%d]%s\n", seq, db_ex.str().c_str());
        throw apache::thrift::TApplicationException(db_ex.str());
    }
}

int64_t CDbProxyHandler::update(const std::string& sign, const int32_t seq, const int32_t update_index, const std::vector<std::string>& tokens)
{
    const int64_t num = do_update(true, sign, seq, update_index, tokens);
    if (num >= 0)
        ++_num_update_success;
    else
        ++_num_update_failure;
    return num;
}

void CDbProxyHandler::async_update(const std::string& sign, const int32_t seq, const int32_t update_index, const std::vector<std::string>& tokens)
{
    // 异步版本，忽略返回值，
    // 降低了可靠性，提升了性能。
    const int64_t num = do_update(false, sign, seq, update_index, tokens);
    if (num >= 0)
        ++_num_async_update_success;
    else
        ++_num_async_update_failure;
}

// UPDATE tablename SET tokens[0].first="tokens[0].second",tokens[1].first="tokens[1].second" WEHRE (conditions[0].left op conditions[0].right)
int64_t CDbProxyHandler::update2(const int32_t seq, const int32_t database_index, const std::string& tablename, const std::map<std::string, std::string> & tokens, const std::vector<Condition> & conditions)
{
    try
    {
        std::string sql;
        struct DbInfo db_info;
        CConfigLoader* config_loader = CConfigLoader::get_singleton();
        if (!config_loader->get_db_info(database_index, &db_info))
        {
            ++_num_update2_failure;
            MYLOG_ERROR("[%d]database_index[%d] not exists\n", seq, database_index);
            throw apache::thrift::TApplicationException(utils::CStringUtils::format_string("database_index(%d) not exists", database_index));
        }

        // DB连接
        sys::DBConnection* db_connection = config_loader->get_db_connection(database_index);
        if (NULL == db_connection)
        {
            ++_num_update2_failure;
            MYLOG_ERROR("[%d]database_index[%d] not exists or cannot connect\n", seq, database_index);
            throw apache::thrift::TApplicationException(utils::CStringUtils::format_string("database_index(%d) not exists or cannot connect", database_index));
        }

        // UPDATE tablename SET tokens[0].first="tokens[0].second",tokens[1].first="tokens[1].second"
        for (std::map<std::string, std::string>::const_iterator iter=tokens.begin(); iter!=tokens.end(); ++iter)
        {
            if (iter == tokens.begin())
                sql = std::string("UPDATE ") + db_connection->escape_string(tablename) + std::string(" SET ") + db_connection->escape_string(iter->first) + std::string("=\"") + db_connection->escape_string(iter->second) + std::string("\"");
            else
                sql += std::string(",") + db_connection->escape_string(iter->first) + std::string("=\"") + db_connection->escape_string(iter->second) + std::string("\"");
        }

        // WEHRE (conditions[0].left op conditions[0].right)
        if (!conditions.empty())
        {
            for (std::vector<Condition>::size_type i=0; i<conditions.size(); ++i)
            {
                const Condition& condition = conditions[i];

                if (0 == i)
                    sql += std::string(" WHERE (") + db_connection->escape_string(condition.left) + std::string(" ") + db_connection->escape_string(condition.op) + std::string(" ");
                else
                    sql += std::string(" AND (") + db_connection->escape_string(condition.left) + std::string(" ") + db_connection->escape_string(condition.op) + std::string(" ");

                if (condition.is_string)
                {
                    sql += std::string("\"") + db_connection->escape_string(condition.right) + std::string("\"");
                }
                else
                {
                    if (utils::CStringUtils::is_numeric_string(condition.right.c_str()))
                    {
                        sql += condition.right;
                    }
                    else
                    {
                        ++_num_update2_failure;
                        MYLOG_ERROR("invalid condition.right[%d][%s]\n", seq, condition.right.c_str());
                        throw apache::thrift::TApplicationException("invalid condition.right");
                    }
                }

                sql += std::string(")");
            }
        }

        const int64_t num_rows = write_sql("U2", seq, db_info, db_connection, sql);
        if (num_rows >= 0)
            ++_num_update2_success;
        else
            ++_num_update2_failure;
        return num_rows;
    }
    catch (sys::CDBException& ex)
    {
        ++_num_update2_failure;
        MYLOG_ERROR("[%d]%s\n", seq, ex.str().c_str());
        throw apache::thrift::TApplicationException(ex.str());
    }
}

// INSERT INTO tablename (tokens[0].first,tokens[1].first) VALUES (tokens[0].second,tokens[1].second)
int64_t CDbProxyHandler::insert2(const int32_t seq, const int32_t database_index, const std::string& tablename, const std::map<std::string, std::string> & tokens)
{
    try
    {
        std::string sql;
        struct DbInfo db_info;
        CConfigLoader* config_loader = CConfigLoader::get_singleton();
        if (!config_loader->get_db_info(database_index, &db_info))
        {
            ++_num_insert2_failure;
            MYLOG_ERROR("[%d]database_index[%d] not exists\n", seq, database_index);
            throw apache::thrift::TApplicationException(utils::CStringUtils::format_string("database_index(%d) not exists", database_index));
        }

        // DB连接
        sys::DBConnection* db_connection = config_loader->get_db_connection(database_index);
        if (NULL == db_connection)
        {
            ++_num_insert2_failure;
            MYLOG_ERROR("[%d]database_index[%d] not exists or cannot connect\n", seq, database_index);
            throw apache::thrift::TApplicationException(utils::CStringUtils::format_string("database_index(%d) not exists or cannot connect", database_index));
        }

        // INSERT INTO tablename (tokens[0].first,tokens[1].first
        for (std::map<std::string, std::string>::const_iterator iter=tokens.begin(); iter!=tokens.end(); ++iter)
        {
            if (iter == tokens.begin())
                sql = std::string("INSERT INTO ") + db_connection->escape_string(tablename) + std::string(" (") + db_connection->escape_string(iter->first);
            else
                sql += std::string(",") + db_connection->escape_string(iter->first);
        }

        // ) VALUES (tokens[0].second,tokens[1].second
        for (std::map<std::string, std::string>::const_iterator iter=tokens.begin(); iter!=tokens.end(); ++iter)
        {
            if (iter == tokens.begin())
                sql += std::string(") VALUES (\"") + db_connection->escape_string(iter->second) + std::string("\"");
            else
                sql += std::string(",\"") + db_connection->escape_string(iter->second) + std::string("\"");;
        }

        // )
        sql += std::string(")");

        uint64_t insert_id = 0;
        const int64_t num_rows = write_sql("I2", seq, db_info, db_connection, sql);
        if (num_rows >= 0)
        {
            ++_num_insert2_success;

            // 取得insert_id
            if (db_info.alias.empty())
            {
                insert_id = db_connection->get_insert_id();
            }
        }
        else
        {
            ++_num_insert2_failure;
        }

        return static_cast<int64_t>(insert_id);
    }
    catch (sys::CDBException& ex)
    {
        ++_num_insert2_failure;
        MYLOG_ERROR("[%d]%s\n", seq, ex.str().c_str());
        throw apache::thrift::TApplicationException(ex.str());
    }
}

// SELECT fields[0],fields[1] FROM tablename WHERE (conditions[0].left conditions[0].op conditions[0].right) GROUP BY groupby ORDER BY orderby LIMIT limit_start,limit
void CDbProxyHandler::query2(DBTable& _return, const int32_t seq, const int32_t database_index, const std::string& tablename, const std::vector<std::string> & fields, const std::vector<Condition> & conditions, const std::string& groupby, const std::string& orderby, const int32_t limit, const int32_t limit_start)
{
    try
    {
        std::string sql;
        struct DbInfo db_info;
        CConfigLoader* config_loader = CConfigLoader::get_singleton();
        if (!config_loader->get_db_info(database_index, &db_info))
        {
            MYLOG_ERROR("[%d]database_index[%d] not exists\n", seq, database_index);
            throw apache::thrift::TApplicationException(utils::CStringUtils::format_string("database_index(%d) not exists", database_index));
        }

        // DB连接
        sys::DBConnection* db_connection = config_loader->get_db_connection(database_index);
        if (NULL == db_connection)
        {
            MYLOG_ERROR("[%d]database_index[%d] not exists or cannot connect\n", seq, database_index);
            throw apache::thrift::TApplicationException(utils::CStringUtils::format_string("database_index(%d) not exists or cannot connect", database_index));
        }

        // SELECT fields[0],fields[1]
        for (std::vector<std::string>::size_type i=0; i<fields.size(); ++i)
        {
            const std::string& field = fields[i];

            if (0 == i)
                sql = std::string("SELECT ") + db_connection->escape_string(field);
            else
                sql += std::string(",") + db_connection->escape_string(field);
        }

        // FROM tablename
        sql += std::string(" FROM ") + db_connection->escape_string(tablename);

        // WHERE (conditions[0].left conditions[0].op conditions[0].right)
        if (!conditions.empty())
        {
            for (std::vector<Condition>::size_type i=0; i<conditions.size(); ++i)
            {
                const Condition& condition = conditions[i];

                if (0 == i)
                    sql += std::string(" WHERE (");
                else
                    sql += std::string(" AND (");

                sql += db_connection->escape_string(condition.left) + std::string(" ") + db_connection->escape_string(condition.op) + std::string(" ");
                if (condition.is_string)
                {
                    sql += std::string("\"") + db_connection->escape_string(condition.right) + std::string("\"");
                }
                else
                {
                    sql += db_connection->escape_string(condition.right);
                }

                sql += std::string(")");
            }
        }

        // GROUP BY groupby
        if (!groupby.empty())
            sql += std::string(" GROUP BY ") + db_connection->escape_string(groupby);
        // ORDER BY orderby
        if (!orderby.empty())
            sql += std::string(" ORDER BY ") + db_connection->escape_string(orderby);

        // LIMIT
        int32_t limit_ = limit;
        if (limit_ > MAX_LIMIT)
            limit_ = MAX_LIMIT;
        if (limit_start < 0)
            sql += utils::CStringUtils::format_string(" LIMIT %d", limit_);
        else
            sql += utils::CStringUtils::format_string(" LIMIT %d,%d", limit_start, limit_);

        MYLOG_DEBUG("%s", sql.c_str());
        db_connection->query(_return, "%s", sql.c_str());
        ++_num_query2_success;
    }
    catch (sys::CDBException& ex)
    {
        --_num_query2_success;
        MYLOG_ERROR("[SEQ:%d][DB:%d]%s\n", seq, database_index, ex.str().c_str());
        throw apache::thrift::TApplicationException(ex.str());
    }
}

void CDbProxyHandler::escape_tokens(void* db_connection, const std::vector<std::string>& tokens, std::vector<std::string>* escaped_tokens)
{
    sys::DBConnection* db_connection_ = (sys::DBConnection*)db_connection;

    escaped_tokens->clear();
    escaped_tokens->resize(tokens.size());
    for (std::vector<std::string>::size_type i=0; i<tokens.size(); ++i)
    {
        std::string escape_token;
        const std::string& token = tokens[i];
        if (NULL == db_connection_)
            sys::CMySQLConnection::escape_string(token, &escape_token);
        else
            escape_token = db_connection_->escape_string(token);
        (*escaped_tokens)[i] = escape_token;
    }
}

// 对于异步调用，不抛异常，因为异步调用不会等待该函数执行，也就是收不到抛出的异常
int64_t CDbProxyHandler::do_update(bool throw_exception, const std::string& sign, const int32_t seq, const int32_t update_index, const std::vector<std::string>& tokens)
{
    CConfigLoader* config_loader = CConfigLoader::get_singleton();
    struct UpdateInfo update_info;

    if (!config_loader->get_update_info(update_index, &update_info))
    {
        MYLOG_ERROR("[%d]update_index[%d] not exists\n", seq, update_index);
        if (throw_exception)
            throw apache::thrift::TApplicationException(utils::CStringUtils::format_string("update_index(%d) not exists", update_index));
    }
    else
    {
        struct DbInfo db_info;
        if (!config_loader->get_db_info(update_info.database_index, &db_info))
        {
            MYLOG_ERROR("[%d]database_index[%d] not exists\n", seq, update_info.database_index);
            if (throw_exception)
                throw apache::thrift::TApplicationException(utils::CStringUtils::format_string("database_index(%d) not exists", update_info.database_index));
        }
        else
        {
            if (tokens.size() > utils::FORMAT_STRING_SIZE)
            {
                MYLOG_ERROR("[%s][%d] tokens too much: %d\n", db_info.str().c_str(), seq, (int)tokens.size());
                if (throw_exception)
                    throw apache::thrift::TApplicationException("tokens too many");
            }
            else
            {
                std::vector<std::string> escaped_tokens;
                sys::DBConnection* db_connection = config_loader->get_db_connection(update_info.database_index);

                // 转义，以防止SQL注入
                escape_tokens(db_connection, tokens, &escaped_tokens);

                // 根据模板生成SQL
                const std::string& sql = utils::format_string(update_info.sql_template.c_str(), escaped_tokens);
                if (sql.empty())
                {
                    MYLOG_ERROR("tokens number(%zd) or template error: update_index=%d\n", escaped_tokens.size(), update_index);
                    if (throw_exception)
                        throw apache::thrift::TApplicationException(utils::CStringUtils::format_string("tokens number(%zd) or template error: update_index=%d", escaped_tokens.size(), update_index));
                }
                else
                {
                    return write_sql("U1", seq, db_info, db_connection, sql);
                }
            }
        }
    }

    return -1;
}

bool CDbProxyHandler::get_data_from_cache(DBTable& dbtable, const std::string& sql)
{
    utils::CMd5Helper md5_helper;
    md5_helper.update(sql);

    sys::ReadLockHelper read_lock(_cache_table_lock);
    CacheTable::iterator iter = _cache_table.find(md5_helper.value());
    if (iter == _cache_table.end())
    {
        MYLOG_DEBUG("[%s][%s] not in cache\n", md5_helper.to_string().c_str(), sql.c_str());
        return false;
    }
    else
    {
        MYLOG_DEBUG("get [%s][%s] from cache\n", md5_helper.to_string().c_str(), sql.c_str());
        dbtable = iter->second.cached_data;
        iter->second.timestamp = time(NULL);
        return true;
    }
}

void CDbProxyHandler::add_data_to_cache(const DBTable& dbtable, const std::string& sql, int cached_seconds)
{
    int32_t cached_number = atomic_read(&_cached_number);

    if (cached_number > mooon::argument::cache_number->value())
    {
        MYLOG_WARN("too many, can not to cache again: %s\n", sql.c_str());
    }
    else
    {
        utils::CMd5Helper md5_helper;
        md5_helper.update(sql);

        struct CachedData cached_data;
        cached_data.timestamp = time(NULL);
        cached_data.cached_seconds = cached_seconds;
        cached_data.cached_data = dbtable;

        sys::WriteLockHelper write_lock(_cache_table_lock);
        std::pair<CacheTable::iterator, bool> ret = _cache_table.insert(std::make_pair(md5_helper.value(), cached_data));
        if (ret.second)
        {
            atomic_inc(&_cached_number);
            MYLOG_DEBUG("[%s][%s] added into cache\n", md5_helper.to_string().c_str(), sql.c_str());
        }
        else
        {
            ret.first->second.timestamp = time(NULL);
            ret.first->second.cached_seconds = cached_seconds;
            ret.first->second.cached_data = dbtable;
            MYLOG_DEBUG("[%s][%s] updated into cache\n", md5_helper.to_string().c_str(), sql.c_str());
        }
    }
}

int64_t CDbProxyHandler::write_sql(const char* tag, int32_t seq, const struct DbInfo& db_info, sys::DBConnection* db_connection, const std::string& sql)
{
    if (db_info.alias.empty())
    {
        // 直接入库
        const int max_retries = 5;

        for (int retries=0; retries<max_retries; ++retries)
        {
            try
            {
                const uint64_t affected_rows = db_connection->update("%s", sql.c_str());
                MYLOG_INFO("[%s][WRITE_SQL][SEQ:%d][%" PRIu64"] %s", tag, seq, affected_rows, sql.c_str());
                ++_num_update_success_sql;
                return static_cast<int64_t>(affected_rows);
            }
            catch (sys::CDBException& db_ex)
            {
                ++_num_update_failure_sql;

                if (!db_connection->is_disconnected_exception(db_ex) || (retries==max_retries-1))
                {
                    ++_num_error_update_sql;
                    MYLOG_ERROR("[%s][ERROR_SQL][SEQ:%d][%s]%s\n", tag, seq, db_ex.sql(), db_ex.str().c_str());
                    break;
                }
                else
                {
                    MYLOG_ERROR("[%s][RETRY_SQL_%d][SEQ:%d][%s]%s\n", tag, retries, seq, sql.c_str(), db_ex.str().c_str());
                    mooon::sys::CUtils::millisleep(100); // 网络类原因稍后重试
                }
            }
        }

        return -1;
    }
    else
    {
        MYLOG_DEBUG("[%s][SEQ:%d] %s", tag, seq, sql.c_str());

        CConfigLoader* config_loader = CConfigLoader::get_singleton();
        CSqlLogger* sql_logger = config_loader->get_sql_logger(db_info.index);

        if (NULL == sql_logger)
        {
            MYLOG_ERROR("[%s][%s] no sql logger: %s\n", tag, db_info.str().c_str(), sql.c_str());
            throw apache::thrift::TApplicationException("no sql logger");
            return -1;
        }
        else
        {
            // 写入文件由dbprocess写入db
            bool written = sql_logger->write_log(sql);
            config_loader->release_sql_logger(sql_logger);
            if (written)
            {
                ++_num_write_success;
                return 0;
            }
            else
            {
                ++_num_write_failure;
                MYLOG_ERROR("[%s][%s] write sql log error: %s\n", tag, db_info.str().c_str(), sql.c_str());
                throw apache::thrift::TApplicationException("io error");
                return -1;
            }
        }
    }
}

void CDbProxyHandler::on_report(mooon::observer::IDataReporter* data_reporter, const std::string& current_datetime)
{
    if ((_num_query_success != 0) || (_num_query_failure != 0) ||
        (_num_query2_success != 0) || (_num_query2_failure != 0) ||
        (_num_update_success != 0) || (_num_update_failure != 0) ||
        (_num_update2_success != 0) || (_num_update2_failure != 0) ||
        (_num_update_success_sql != 0) || (_num_update_failure_sql != 0) ||
        (_num_async_update_success != 0) || (_num_async_update_failure != 0) ||
        (_num_insert2_success != 0) || (_num_insert2_failure != 0) ||
        (_num_write_success != 0) || (_num_write_failure != 0) ||
        (_num_error_update_sql != 0))
    {
        data_reporter->report("[%s][B]%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", current_datetime.c_str(),
            _num_query_success, _num_query_failure,
            _num_query2_success, _num_query2_failure,
            _num_update_success, _num_update_failure,
            _num_update2_success, _num_update2_failure,
            _num_update_success_sql, _num_update_failure_sql,
            _num_async_update_success, _num_async_update_failure,
            _num_insert2_success, _num_insert2_failure,
            _num_write_success, _num_write_failure,
            _num_error_update_sql);
        reset();
    }
}

void CDbProxyHandler::reset()
{
    _num_query_success = 0;
    _num_query_failure = 0;
    _num_query2_success = 0;
    _num_query2_failure = 0;

    _num_update_success = 0;
    _num_update_failure = 0;
    _num_update2_success = 0;
    _num_update2_failure = 0;
    _num_update_success_sql = 0;
    _num_update_failure_sql = 0;
    _num_async_update_success = 0;
    _num_async_update_failure = 0;

    _num_insert2_success = 0;
    _num_insert2_failure = 0;

    _num_write_success = 0;
    _num_write_failure = 0;

    _num_error_update_sql = 0;
}

} // namespace mooon
} // namespace db_proxy
