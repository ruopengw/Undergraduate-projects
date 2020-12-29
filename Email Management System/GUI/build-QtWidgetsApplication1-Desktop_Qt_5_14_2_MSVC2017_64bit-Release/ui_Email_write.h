/********************************************************************************
** Form generated from reading UI file 'Email_write.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMAIL_WRITE_H
#define UI_EMAIL_WRITE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_Email_write
{
public:
    QPushButton *pushButton;
    QTextEdit *textEdit;
    QTextEdit *textEdit_2;
    QPushButton *pushButton_3;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLabel *label_4;

    void setupUi(QDialog *Email_write)
    {
        if (Email_write->objectName().isEmpty())
            Email_write->setObjectName(QString::fromUtf8("Email_write"));
        Email_write->resize(375, 300);
        pushButton = new QPushButton(Email_write);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(150, 220, 80, 31));
        textEdit = new QTextEdit(Email_write);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(120, 60, 151, 31));
        textEdit_2 = new QTextEdit(Email_write);
        textEdit_2->setObjectName(QString::fromUtf8("textEdit_2"));
        textEdit_2->setGeometry(QRect(120, 100, 151, 31));
        pushButton_3 = new QPushButton(Email_write);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(150, 180, 81, 31));
        label = new QLabel(Email_write);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 25, 51, 21));
        label_2 = new QLabel(Email_write);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(50, 70, 54, 12));
        label_3 = new QLabel(Email_write);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(50, 110, 54, 12));
        lineEdit = new QLineEdit(Email_write);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(110, 19, 161, 31));
        lineEdit_2 = new QLineEdit(Email_write);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(110, 140, 161, 31));
        label_4 = new QLabel(Email_write);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(40, 140, 61, 21));

        retranslateUi(Email_write);
        QObject::connect(pushButton, SIGNAL(clicked()), Email_write, SLOT(close()));

        QMetaObject::connectSlotsByName(Email_write);
    } // setupUi

    void retranslateUi(QDialog *Email_write)
    {
        Email_write->setWindowTitle(QCoreApplication::translate("Email_write", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("Email_write", "\350\277\224\345\233\236", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Email_write", "\345\217\221\351\200\201", nullptr));
        label->setText(QCoreApplication::translate("Email_write", "\346\224\266\344\273\266\344\272\272", nullptr));
        label_2->setText(QCoreApplication::translate("Email_write", "\346\240\207\351\242\230", nullptr));
        label_3->setText(QCoreApplication::translate("Email_write", "\346\255\243\346\226\207", nullptr));
        label_4->setText(QCoreApplication::translate("Email_write", "\351\231\204\344\273\266\345\234\260\345\235\200", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Email_write: public Ui_Email_write {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMAIL_WRITE_H
