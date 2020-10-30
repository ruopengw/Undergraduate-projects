#include <iostream>
#include "sqlite3.h"
#include <string>
#include<cstdlib>
#include "sqlite_func.h"
#pragma comment(lib, "sqlite3.lib")
using namespace std;

//static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
//	int i;
//	for (i = 0; i < argc; i++) {
//		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
//	}
//	printf("\n");
//	return 0;
//}

int main() {
	cout << "Hello world" << endl;
	
	/*连接数据库*/
	char* db_name=new char;
	cin >> db_name;
	Create_database(db_name);

	/*创建表*/
	char* tableinfo = new char;
	tableinfo = "CREATE TABLE EmailAccount("  \
		"ADDRESS CHAR(50) PRIMARY KEY     NOT NULL," \
		"password           char(50)    not null);";
	Create_table(db_name, tableinfo);
	tableinfo = "CREATE TABLE EmailLib("  \
		"ID INTEGER PRIMARY KEY   AUTOINCREMENT,"\
		"RECEIVER CHAR(50)      NOT NULL," \
		"SENDER           CHAR(50)      NOT NULL," \
		"TIME	DATETIME DEFAULT CURRENT_TIMESTAMP,"\
		"TITLE           CHAR(30)    NOT NULL,"\
		"CONTENT           TEXT    NOT NULL,"\
		"AttachPath		CHAR(100)	DEFAULT NULL);";

	Create_table(db_name,tableinfo);
	cout << "Hello world" << endl;

	/*插入*/

	char* record_1;
	char* record_2;
	record_1= "INSERT INTO EmailAccount (ADDRESS,PASSWORD)" \
		"VALUES ('sukikadaki@oop.com','love_lijian1998');" \
		"INSERT INTO EmailAccount (ADDRESS,PASSWORD)" \
		"VALUES ('lijian@oop.com','1998_love_suki' );";
	record_2= "INSERT INTO EmailLib (RECEIVER,SENDER,TITLE,CONTENT)" \
		"VALUES ('sukikadaki@oop.com','lijian@oop.com','Test mail1','hello,this is a test email.');" \
		"INSERT INTO EmailLib (RECEIVER,SENDER,TITLE,CONTENT)" \
		"VALUES ('sukikadaki@oop.com','lijian@oop.com','Test mail2','hello,this is also a test email.');" \
		"INSERT INTO EmailLib (RECEIVER,SENDER,TITLE,CONTENT)" \
		"VALUES ('sukikadaki@oop.com','lijian@oop.com','How are you?','hello,\n this is not a test email.');" \
		"INSERT INTO EmailLib (RECEIVER,SENDER,TITLE,CONTENT)" \
		"VALUES ('lijian@oop.com','1998_love_suki','Testing','hello\n,hello\n.love ya' );"\
		"INSERT INTO EmailLib (RECEIVER,SENDER,TITLE,CONTENT)" \
		"VALUES ('lijian@oop.com','1998_love_suki','Test ending','hello,this is a test email.love ya' );";

	Insert_Record(record_1);
	Insert_Record(record_2);
	cout << "Hello world" << endl;

	/*删除*/

	system("pause");
}