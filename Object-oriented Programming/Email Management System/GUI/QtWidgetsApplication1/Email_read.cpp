#include "Email_read.h"
#include "ui_Email_read.h"
#include "sqlite3.h"

Email_read::Email_read(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Email_read)
{
    ui->setupUi(this);
    _email_list();
	connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(_email_read()));
}

Email_read::~Email_read()
{
    delete ui;
}

void Email_read::_email_list()
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	char* sql;
	char** result;
	int nRow, nColumn;
	string s;

	/* Open database */
	rc = sqlite3_open("EmailSys.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}

	/* Create SQL statement */
	s = "SELECT ID,TITLE,SENDER,ATTACHPATH,TIME from EmailLib WHERE RECEIVER ='" + this->address + "'" + "ORDER BY ID;";
	sql = (char*)s.data();

	/* Execute SQL statement */
	rc = sqlite3_get_table(db, sql, &result, &nRow, &nColumn, &zErrMsg);
	int index = nColumn;
	if (rc == SQLITE_OK) {
		ui->plainTextEdit->insertPlainText( "ID	title 	sender   attach	  time\n" );
		for (int i = 0; i < nRow; i++) {
			for (int j = 0; j < 5; j++) {
				ui->plainTextEdit->insertPlainText(result[i*5+ j]);
				ui->plainTextEdit->insertPlainText(" ");
			}
			ui->plainTextEdit->insertPlainText("\n");
//			index += 5;
		}

	}
	sqlite3_close(db);
}

void Email_read::_email_read()
{
	string ID = ui->lineEdit->text().toStdString();
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	char* sql;
	char** result;
	int nRow, nColumn;
	string s;

	/* Open database */
	rc = sqlite3_open("EmailSys.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}

	/* Create SQL statement */
	s = "SELECT TITLE,SENDER,CONTENT from EmailLib WHERE ID ='" + ID + "';";
	sql = (char*)s.data();

	/* Execute SQL statement */
	rc = sqlite3_get_table(db, sql, &result, &nRow, &nColumn, &zErrMsg);
	if (rc == SQLITE_OK) {
		ui->textBrowser->clear();
		ui->textBrowser->insertPlainText( ">>>title:");
		ui->textBrowser->insertPlainText(result[3]);
		ui->textBrowser->insertPlainText("\n");
		ui->textBrowser->insertPlainText(">>>sender:");
		ui->textBrowser->insertPlainText(result[4]);
		ui->textBrowser->insertPlainText("\n");
		ui->textBrowser->insertPlainText( ">>>content:");
		ui->textBrowser->insertPlainText(result[5]);
	}
	sqlite3_close(db);
}
