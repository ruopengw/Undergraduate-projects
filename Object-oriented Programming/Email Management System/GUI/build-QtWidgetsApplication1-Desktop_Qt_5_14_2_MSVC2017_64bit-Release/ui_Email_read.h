/********************************************************************************
** Form generated from reading UI file 'Email_read.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMAIL_READ_H
#define UI_EMAIL_READ_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_Email_read
{
public:
    QPushButton *pushButton;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *pushButton_2;
    QLineEdit *lineEdit;
    QPlainTextEdit *plainTextEdit;
    QTextBrowser *textBrowser;

    void setupUi(QDialog *Email_read)
    {
        if (Email_read->objectName().isEmpty())
            Email_read->setObjectName(QString::fromUtf8("Email_read"));
        Email_read->resize(400, 300);
        pushButton = new QPushButton(Email_read);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(160, 260, 91, 31));
        label = new QLabel(Email_read);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(170, 19, 101, 21));
        label_2 = new QLabel(Email_read);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(180, 160, 61, 21));
        label_3 = new QLabel(Email_read);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(40, 110, 71, 31));
        pushButton_2 = new QPushButton(Email_read);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(220, 110, 91, 31));
        lineEdit = new QLineEdit(Email_read);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(102, 110, 111, 31));
        plainTextEdit = new QPlainTextEdit(Email_read);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(70, 40, 271, 61));
        textBrowser = new QTextBrowser(Email_read);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(60, 180, 281, 71));

        retranslateUi(Email_read);
        QObject::connect(pushButton, SIGNAL(clicked()), Email_read, SLOT(close()));

        QMetaObject::connectSlotsByName(Email_read);
    } // setupUi

    void retranslateUi(QDialog *Email_read)
    {
        Email_read->setWindowTitle(QCoreApplication::translate("Email_read", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("Email_read", "\350\277\224\345\233\236", nullptr));
        label->setText(QCoreApplication::translate("Email_read", "\351\202\256\344\273\266\345\210\227\350\241\250", nullptr));
        label_2->setText(QCoreApplication::translate("Email_read", "\351\202\256\344\273\266\345\206\205\345\256\271", nullptr));
        label_3->setText(QCoreApplication::translate("Email_read", "\351\202\256\344\273\266ID:", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Email_read", "\346\237\245\346\211\276", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Email_read: public Ui_Email_read {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMAIL_READ_H
