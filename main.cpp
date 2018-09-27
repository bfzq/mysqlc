#include <iostream>
#include "mysqlc.h"

int main() {
	Mysqlc* mysqlc = new Mysqlc() ;
	mysqlc->connect("127.0.0.1","root","!QAZ2wsx","t1",3306) ;
	mysqlc->disConnect();
	delete mysqlc ;
	return 0 ;
}
