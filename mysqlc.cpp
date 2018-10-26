#include "mysqlc.h"

MysqlcException::MysqlcException(unsigned int errorNo, char* error) {
	_errno = errorNo ;
	strcpy(_error, error) ;	
}

MysqlcException::~MysqlcException() {}

void MysqlcException::what() const {
	printf("Error : %u %s\n",_errno,_error) ;
}

unsigned int MysqlcException::errorNo() const {
	return _errno ;
}


Mysqlc::Mysqlc() {
    	mysql_thread_init() ;
	_mysql = mysql_init(nullptr);
}


Mysqlc::~Mysqlc() {
	disConnect();
	mysql_thread_end();
}


bool Mysqlc::connect(const char* host, const char* user, const char* password, const char* db, int port) {
	if (!_mysql) {
		_mysql = mysql_init(nullptr);
	}
	if (!mysql_real_connect(_mysql, host, user, password, db, port, NULL, 0)) {
		throw MysqlcException(errNo(), (char*)error()) ;
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
		throw MysqlcException(errNo(), (char*)error()) ;
	}
	return true ;
}



bool Mysqlc::createDataBase(const char* dbname) {
	std::string sql = "create database if not exists ";
	sql = sql + std::string(dbname) ;
        bool ret = execute(sql.c_str(),nullptr) ;
	if (!ret) {
		throw MysqlcException(errNo(), (char*)error()) ;
	}
	return ret ;
}

bool Mysqlc::dropDataBase(const char* dbname) {
	std::string sql = "drop database if exists ";
        sql = sql + std::string(dbname) ;
        bool ret = execute(sql.c_str(),nullptr) ;
	if (!ret) {
		throw MysqlcException(errNo(), (char*)error()) ;
	}	
        return ret ;
}


bool Mysqlc::begin() {
	if (!execute("begin",nullptr)) {
		throw MysqlcException(errNo(), (char*)error()) ;
	}
	return true ;
}

bool Mysqlc::startTransaction() {
	return begin() ;
}

bool Mysqlc::commit() {
	if (!mysql_commit(_mysql)) {
		return true ;
	} else {
		throw MysqlcException(errNo(), (char*)error()) ;
	}
}

bool Mysqlc::rollback() {
	if (!mysql_rollback(_mysql)) {
		return true ;
	} else {
		throw MysqlcException(errNo(), (char*)error()) ;
	}
}

const char* Mysqlc::error() {
	return mysql_error(_mysql) ;
}

unsigned int Mysqlc::errNo() {
	return mysql_errno(_mysql) ;
}


bool Mysqlc::use(const char* dbname) {
	if (mysql_select_db(_mysql, dbname)) {
		throw MysqlcException(errNo(), (char*)error()) ;
	}
	return true ;
}

bool Mysqlc::autoCommit(bool mode) {
		if (mysql_autocommit(_mysql,mode ? '1' : '0')) {
			throw MysqlcException(errNo(), (char*)error()) ;
		}
		return true ;
}

bool Mysqlc::execute(const char* sql,std::function<void(long)> affected_rows) {
	return baseQuery(sql,[&](MYSQL_RES* res) {
		if(nullptr == res) {
			if (0 == mysql_field_count(_mysql)) {
				if (affected_rows) {
					affected_rows(mysql_affected_rows(_mysql)) ;
				}
				return true ;
			} else {
				throw MysqlcException(errNo(), (char*)error()) ;
			}
		} else {
			throw MysqlcException(errNo(), (char*)error()) ;
		}
	});
}

bool Mysqlc::baseQuery(const char* sql, std::function<bool(MYSQL_RES*)> query_reslut) {
	if (!mysql_query(_mysql, sql)) {
		MYSQL_RES* res = mysql_store_result(_mysql) ;
		bool retVal = query_reslut(res) ;
		if (res) mysql_free_result(res);
		return retVal ;
	}
	else {
		throw MysqlcException(errNo(), (char*)error()) ;
	}
}


bool Mysqlc::query(const char* sql, std::function<bool(MYSQL_ROW)> data_rows, std::function<void(void)> empty) {
	return baseQuery(sql, [&](MYSQL_RES* res){
		if (res) {
			bool retVal = true;
			if (mysql_num_rows(res)) {
				if (data_rows) {
					MYSQL_ROW row ;
					while ((row = mysql_fetch_row(res)) != NULL && retVal) {
						retVal = data_rows(row);
					}
				}
			}else {
				if (empty) {
					empty();
				}
			}
			return true ;
		} else {
			throw MysqlcException(errNo(), (char*)error()) ;
		}
	}) ;
}
