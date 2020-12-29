/********************************************************************************
** Form generated from reading UI file 'mail.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIL_H
#define UI_MAIL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mail
{
public:
    QWidget *centralwidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QLabel *label;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *mail)
    {
        if (mail->objectName().isEmpty())
            mail->setObjectName(QString::fromUtf8("mail"));
        mail->resize(349, 252);
        centralwidget = new QWidget(mail);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(120, 160, 91, 31));
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(110, 60, 111, 31));
        pushButton_3 = new QPushButton(centralwidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(110, 100, 111, 31));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(133, 19, 71, 31));
        mail->setCentralWidget(centralwidget);
        menubar = new QMenuBar(mail);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 349, 21));
        mail->setMenuBar(menubar);
        statusbar = new QStatusBar(mail);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        mail->setStatusBar(statusbar);

        retranslateUi(mail);
        QObject::connect(pushButton, SIGNAL(clicked()), mail, SLOT(close()));

        QMetaObject::connectSlotsByName(mail);
    } // setupUi

    void retranslateUi(QMainWindow *mail)
    {
        mail->setWindowTitle(QCoreApplication::translate("mail", "mail", nullptr));
        pushButton->setText(QCoreApplication::translate("mail", "\351\200\200\345\207\272", nullptr));
        pushButton_2->setText(QCoreApplication::translate("mail", "\346\234\254\345\234\260\351\202\256\347\256\261\347\231\273\351\231\206", nullptr));
        pushButton_3->setText(QCoreApplication::translate("mail", "\346\234\254\345\234\260\351\202\256\347\256\261\346\263\250\345\206\214", nullptr));
        label->setText(QCoreApplication::translate("mail", "\351\202\256\344\273\266\347\263\273\347\273\237", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mail: public Ui_mail {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIL_H
