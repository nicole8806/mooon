/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef DbProxyService_H
#define DbProxyService_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "db_proxy_types.h"

namespace mooon { namespace db_proxy {

#ifdef _WIN32
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class DbProxyServiceIf {
 public:
  virtual ~DbProxyServiceIf() {}
  virtual void query(DBTable& _return, const std::string& sign, const int32_t seq, const int32_t query_index, const std::vector<std::string> & tokens, const int32_t limit, const int32_t limit_start) = 0;
  virtual int64_t update(const std::string& sign, const int32_t seq, const int32_t update_index, const std::vector<std::string> & tokens) = 0;
  virtual void async_update(const std::string& sign, const int32_t seq, const int32_t update_index, const std::vector<std::string> & tokens) = 0;
  virtual int64_t update2(const int32_t seq, const int32_t database_index, const std::string& tablename, const std::map<std::string, std::string> & tokens, const std::vector<Condition> & conditions) = 0;
  virtual int64_t insert2(const int32_t seq, const int32_t database_index, const std::string& tablename, const std::map<std::string, std::string> & tokens) = 0;
  virtual void query2(DBTable& _return, const int32_t seq, const int32_t database_index, const std::string& tablename, const std::vector<std::string> & fields, const std::vector<Condition> & conditions, const std::string& groupby, const std::string& orderby, const int32_t limit, const int32_t limit_start) = 0;
};

class DbProxyServiceIfFactory {
 public:
  typedef DbProxyServiceIf Handler;

  virtual ~DbProxyServiceIfFactory() {}

  virtual DbProxyServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(DbProxyServiceIf* /* handler */) = 0;
};

class DbProxyServiceIfSingletonFactory : virtual public DbProxyServiceIfFactory {
 public:
  DbProxyServiceIfSingletonFactory(const boost::shared_ptr<DbProxyServiceIf>& iface) : iface_(iface) {}
  virtual ~DbProxyServiceIfSingletonFactory() {}

  virtual DbProxyServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(DbProxyServiceIf* /* handler */) {}

 protected:
  boost::shared_ptr<DbProxyServiceIf> iface_;
};

class DbProxyServiceNull : virtual public DbProxyServiceIf {
 public:
  virtual ~DbProxyServiceNull() {}
  void query(DBTable& /* _return */, const std::string& /* sign */, const int32_t /* seq */, const int32_t /* query_index */, const std::vector<std::string> & /* tokens */, const int32_t /* limit */, const int32_t /* limit_start */) {
    return;
  }
  int64_t update(const std::string& /* sign */, const int32_t /* seq */, const int32_t /* update_index */, const std::vector<std::string> & /* tokens */) {
    int64_t _return = 0;
    return _return;
  }
  void async_update(const std::string& /* sign */, const int32_t /* seq */, const int32_t /* update_index */, const std::vector<std::string> & /* tokens */) {
    return;
  }
  int64_t update2(const int32_t /* seq */, const int32_t /* database_index */, const std::string& /* tablename */, const std::map<std::string, std::string> & /* tokens */, const std::vector<Condition> & /* conditions */) {
    int64_t _return = 0;
    return _return;
  }
  int64_t insert2(const int32_t /* seq */, const int32_t /* database_index */, const std::string& /* tablename */, const std::map<std::string, std::string> & /* tokens */) {
    int64_t _return = 0;
    return _return;
  }
  void query2(DBTable& /* _return */, const int32_t /* seq */, const int32_t /* database_index */, const std::string& /* tablename */, const std::vector<std::string> & /* fields */, const std::vector<Condition> & /* conditions */, const std::string& /* groupby */, const std::string& /* orderby */, const int32_t /* limit */, const int32_t /* limit_start */) {
    return;
  }
};

typedef struct _DbProxyService_query_args__isset {
  _DbProxyService_query_args__isset() : sign(false), seq(false), query_index(false), tokens(false), limit(false), limit_start(false) {}
  bool sign :1;
  bool seq :1;
  bool query_index :1;
  bool tokens :1;
  bool limit :1;
  bool limit_start :1;
} _DbProxyService_query_args__isset;

class DbProxyService_query_args {
 public:

