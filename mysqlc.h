#pragma once
#ifndef __MYSQLC__
#define __MYSQLC__

#include "mysql.h"
#include <iostream>
#include <functional>
#include <cstring>

#define _mysql_library_init mysql_library_init(0, NULL, NULL);
#define _mysql_library_end mysql_library_end();



class MysqlcException {
public:
	MysqlcException(unsigned int errorNo, char* error) ;
	~MysqlcException() ;
public:
	void what() const;
	unsigned int errorNo() const ;
private:
	char _error[256] ;
	unsigned int _errno ;
};


class Mysqlc {
public:
	Mysqlc();
	~Mysqlc();
private:
	MYSQL* _mysql;
public:
	bool connect(const char* host, const char* user, const char* password, const char* db, int port); // 连接数据库
	bool disConnect(); // 断开连接数据库
	bool setCharacterSet(const char* csname) ; // 设置字符集
	bool createDataBase(const char* dbname) ; // 新建库
	bool dropDataBase(const char* dbname) ; // 删除库
	bool use(const char* dbname); // 切换库
public:
	bool autoCommit(bool) ; // 自动提交
	bool begin() ; // 开启事务
	bool startTransaction() ; // 开启事务
	bool execute(const char* sql,std::function<void(long)> affected_rows) ; // 获取DML affected_rows 影响行数
	bool query(const char* sql, std::function<bool(MYSQL_ROW)> data_rows, std::function<void(void)> empty) ; // 获取DQL的数据
	bool commit() ; // 提交
	bool rollback() ; // 回滚
	const char* error() ; // 获取错误信息
	unsigned int errNo() ; // 获取错误代码
private:
	bool baseQuery(const char* sql, std::function<bool(MYSQL_RES*)> query_reslut) ; // 执行SQL
};

#endif // !__MYSQLC__
