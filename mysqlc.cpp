#include "mysqlc.h"



Mysqlc::Mysqlc() {
    	mysql_thread_init() ;
	_mysql = mysql_init(nullptr);
}


Mysqlc::~Mysqlc() {
	disConnect();
	mysql_thread_end();
}


bool Mysqlc::connect(std::string host, std::string user, std::string password, std::string db, int port) {
	if (!mysql_real_connect(_mysql, host.c_str(), user.c_str(), password.c_str(), db.c_str(), port, NULL, 0)) {
		return false;
	}
	return true;
}

bool Mysqlc::disConnect() {
	if (_mysql) {
		mysql_close(_mysql);
		_mysql = nullptr ;
	}
	if (nullptr == _mysql) {
		return true ;
	} else {
		return false ;
	}
}

bool Mysqlc::setCharacterSet(const char* csname) {
	if (mysql_set_character_set(_mysql, csname)) {
		return false ;
	}
	return true ;
}

bool Mysqlc::createDataBase(const char* dbname) {
	std::string sql = "create database " + std::string(dbname) ;
        return execute(sql,nullptr) ;
}

bool Mysqlc::dropDataBase(const char* dbname) {
	std::string sql = "drop database " + std::string(dbname) ;
	return execute(sql,nullptr) ;
}


bool Mysqlc::begin() {
	return execute("begin",nullptr) ;
}

bool Mysqlc::commit() {
	if (!mysql_commit(_mysql)) {
		return true ;
	} else {
		return false ;
	}
}

bool Mysqlc::rollback() {
	if (!mysql_rollback(_mysql)) {
		return true ;
	} else {
		return false ;
	}
}

const char* Mysqlc::error() {
	return mysql_error(_mysql) ;
}


bool Mysqlc::use(const char* dbname) {
	if (mysql_select_db(_mysql, dbname)) {
		return false ;
	}
	return true ;
}

bool Mysqlc::autoCommit(bool mode) {
		if (!mysql_autocommit(_mysql,mode ? '1' : '0')) {
			return true ;
		}
		return false ;
}

bool Mysqlc::execute(std::string sql,std::function<void(long)> affected_rows) {
	return baseQuery(sql,[&](MYSQL_RES* res) {
		if(nullptr == res) {
			if (0 == mysql_field_count(_mysql)) {
				if (affected_rows) {
					affected_rows(mysql_affected_rows(_mysql)) ;
				}
				return true ;
			} else {
				return false ;
			}
		} else {
			return false ;
		}
	});
}

bool Mysqlc::baseQuery(std::string sql, std::function<bool(MYSQL_RES*)> query_reslut) {
	if (!mysql_query(_mysql, sql.c_str())) {
		MYSQL_RES* res = mysql_store_result(_mysql) ;
		bool retVal = query_reslut(res) ;
		if (res) mysql_free_result(res);
		return retVal ;
	}
	else {
		return false;
	}
}


bool Mysqlc::query(std::string sql, std::function<bool(MYSQL_ROW)> data_rows, std::function<void(void)> empty) {
	return baseQuery(sql, [&](MYSQL_RES* res){
		if (res) {
			bool retVal = true;
			if (mysql_num_rows(res)) {
				MYSQL_ROW row ;
				while ((row = mysql_fetch_row(res)) != NULL && retVal) {
					retVal = data_rows(row);
				}
			}else {
				if (empty) {
					empty();
				}
			}
			return true ;
		} else {
			return false ;
		}
	}) ;
}
