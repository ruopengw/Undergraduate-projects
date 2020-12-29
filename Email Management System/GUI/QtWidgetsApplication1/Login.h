#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
class Email;
namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private:
    Ui::Login *ui;
    Email * _email = nullptr;

private slots:
    void _login();
};

#endif // LOGIN_H
