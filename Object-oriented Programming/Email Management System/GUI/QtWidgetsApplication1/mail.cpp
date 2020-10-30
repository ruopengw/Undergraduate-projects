#include "mail.h"
#include "ui_mail.h"
#include "Login.h"
#include "Register.h"

using namespace std;

mail::mail(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mail)
{
    ui->setupUi(this);
    _login = new Login;
    _register = new Register;
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(slotBtn()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(slotBtn2()));
}

mail::~mail()
{
    delete ui;
}

void mail::slotBtn() {
    _login->show();
}

void mail::slotBtn2() {
    _register->show();
}
