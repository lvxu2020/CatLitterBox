/********************************************************************************
** Form generated from reading UI file 'dialogscanmask.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGSCANMASK_H
#define UI_DIALOGSCANMASK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_DialogScanMask
{
public:
    QLabel *label_1;

    void setupUi(QDialog *DialogScanMask)
    {
        if (DialogScanMask->objectName().isEmpty())
            DialogScanMask->setObjectName(QString::fromUtf8("DialogScanMask"));
        DialogScanMask->resize(640, 400);
        QFont font;
        font.setPointSize(18);
        DialogScanMask->setFont(font);
        DialogScanMask->setStyleSheet(QString::fromUtf8(""));
        label_1 = new QLabel(DialogScanMask);
        label_1->setObjectName(QString::fromUtf8("label_1"));
        label_1->setGeometry(QRect(240, 170, 101, 31));
        label_1->setFont(font);

        retranslateUi(DialogScanMask);

        QMetaObject::connectSlotsByName(DialogScanMask);
    } // setupUi

    void retranslateUi(QDialog *DialogScanMask)
    {
        DialogScanMask->setWindowTitle(QCoreApplication::translate("DialogScanMask", "Dialog", nullptr));
        label_1->setText(QCoreApplication::translate("DialogScanMask", "\346\211\253\346\217\217\344\270\255...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogScanMask: public Ui_DialogScanMask {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGSCANMASK_H
