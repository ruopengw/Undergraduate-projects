#include <iostream>
#include "sqlite3.h"
#include <string>
#include<cstdlib>
#include "sqlite_operation.h"
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

int Insert_Record(char* record) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	int check;

	/* Open database */
	rc = sqlite3_open("EmailSys.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}

	/* Execute SQL statement */
	rc = sqlite3_exec(db, record, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		sqlite3_free(zErrMsg);
		check = 0;
	}
	else {
		check = 1;
	}

	sqlite3_close(db);
	return check;
}

int Check_Account(string address) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	char **result;
	int check=0,nRow,nColumn;
	string s;

	/* Open database */
	rc = sqlite3_open("EmailSys.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}

	/* Create SQL statement */
	s="SELECT * from EmailAccount WHERE ADDRESS ='"+ address+"';";
	sql = (char*)s.data();

	/* Execute SQL statement */
	rc = sqlite3_get_table(db, sql, &result,&nRow,&nColumn, &zErrMsg);
	check = nRow;
	sqlite3_close(db);
	return check;
}

int Login_Check_Account(string address,string password) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	char **result;
	int check = 0, nRow, nColumn;
	string s;

	/* Open database */
	rc = sqlite3_open("EmailSys.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}

	/* Create SQL statement */
	s = "SELECT * from EmailAccount WHERE ADDRESS ='" + address + "' AND PASSWORD ='"+password+"'"+";" ;
	sql = (char*)s.data();

	/* Execute SQL statement */
	rc = sqlite3_get_table(db, sql, &result, &nRow, &nColumn, &zErrMsg);
	if (rc == SQLITE_OK && nRow == 1) {
		check = 1;
	}
	sqlite3_close(db);
	return check;
}

void Email_list(string address) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	char **result;
	int nRow, nColumn;
	string s;

	/* Open database */
	rc = sqlite3_open("EmailSys.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}

	/* Create SQL statement */
	s = "SELECT ID,READ_SIGN,TITLE,SENDER,ATTACHPATH,TIME from EmailLib WHERE RECEIVER ='" + address \
		+ "'AND DELETE_STATUS = 0 ORDER BY ID;";
	sql = (char*)s.data();

	/* Execute SQL statement */
	rc = sqlite3_get_table(db, sql, &result, &nRow, &nColumn, &zErrMsg);
	int index = nColumn;
	if (rc == SQLITE_OK) {
		cout << "ID	状态	标题		发件人		附件地址	发送时间" << endl;
		for (int i = 0; i < nRow; i++) {
			cout << result[index] << " " << result[index + 1] << " " << result[index + 2] <<" "\
				<< result[index+3] << " " << result[index + 4] << " " << result[index+5]<< endl;
			index += 6;
		}
		
	}
	else {
		cout << "something wrong" << endl;
	}
	sqlite3_close(db);
}

void Show_Email(string ID) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	char **result;
	int nRow, nColumn;
	string s;

	/* Open database */
	rc = sqlite3_open("EmailSys.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}

	/* Create SQL statement */
	s = "SELECT TITLE,SENDER,CONTENT from EmailLib WHERE ID ='" + ID+ "';";
	sql = (char*)s.data();

	/* Execute SQL statement */
	rc = sqlite3_get_table(db, sql, &result, &nRow, &nColumn, &zErrMsg);
	if (rc == SQLITE_OK) {
		cout << "----------------------------------------------------" << endl;
		cout << ">>>标题：" << result[3]<<endl;
		cout << ">>>发件人：" << result[4] << endl;
		cout << ">>>正文：" << endl;
		cout << result[5] << endl;
		cout << "----------------------------------------------------" << endl;
	}
	sqlite3_close(db);
}

void Email_Send(string address, string receiver, string title, string content,string path) {
	string s;
	char *sql;
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	/* Open database */
	rc = sqlite3_open("EmailSys.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}

	s = "INSERT INTO EmailLib (RECEIVER,SENDER,TITLE,CONTENT,ATTACHPATH) VALUES ('" + receiver + \
		"','" + address + "','"+title+"','"+content+"','"+path+"');";
	sql = (char*)s.data();

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
	if (rc != SQLITE_OK) {
		cout << "发送失败。" << endl;
	}
	else {
		cout << "发送成功！" << endl;
	}

	sqlite3_close(db);

}

void Email_Delete(string ID) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	char **result;
	int check = 0, nRow, nColumn;
	string s;

	/* Open database */
	rc = sqlite3_open("EmailSys.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}

	/* Create SQL statement */
	s = "UPDATE EmailLib SET DELETE_STATUS = 1 WHERE ID ='" + ID + "';";
	sql = (char*)s.data();

	/* Execute SQL statement */
	rc = sqlite3_get_table(db, sql, &result, &nRow, &nColumn, &zErrMsg);
	if (rc == SQLITE_OK) {
		cout << "已删除！" << endl;
	}
	sqlite3_close(db);
}

void Email_Sign(string ID) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	char **result;
	int check = 0, nRow, nColumn;
	string s;

	/* Open database */
	rc = sqlite3_open("EmailSys.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}

	/* Create SQL statement */
	s = "UPDATE EmailLib SET READ_SIGN = '[HasRead]' WHERE ID ='" + ID + "';";
	sql = (char*)s.data();

	/* Execute SQL statement */
	rc = sqlite3_get_table(db, sql, &result, &nRow, &nColumn, &zErrMsg);
	if (rc == SQLITE_OK) {
		cout << "Done！" << endl;
	}
	sqlite3_close(db);
}