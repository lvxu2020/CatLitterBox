/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *WIFI;
    QListWidget *listWIFI;
    QPushButton *WIFIScan;
    QPushButton *connectWIFI;
    QLabel *WIFIConnect;
    QLabel *WIFIName;
    QLabel *netWork1;
    QLabel *label;
    QLabel *sheibeihao;
    QWidget *manual;
    QLabel *labVerson;
    QLabel *labVersonNum;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 480);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(140, 10, 621, 441));
        WIFI = new QWidget();
        WIFI->setObjectName(QString::fromUtf8("WIFI"));
        listWIFI = new QListWidget(WIFI);
        listWIFI->setObjectName(QString::fromUtf8("listWIFI"));
        listWIFI->setGeometry(QRect(145, 61, 461, 341));
        WIFIScan = new QPushButton(WIFI);
        WIFIScan->setObjectName(QString::fromUtf8("WIFIScan"));
        WIFIScan->setGeometry(QRect(20, 110, 89, 25));
        connectWIFI = new QPushButton(WIFI);
        connectWIFI->setObjectName(QString::fromUtf8("connectWIFI"));
        connectWIFI->setGeometry(QRect(20, 210, 89, 25));
        WIFIConnect = new QLabel(WIFI);
        WIFIConnect->setObjectName(QString::fromUtf8("WIFIConnect"));
        WIFIConnect->setGeometry(QRect(30, 20, 67, 17));
        netWork1 = new QLabel(WIFI);
        netWork1->setObjectName(QString::fromUtf8("netWork1"));
        netWork1->setGeometry(QRect(20, 50, 101, 21));
        sheibeihao = new QLabel(WIFI);
        sheibeihao->setObjectName(QString::fromUtf8("sheibeihao"));
        sheibeihao->setGeometry(QRect(80, 310, 51, 16));
        label = new QLabel(WIFI);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 310, 70, 17));

        WIFIName = new QLabel(WIFI);
        WIFIName->setObjectName(QString::fromUtf8("WIFIName"));
        WIFIName->setGeometry(QRect(150, 20, 451, 20));
        tabWidget->addTab(WIFI, QString());
        manual = new QWidget();
        manual->setObjectName(QString::fromUtf8("manual"));
        tabWidget->addTab(manual, QString());
        labVerson = new QLabel(centralwidget);
        labVerson->setObjectName(QString::fromUtf8("labVerson"));
        labVerson->setGeometry(QRect(10, 10, 71, 21));
        labVersonNum = new QLabel(centralwidget);
        labVersonNum->setObjectName(QString::fromUtf8("labVersonNum"));
        labVersonNum->setGeometry(QRect(80, 10, 61, 21));
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        WIFIScan->setText(QCoreApplication::translate("MainWindow", "\346\211\253\346\217\217", nullptr));
        connectWIFI->setText(QCoreApplication::translate("MainWindow", "\350\277\236\346\216\245", nullptr));
        WIFIConnect->setText(QCoreApplication::translate("MainWindow", "\346\234\252\350\277\236\346\216\245", nullptr));
        WIFIName->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(WIFI), QCoreApplication::translate("MainWindow", "WIFI", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(manual), QCoreApplication::translate("MainWindow", "\346\211\213\345\212\250", nullptr));
        labVerson->setText(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215\347\211\210\346\234\254\357\274\232\n"
"", nullptr));
        labVersonNum->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
