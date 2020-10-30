#ifndef EMAIL_H
#define EMAIL_H

#include <QDialog>
#include"Email_read.h"
#include"Email_write.h"
#include<string>

class Email_read;
class Email_write;

namespace Ui {
class Email;
}

class Email : public QDialog
{
    Q_OBJECT

public:
    string address;
    explicit Email(QWidget *parent = nullptr);
    ~Email();

private:
    Ui::Email *ui;
    Email_write* _email_write = nullptr;
    Email_read* _email_read = nullptr;

private slots:
    void mailbox();
    void write();
};

#endif // EMAIL_H
