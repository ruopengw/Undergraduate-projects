#ifndef EMAIL_WRITE_H
#define EMAIL_WRITE_H

#include <QDialog>
using namespace std;
namespace Ui {
class Email_write;
}

class Email_write : public QDialog
{
    Q_OBJECT

public:
    string address;
    explicit Email_write(QWidget *parent = nullptr);
    ~Email_write();

private:
    Ui::Email_write *ui;

private slots:
    void write_send();
};

#endif // EMAIL_WRITE_H
