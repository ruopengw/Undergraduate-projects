#ifndef MAIL_H
#define MAIL_H

#include <QMainwindow>
class Login;
class Register;
QT_BEGIN_NAMESPACE
namespace Ui { 
	class mail;
}
QT_END_NAMESPACE

class mail : public QMainWindow
{
    Q_OBJECT

public:
    mail( QWidget* parent = nullptr);
    ~mail();

private slots:
    void slotBtn();//for log in
    void slotBtn2();// for register

private:
    Ui::mail *ui;
    Login* _login = nullptr;
    Register* _register = nullptr;
};
#endif // MAIL
