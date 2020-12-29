#include <iostream>
#include "sqlite3.h"
#include <string>
#include<cstdlib>
#include "sqlite_func.h"
#pragma comment(lib, "sqlite3.lib")
using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

/*创建数据库EmailSys*/
void Create_database(char* db_name) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;


	/* Open database */
	rc = sqlite3_open(db_name, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else {
		fprintf(stdout, "Opened database successfully\n");
	}
	sqlite3_close(db);
}

/*创建表*/

void Create_table(char* Database,char* TableInfo) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	rc = sqlite3_open(Database, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}

	/* Execute SQL statement */
	rc = sqlite3_exec(db, TableInfo, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Table created successfully\n");
	}

	sqlite3_close(db);
}

/*插入信息*/

void Insert_Record(char* record) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	/* Open database */
	rc = sqlite3_open("EmailSys.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else {
		fprintf(stderr, "Opened database successfully\n");
	}

	///* Create SQL statement */
	//sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
	//	"VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
	//	"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
	//	"VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
	//	"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
	//	"VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
	//	"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
	//	"VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, record, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Records created successfully\n");
	}

	sqlite3_close(db);
}

bool Check_Account(string address) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	int check;
	string s;
	/*const char* data = "Callback function called";*/

	/* Open database */
	rc = sqlite3_open("EmailSys.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	/*else {
		fprintf(stderr, "Opened database successfully\n");
	}*/

	/* Create SQL statement */
	s = "SELECT ADDRESS from EmailAccount WHERE ADDRESS ='"+address+"');";
	sql = (char*)s.data();

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback,0, &zErrMsg);
	if (rc != SQLITE_OK) {
		check = 1;
	}
	else {
		check = 0;
	}
	sqlite3_close(db);
	if (check == 1) {
		return true;
	}
	else {
		return false;
	}
}