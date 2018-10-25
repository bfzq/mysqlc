#include <iostream>
#include "mysqlc.h"

int main() {
	_mysql_library_init
	Mysqlc* mysqlc = new Mysqlc() ;
	try{
	mysqlc->connect("127.0.0.1","root","!QAZ2wsx","t1",3306) ;
	mysqlc->setCharacterSet("utf8mb4") ;
	mysqlc->createDataBase("test2test2ttest2test2test2test2test2test2test2test2test2test2test2test2testtest2test2test2test2test2test2test2test2test2test2test2test2testtest2test2test2test2test2test2test2test2test2test2test2test2testest2test2test2test2test2test2test2test2test2test2test") ;
	printf("%s\n",mysqlc->error()) ;
	mysqlc->use("test2") ;
	printf("%u,%s\n",mysqlc->errNo(),mysqlc->error()) ;
	

	mysqlc->begin() ;
	
	mysqlc->execute("create table if not exists t1(id int)",[](long num) {
		printf("create table t1(id int), %ld\n", num) ;
	}) ;
	printf("%s\n",mysqlc->error()) ;
	
	mysqlc->execute("insert into t1(id) value(1)", [](long num) {
		printf("insert into t1(id) value(1), %ld\n", num) ;
	}) ;
	printf("2. %s\n",mysqlc->error()) ;
	
	mysqlc->execute("insert into t1(id) value(2)", nullptr) ;
	printf("3. %s\n",mysqlc->error()) ;
	mysqlc->query("select * from t1", [](MYSQL_ROW row){
		printf("select * from t1, %s\n",row[0]) ;
		return true ;
	},[](){
		printf("empty\n") ;
	}) ;
	mysqlc->commit() ;
	
	mysqlc->query("select * from t2", nullptr, nullptr) ;
	printf("%s\n",mysqlc->error()) ;
	
	mysqlc->disConnect();
	}
	catch(MysqlcException & me) {
		me.what() ;
	}
	delete mysqlc;
	_mysql_library_end
	return 0 ;
}
