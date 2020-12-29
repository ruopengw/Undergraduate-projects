/********************************************************************************
** Form generated from reading UI file 'Email.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMAIL_H
#define UI_EMAIL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Email
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QLabel *label;

    void setupUi(QDialog *Email)
    {
        if (Email->objectName().isEmpty())
            Email->setObjectName(QString::fromUtf8("Email"));
        Email->resize(354, 278);
        pushButton = new QPushButton(Email);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(130, 50, 91, 31));
        pushButton_2 = new QPushButton(Email);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(130, 90, 91, 31));
        pushButton_3 = new QPushButton(Email);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(130, 130, 91, 31));
        label = new QLabel(Email);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(160, 20, 51, 21));

        retranslateUi(Email);
        QObject::connect(pushButton, SIGNAL(clicked()), Email, SLOT(close()));

        QMetaObject::connectSlotsByName(Email);
    } // setupUi

    void retranslateUi(QDialog *Email)
    {
        Email->setWindowTitle(QCoreApplication::translate("Email", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("Email", "\351\200\200\345\207\272\347\231\273\351\231\206", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Email", "\345\206\231\344\277\241", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Email", "\346\224\266\344\273\266\347\256\261", nullptr));
        label->setText(QCoreApplication::translate("Email", "\346\254\242\350\277\216", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Email: public Ui_Email {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMAIL_H
