#include "Register.h"
#include "ui_Register.h"
#include<string>
#include "sqlite_operation.h"
#include"qmessagebox.h"

Register::Register(QWidget *parent):
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(regis()));
}

Register::~Register()
{
    delete ui;
}

void Register::regis() {
    int check;
    string s;
    char* sql;
    string address, code;
    address = ui->lineEdit->text().toStdString();
    code = ui->lineEdit_2->text().toStdString();
    s = "INSERT INTO EmailAccount (ADDRESS,PASSWORD) VALUES ('" + address + "','" + code + "');";
    sql = (char*)s.data();
    check = Check_Account(address);
    if (check == 1) {
        QMessageBox::critical(this, "ERROR", "this address has been registered");
    }
    else {
        check = Insert_Record(sql);
        if(check==1) QMessageBox::information(this, "message", "register success");
        else QMessageBox::critical(this, "ERROR", "register fail");
    }
}
