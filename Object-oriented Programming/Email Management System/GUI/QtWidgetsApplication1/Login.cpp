#include "Login.h"
#include "ui_Login.h"
#include<string>
#include "sqlite_operation.h"
#include"qmessagebox.h"
#include"Email.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(_login()));
    _email = new Email;
}

Login::~Login()
{
    delete ui;
}

void Login::_login() {
    int check;
    string s;
    char* sql;
    string address, code;
    address = ui->lineEdit->text().toStdString();
    code = ui->lineEdit_2->text().toStdString();
    check = Login_Check_Account(address, code);
    if (check == 0) {
        QMessageBox::critical(this, "ERROR", "wrong name or wrong password");
    }
    else {
        _email->address = address;
        _email->show();
    }

}
