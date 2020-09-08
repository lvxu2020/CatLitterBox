/********************************************************************************
** Form generated from reading UI file 'dialogconnectmask.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGCONNECTMASK_H
#define UI_DIALOGCONNECTMASK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_DialogConnectMask
{
public:
    QLabel *connectStatus;

    void setupUi(QDialog *DialogConnectMask)
    {
        if (DialogConnectMask->objectName().isEmpty())
            DialogConnectMask->setObjectName(QString::fromUtf8("DialogConnectMask"));
        DialogConnectMask->resize(600, 350);
        connectStatus = new QLabel(DialogConnectMask);
        connectStatus->setObjectName(QString::fromUtf8("connectStatus"));
        connectStatus->setGeometry(QRect(200, 140, 191, 41));

        retranslateUi(DialogConnectMask);

        QMetaObject::connectSlotsByName(DialogConnectMask);
    } // setupUi

    void retranslateUi(QDialog *DialogConnectMask)
    {
        DialogConnectMask->setWindowTitle(QCoreApplication::translate("DialogConnectMask", "Dialog", nullptr));
        connectStatus->setText(QCoreApplication::translate("DialogConnectMask", "\350\277\236\346\216\245\344\270\255\350\257\267\347\250\215\345\200\231....", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogConnectMask: public Ui_DialogConnectMask {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGCONNECTMASK_H
