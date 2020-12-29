#include "Email.h"
#include "ui_Email.h"
#include <iostream>
#include "sqlite_operation.h"
#include "Email_operation.h"
#include<QDialog>
using namespace std;
Email::Email(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Email)
{
    ui->setupUi(this);
    _email_read = new Email_read;
    _email_write = new Email_write;
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(write()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(mailbox()));
}

Email::~Email()
{
    delete ui;
}

void Email::mailbox() {
    _email_read->address = this->address;
    _email_read->show();
}

void Email::write() {
    _email_write->address = this->address;
    _email_write->show();
}