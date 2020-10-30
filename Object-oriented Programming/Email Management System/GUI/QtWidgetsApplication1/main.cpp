#include "mail.h"
#include <QApplication>
#include "Email_operation.h"
#include "Email.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mail w;
    Email e;
    w.show();
    return a.exec();
}
