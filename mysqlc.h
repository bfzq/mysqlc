#pragma once
#ifndef __MYSQLC__
#define __MYSQLC__

#include "mysql.h"
#include <string>
#include <iostream>
#include <functional>

#define _mysql_library_init mysql_library_init(0, NULL, NULL);
#define _mysql_library_end mysql_library_end();

class Mysqlc {
public:
	Mysqlc();
	~Mysqlc();
private:
	MYSQL* _mysql;
public:
	bool connect(std::string host, std::string user, std::string password, std::string db, int port); // 连接数据库
	bool disConnect(); // 断开连接数据库
	bool setCharacterSet(const char* csname) ; // 设置字符集
	bool createDataBase(const char* dbname) ; // 新建库
	bool dropDataBase(const char* dbname) ; // 删除库
	bool use(const char* dbname); // 切换库
public:
	bool autoCommit(bool) ; // 自动提交
	bool begin() ; // 开启事务
	bool startTransaction() ; // 开启事务
	bool execute(std::string sql,std::function<void(long)> affected_rows) ; // 获取DML affected_rows 影响行数
	bool query(std::string sql, std::function<bool(MYSQL_ROW)> data_rows, std::function<void(void)> empty) ; // 获取DQL的数据
	bool commit() ; // 提交
	bool rollback() ; // 回滚
	const char* error() ; // 获取错误信息
private:
	bool baseQuery(std::string sql, std::function<bool(MYSQL_RES*)> query_reslut) ; // 执行SQL
public:
	MYSQL getMYSQL() ;
};

#endif // !__MYSQLC__