  DbProxyService_query_args(const DbProxyService_query_args&);
  DbProxyService_query_args& operator=(const DbProxyService_query_args&);
  DbProxyService_query_args() : sign(), seq(0), query_index(0), limit(0), limit_start(0) {
  }

  virtual ~DbProxyService_query_args() throw();
  std::string sign;
  int32_t seq;
  int32_t query_index;
  std::vector<std::string>  tokens;
  int32_t limit;
  int32_t limit_start;

  _DbProxyService_query_args__isset __isset;

  void __set_sign(const std::string& val);

  void __set_seq(const int32_t val);

  void __set_query_index(const int32_t val);

  void __set_tokens(const std::vector<std::string> & val);

  void __set_limit(const int32_t val);

  void __set_limit_start(const int32_t val);

  bool operator == (const DbProxyService_query_args & rhs) const
  {
    if (!(sign == rhs.sign))
      return false;
    if (!(seq == rhs.seq))
      return false;
    if (!(query_index == rhs.query_index))
      return false;
    if (!(tokens == rhs.tokens))
      return false;
    if (!(limit == rhs.limit))
      return false;
    if (!(limit_start == rhs.limit_start))
      return false;
    return true;
  }
  bool operator != (const DbProxyService_query_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DbProxyService_query_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DbProxyService_query_pargs {
 public:


  virtual ~DbProxyService_query_pargs() throw();
  const std::string* sign;
  const int32_t* seq;
  const int32_t* query_index;
  const std::vector<std::string> * tokens;
  const int32_t* limit;
  const int32_t* limit_start;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DbProxyService_query_result__isset {
  _DbProxyService_query_result__isset() : success(false) {}
  bool success :1;
} _DbProxyService_query_result__isset;

class DbProxyService_query_result {
 public:

  DbProxyService_query_result(const DbProxyService_query_result&);
  DbProxyService_query_result& operator=(const DbProxyService_query_result&);
  DbProxyService_query_result() {
  }

  virtual ~DbProxyService_query_result() throw();
  DBTable success;

  _DbProxyService_query_result__isset __isset;

  void __set_success(const DBTable& val);

  bool operator == (const DbProxyService_query_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const DbProxyService_query_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DbProxyService_query_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DbProxyService_query_presult__isset {
  _DbProxyService_query_presult__isset() : success(false) {}
  bool success :1;
} _DbProxyService_query_presult__isset;

class DbProxyService_query_presult {
 public:


  virtual ~DbProxyService_query_presult() throw();
  DBTable* success;

  _DbProxyService_query_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _DbProxyService_update_args__isset {
  _DbProxyService_update_args__isset() : sign(false), seq(false), update_index(false), tokens(false) {}
  bool sign :1;
  bool seq :1;
  bool update_index :1;
  bool tokens :1;
} _DbProxyService_update_args__isset;

class DbProxyService_update_args {
 public:

  DbProxyService_update_args(const DbProxyService_update_args&);
  DbProxyService_update_args& operator=(const DbProxyService_update_args&);
  DbProxyService_update_args() : sign(), seq(0), update_index(0) {
  }

  virtual ~DbProxyService_update_args() throw();
  std::string sign;
  int32_t seq;
  int32_t update_index;
  std::vector<std::string>  tokens;

  _DbProxyService_update_args__isset __isset;

  void __set_sign(const std::string& val);

  void __set_seq(const int32_t val);

  void __set_update_index(const int32_t val);

  void __set_tokens(const std::vector<std::string> & val);

  bool operator == (const DbProxyService_update_args & rhs) const
  {
    if (!(sign == rhs.sign))
      return false;
    if (!(seq == rhs.seq))
      return false;
    if (!(update_index == rhs.update_index))
      return false;
    if (!(tokens == rhs.tokens))
      return false;
    return true;
  }
  bool operator != (const DbProxyService_update_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DbProxyService_update_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DbProxyService_update_pargs {
 public:


  virtual ~DbProxyService_update_pargs() throw();
  const std::string* sign;
  const int32_t* seq;
  const int32_t* update_index;
  const std::vector<std::string> * tokens;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DbProxyService_update_result__isset {
  _DbProxyService_update_result__isset() : success(false) {}
  bool success :1;
} _DbProxyService_update_result__isset;

class DbProxyService_update_result {
 public:

  DbProxyService_update_result(const DbProxyService_update_result&);
  DbProxyService_update_result& operator=(const DbProxyService_update_result&);
  DbProxyService_update_result() : success(0) {
  }

  virtual ~DbProxyService_update_result() throw();
  int64_t success;

  _DbProxyService_update_result__isset __isset;

  void __set_success(const int64_t val);

  bool operator == (const DbProxyService_update_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const DbProxyService_update_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DbProxyService_update_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DbProxyService_update_presult__isset {
  _DbProxyService_update_presult__isset() : success(false) {}
  bool success :1;
} _DbProxyService_update_presult__isset;

class DbProxyService_update_presult {
 public:


  virtual ~DbProxyService_update_presult() throw();
  int64_t* success;

  _DbProxyService_update_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _DbProxyService_async_update_args__isset {
  _DbProxyService_async_update_args__isset() : sign(false), seq(false), update_index(false), tokens(false) {}
  bool sign :1;
  bool seq :1;
  bool update_index :1;
  bool tokens :1;
} _DbProxyService_async_update_args__isset;

class DbProxyService_async_update_args {
 public:

  DbProxyService_async_update_args(const DbProxyService_async_update_args&);
  DbProxyService_async_update_args& operator=(const DbProxyService_async_update_args&);
  DbProxyService_async_update_args() : sign(), seq(0), update_index(0) {
  }

  virtual ~DbProxyService_async_update_args() throw();
  std::string sign;
  int32_t seq;
  int32_t update_index;
  std::vector<std::string>  tokens;

  _DbProxyService_async_update_args__isset __isset;

  void __set_sign(const std::string& val);

  void __set_seq(const int32_t val);

  void __set_update_index(const int32_t val);

  void __set_tokens(const std::vector<std::string> & val);

  bool operator == (const DbProxyService_async_update_args & rhs) const
  {
    if (!(sign == rhs.sign))
      return false;
    if (!(seq == rhs.seq))
      return false;
    if (!(update_index == rhs.update_index))
      return false;
    if (!(tokens == rhs.tokens))
      return false;
    return true;
  }
  bool operator != (const DbProxyService_async_update_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DbProxyService_async_update_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DbProxyService_async_update_pargs {
 public:


  virtual ~DbProxyService_async_update_pargs() throw();
  const std::string* sign;
  const int32_t* seq;
  const int32_t* update_index;
  const std::vector<std::string> * tokens;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DbProxyService_update2_args__isset {
  _DbProxyService_update2_args__isset() : seq(false), database_index(false), tablename(false), tokens(false), conditions(false) {}
  bool seq :1;
  bool database_index :1;
  bool tablename :1;
  bool tokens :1;
  bool conditions :1;
} _DbProxyService_update2_args__isset;

class DbProxyService_update2_args {
 public:

  DbProxyService_update2_args(const DbProxyService_update2_args&);
  DbProxyService_update2_args& operator=(const DbProxyService_update2_args&);
  DbProxyService_update2_args() : seq(0), database_index(0), tablename() {
  }

  virtual ~DbProxyService_update2_args() throw();
  int32_t seq;
  int32_t database_index;
  std::string tablename;
  std::map<std::string, std::string>  tokens;
  std::vector<Condition>  conditions;

  _DbProxyService_update2_args__isset __isset;

  void __set_seq(const int32_t val);

  void __set_database_index(const int32_t val);

  void __set_tablename(const std::string& val);

  void __set_tokens(const std::map<std::string, std::string> & val);

  void __set_conditions(const std::vector<Condition> & val);

  bool operator == (const DbProxyService_update2_args & rhs) const
  {
    if (!(seq == rhs.seq))
      return false;
    if (!(database_index == rhs.database_index))
      return false;
    if (!(tablename == rhs.tablename))
      return false;
    if (!(tokens == rhs.tokens))
      return false;
    if (!(conditions == rhs.conditions))
      return false;
    return true;
  }
  bool operator != (const DbProxyService_update2_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DbProxyService_update2_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DbProxyService_update2_pargs {
 public:


  virtual ~DbProxyService_update2_pargs() throw();
  const int32_t* seq;
  const int32_t* database_index;
  const std::string* tablename;
  const std::map<std::string, std::string> * tokens;
  const std::vector<Condition> * conditions;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DbProxyService_update2_result__isset {
  _DbProxyService_update2_result__isset() : success(false) {}
  bool success :1;
} _DbProxyService_update2_result__isset;

class DbProxyService_update2_result {
 public:

  DbProxyService_update2_result(const DbProxyService_update2_result&);
  DbProxyService_update2_result& operator=(const DbProxyService_update2_result&);
  DbProxyService_update2_result() : success(0) {
  }

  virtual ~DbProxyService_update2_result() throw();
  int64_t success;

  _DbProxyService_update2_result__isset __isset;

  void __set_success(const int64_t val);

  bool operator == (const DbProxyService_update2_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const DbProxyService_update2_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DbProxyService_update2_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DbProxyService_update2_presult__isset {
  _DbProxyService_update2_presult__isset() : success(false) {}
  bool success :1;
} _DbProxyService_update2_presult__isset;

class DbProxyService_update2_presult {
 public:


  virtual ~DbProxyService_update2_presult() throw();
  int64_t* success;

  _DbProxyService_update2_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _DbProxyService_insert2_args__isset {
  _DbProxyService_insert2_args__isset() : seq(false), database_index(false), tablename(false), tokens(false) {}
  bool seq :1;
  bool database_index :1;
  bool tablename :1;
  bool tokens :1;
} _DbProxyService_insert2_args__isset;

class DbProxyService_insert2_args {
 public:

  DbProxyService_insert2_args(const DbProxyService_insert2_args&);
  DbProxyService_insert2_args& operator=(const DbProxyService_insert2_args&);
  DbProxyService_insert2_args() : seq(0), database_index(0), tablename() {
  }

  virtual ~DbProxyService_insert2_args() throw();
  int32_t seq;
  int32_t database_index;
  std::string tablename;
  std::map<std::string, std::string>  tokens;

  _DbProxyService_insert2_args__isset __isset;

  void __set_seq(const int32_t val);

  void __set_database_index(const int32_t val);

  void __set_tablename(const std::string& val);

  void __set_tokens(const std::map<std::string, std::string> & val);

  bool operator == (const DbProxyService_insert2_args & rhs) const
  {
    if (!(seq == rhs.seq))
      return false;
    if (!(database_index == rhs.database_index))
      return false;
    if (!(tablename == rhs.tablename))
      return false;
    if (!(tokens == rhs.tokens))
      return false;
    return true;
  }
  bool operator != (const DbProxyService_insert2_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DbProxyService_insert2_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DbProxyService_insert2_pargs {
 public:


  virtual ~DbProxyService_insert2_pargs() throw();
  const int32_t* seq;
  const int32_t* database_index;
  const std::string* tablename;
  const std::map<std::string, std::string> * tokens;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DbProxyService_insert2_result__isset {
  _DbProxyService_insert2_result__isset() : success(false) {}
  bool success :1;
} _DbProxyService_insert2_result__isset;

class DbProxyService_insert2_result {
 public:

  DbProxyService_insert2_result(const DbProxyService_insert2_result&);
  DbProxyService_insert2_result& operator=(const DbProxyService_insert2_result&);
  DbProxyService_insert2_result() : success(0) {
  }

  virtual ~DbProxyService_insert2_result() throw();
  int64_t success;

  _DbProxyService_insert2_result__isset __isset;

  void __set_success(const int64_t val);

  bool operator == (const DbProxyService_insert2_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const DbProxyService_insert2_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DbProxyService_insert2_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DbProxyService_insert2_presult__isset {
  _DbProxyService_insert2_presult__isset() : success(false) {}
  bool success :1;
} _DbProxyService_insert2_presult__isset;

class DbProxyService_insert2_presult {
 public:


  virtual ~DbProxyService_insert2_presult() throw();
  int64_t* success;

  _DbProxyService_insert2_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _DbProxyService_query2_args__isset {
  _DbProxyService_query2_args__isset() : seq(false), database_index(false), tablename(false), fields(false), conditions(false), groupby(false), orderby(false), limit(false), limit_start(false) {}
  bool seq :1;
  bool database_index :1;
  bool tablename :1;
  bool fields :1;
  bool conditions :1;
  bool groupby :1;
  bool orderby :1;
  bool limit :1;
  bool limit_start :1;
} _DbProxyService_query2_args__isset;

class DbProxyService_query2_args {
 public:

  DbProxyService_query2_args(const DbProxyService_query2_args&);
  DbProxyService_query2_args& operator=(const DbProxyService_query2_args&);
  DbProxyService_query2_args() : seq(0), database_index(0), tablename(), groupby(), orderby(), limit(0), limit_start(0) {
  }

  virtual ~DbProxyService_query2_args() throw();
  int32_t seq;
  int32_t database_index;
  std::string tablename;
  std::vector<std::string>  fields;
  std::vector<Condition>  conditions;
  std::string groupby;
  std::string orderby;
  int32_t limit;
  int32_t limit_start;

  _DbProxyService_query2_args__isset __isset;

  void __set_seq(const int32_t val);

  void __set_database_index(const int32_t val);

  void __set_tablename(const std::string& val);

  void __set_fields(const std::vector<std::string> & val);

  void __set_conditions(const std::vector<Condition> & val);

  void __set_groupby(const std::string& val);

  void __set_orderby(const std::string& val);

  void __set_limit(const int32_t val);

  void __set_limit_start(const int32_t val);

  bool operator == (const DbProxyService_query2_args & rhs) const
  {
    if (!(seq == rhs.seq))
      return false;
    if (!(database_index == rhs.database_index))
      return false;
    if (!(tablename == rhs.tablename))
      return false;
    if (!(fields == rhs.fields))
      return false;
    if (!(conditions == rhs.conditions))
      return false;
    if (!(groupby == rhs.groupby))
      return false;
    if (!(orderby == rhs.orderby))
      return false;
    if (!(limit == rhs.limit))
      return false;
    if (!(limit_start == rhs.limit_start))
      return false;
    return true;
  }
  bool operator != (const DbProxyService_query2_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DbProxyService_query2_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DbProxyService_query2_pargs {
 public:


  virtual ~DbProxyService_query2_pargs() throw();
  const int32_t* seq;
  const int32_t* database_index;
  const std::string* tablename;
  const std::vector<std::string> * fields;
  const std::vector<Condition> * conditions;
  const std::string* groupby;
  const std::string* orderby;
  const int32_t* limit;
  const int32_t* limit_start;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DbProxyService_query2_result__isset {
  _DbProxyService_query2_result__isset() : success(false) {}
  bool success :1;
} _DbProxyService_query2_result__isset;

class DbProxyService_query2_result {
 public:

  DbProxyService_query2_result(const DbProxyService_query2_result&);
  DbProxyService_query2_result& operator=(const DbProxyService_query2_result&);
  DbProxyService_query2_result() {
  }

  virtual ~DbProxyService_query2_result() throw();
  DBTable success;

  _DbProxyService_query2_result__isset __isset;

  void __set_success(const DBTable& val);

  bool operator == (const DbProxyService_query2_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const DbProxyService_query2_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DbProxyService_query2_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DbProxyService_query2_presult__isset {
  _DbProxyService_query2_presult__isset() : success(false) {}
  bool success :1;
} _DbProxyService_query2_presult__isset;

class DbProxyService_query2_presult {
 public:


  virtual ~DbProxyService_query2_presult() throw();
  DBTable* success;

  _DbProxyService_query2_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class DbProxyServiceClient : virtual public DbProxyServiceIf {
 public:
  DbProxyServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  DbProxyServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void query(DBTable& _return, const std::string& sign, const int32_t seq, const int32_t query_index, const std::vector<std::string> & tokens, const int32_t limit, const int32_t limit_start);
  void send_query(const std::string& sign, const int32_t seq, const int32_t query_index, const std::vector<std::string> & tokens, const int32_t limit, const int32_t limit_start);
  void recv_query(DBTable& _return);
  int64_t update(const std::string& sign, const int32_t seq, const int32_t update_index, const std::vector<std::string> & tokens);
  void send_update(const std::string& sign, const int32_t seq, const int32_t update_index, const std::vector<std::string> & tokens);
  int64_t recv_update();
  void async_update(const std::string& sign, const int32_t seq, const int32_t update_index, const std::vector<std::string> & tokens);
  void send_async_update(const std::string& sign, const int32_t seq, const int32_t update_index, const std::vector<std::string> & tokens);
  int64_t update2(const int32_t seq, const int32_t database_index, const std::string& tablename, const std::map<std::string, std::string> & tokens, const std::vector<Condition> & conditions);
  void send_update2(const int32_t seq, const int32_t database_index, const std::string& tablename, const std::map<std::string, std::string> & tokens, const std::vector<Condition> & conditions);
  int64_t recv_update2();
  int64_t insert2(const int32_t seq, const int32_t database_index, const std::string& tablename, const std::map<std::string, std::string> & tokens);
  void send_insert2(const int32_t seq, const int32_t database_index, const std::string& tablename, const std::map<std::string, std::string> & tokens);
  int64_t recv_insert2();
  void query2(DBTable& _return, const int32_t seq, const int32_t database_index, const std::string& tablename, const std::vector<std::string> & fields, const std::vector<Condition> & conditions, const std::string& groupby, const std::string& orderby, const int32_t limit, const int32_t limit_start);
  void send_query2(const int32_t seq, const int32_t database_index, const std::string& tablename, const std::vector<std::string> & fields, const std::vector<Condition> & conditions, const std::string& groupby, const std::string& orderby, const int32_t limit, const int32_t limit_start);
  void recv_query2(DBTable& _return);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class DbProxyServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<DbProxyServiceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (DbProxyServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_query(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_update(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_async_update(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_update2(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_insert2(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_query2(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  DbProxyServiceProcessor(boost::shared_ptr<DbProxyServiceIf> iface) :
    iface_(iface) {
    processMap_["query"] = &DbProxyServiceProcessor::process_query;
    processMap_["update"] = &DbProxyServiceProcessor::process_update;
    processMap_["async_update"] = &DbProxyServiceProcessor::process_async_update;
    processMap_["update2"] = &DbProxyServiceProcessor::process_update2;
    processMap_["insert2"] = &DbProxyServiceProcessor::process_insert2;
    processMap_["query2"] = &DbProxyServiceProcessor::process_query2;
  }

  virtual ~DbProxyServiceProcessor() {}
};

class DbProxyServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  DbProxyServiceProcessorFactory(const ::boost::shared_ptr< DbProxyServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< DbProxyServiceIfFactory > handlerFactory_;
};

class DbProxyServiceMultiface : virtual public DbProxyServiceIf {
 public:
  DbProxyServiceMultiface(std::vector<boost::shared_ptr<DbProxyServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~DbProxyServiceMultiface() {}
 protected:
  std::vector<boost::shared_ptr<DbProxyServiceIf> > ifaces_;
  DbProxyServiceMultiface() {}
  void add(boost::shared_ptr<DbProxyServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void query(DBTable& _return, const std::string& sign, const int32_t seq, const int32_t query_index, const std::vector<std::string> & tokens, const int32_t limit, const int32_t limit_start) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->query(_return, sign, seq, query_index, tokens, limit, limit_start);
    }
    ifaces_[i]->query(_return, sign, seq, query_index, tokens, limit, limit_start);
    return;
  }

  int64_t update(const std::string& sign, const int32_t seq, const int32_t update_index, const std::vector<std::string> & tokens) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->update(sign, seq, update_index, tokens);
    }
    return ifaces_[i]->update(sign, seq, update_index, tokens);
  }

  void async_update(const std::string& sign, const int32_t seq, const int32_t update_index, const std::vector<std::string> & tokens) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->async_update(sign, seq, update_index, tokens);
    }
    ifaces_[i]->async_update(sign, seq, update_index, tokens);
  }

  int64_t update2(const int32_t seq, const int32_t database_index, const std::string& tablename, const std::map<std::string, std::string> & tokens, const std::vector<Condition> & conditions) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->update2(seq, database_index, tablename, tokens, conditions);
    }
    return ifaces_[i]->update2(seq, database_index, tablename, tokens, conditions);
  }

  int64_t insert2(const int32_t seq, const int32_t database_index, const std::string& tablename, const std::map<std::string, std::string> & tokens) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->insert2(seq, database_index, tablename, tokens);
    }
    return ifaces_[i]->insert2(seq, database_index, tablename, tokens);
  }

  void query2(DBTable& _return, const int32_t seq, const int32_t database_index, const std::string& tablename, const std::vector<std::string> & fields, const std::vector<Condition> & conditions, const std::string& groupby, const std::string& orderby, const int32_t limit, const int32_t limit_start) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->query2(_return, seq, database_index, tablename, fields, conditions, groupby, orderby, limit, limit_start);
    }
    ifaces_[i]->query2(_return, seq, database_index, tablename, fields, conditions, groupby, orderby, limit, limit_start);
    return;
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class DbProxyServiceConcurrentClient : virtual public DbProxyServiceIf {
 public:
  DbProxyServiceConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  DbProxyServiceConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void query(DBTable& _return, const std::string& sign, const int32_t seq, const int32_t query_index, const std::vector<std::string> & tokens, const int32_t limit, const int32_t limit_start);
  int32_t send_query(const std::string& sign, const int32_t seq, const int32_t query_index, const std::vector<std::string> & tokens, const int32_t limit, const int32_t limit_start);
  void recv_query(DBTable& _return, const int32_t seqid);
  int64_t update(const std::string& sign, const int32_t seq, const int32_t update_index, const std::vector<std::string> & tokens);
  int32_t send_update(const std::string& sign, const int32_t seq, const int32_t update_index, const std::vector<std::string> & tokens);
  int64_t recv_update(const int32_t seqid);
  void async_update(const std::string& sign, const int32_t seq, const int32_t update_index, const std::vector<std::string> & tokens);
  void send_async_update(const std::string& sign, const int32_t seq, const int32_t update_index, const std::vector<std::string> & tokens);
  int64_t update2(const int32_t seq, const int32_t database_index, const std::string& tablename, const std::map<std::string, std::string> & tokens, const std::vector<Condition> & conditions);
  int32_t send_update2(const int32_t seq, const int32_t database_index, const std::string& tablename, const std::map<std::string, std::string> & tokens, const std::vector<Condition> & conditions);
  int64_t recv_update2(const int32_t seqid);
  int64_t insert2(const int32_t seq, const int32_t database_index, const std::string& tablename, const std::map<std::string, std::string> & tokens);
  int32_t send_insert2(const int32_t seq, const int32_t database_index, const std::string& tablename, const std::map<std::string, std::string> & tokens);
  int64_t recv_insert2(const int32_t seqid);
  void query2(DBTable& _return, const int32_t seq, const int32_t database_index, const std::string& tablename, const std::vector<std::string> & fields, const std::vector<Condition> & conditions, const std::string& groupby, const std::string& orderby, const int32_t limit, const int32_t limit_start);
  int32_t send_query2(const int32_t seq, const int32_t database_index, const std::string& tablename, const std::vector<std::string> & fields, const std::vector<Condition> & conditions, const std::string& groupby, const std::string& orderby, const int32_t limit, const int32_t limit_start);
  void recv_query2(DBTable& _return, const int32_t seqid);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  ::apache::thrift::async::TConcurrentClientSyncInfo sync_;
};

#ifdef _WIN32
  #pragma warning( pop )
#endif

}} // namespace

#endif
