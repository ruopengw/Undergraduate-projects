#ifndef EMAIL_READ_H
#define EMAIL_READ_H

#include <QDialog>
#include<string>
using namespace std;

namespace Ui {
class Email_read;
}

class Email_read : public QDialog
{
    Q_OBJECT

public:
    string address;
    explicit Email_read(QWidget *parent = nullptr);
    ~Email_read();

private:
    Ui::Email_read *ui;


private slots:
    void _email_read();
    void _email_list();
};

#endif // EMAIL_READ_H
