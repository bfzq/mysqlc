CC = g++
DEBUG = -g
PROGRAM = mysqlc
OS = $(shell uname -s)
ifeq ($(OS),Linux)
	MYSQL_INCLUDE = `mysql_config --cflags`
	MYSQL_LIB = `mysql_config --variable=pkglibdir`/libmysqlclient.a
else 
	MYSQL_INCLUDE = -I/usr/local/mysql/include
	MYSQL_LIB = /usr/local/mysql/lib/libmysqlclient.a
endif


MAKE = make

CFLAGS = -Wall -std=c++11 $(MYSQL_INCLUDE)
LFLAGS = -ldl -lpthread --std=c++11 
LIBLAGS = $(MYSQL_LIB)

OBJECTS = main.o mysqlc.o


$(PROGRAM):$(OBJECTS)
	$(CC) $(LFLAGS) -o $(PROGRAM) $(OBJECTS) $(LIBLAGS)
%.o:%.cpp
	$(CC) $(DEBUG) $(CFLAGS)  -c $< -o $@
clean:
	rm -rf *.o $(PROGRAM)
