#include "Email_write.h"
#include "ui_Email_write.h"
#include<string>
#include "sqlite_operation.h"
#include <QtWidgets\qmessagebox.h>
#include<iostream>
using namespace std;

Email_write::Email_write(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Email_write)
{
    ui->setupUi(this);
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(write_send()));
}

Email_write::~Email_write()
{
    delete ui;
}

void Email_write::write_send()
{
    string receiver;
    string content;
    string title;
    string path="";
    int check;

    receiver = ui->lineEdit->text().toStdString();
    title = ui->textEdit->toPlainText().toStdString();
    content = ui->textEdit_2->toPlainText().toStdString();
    path = ui->lineEdit_2->text().toStdString();
    check = Check_Account(receiver);
//    cout << "test";
    if (check == 0) {
        QMessageBox::critical(this, "ERROR", "the email address is not exist");
    }
    else {
        Email_Send(this->address, receiver, title, content, path);
    }
}