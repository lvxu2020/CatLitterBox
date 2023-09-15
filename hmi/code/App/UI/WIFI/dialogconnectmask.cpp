#include "dialogconnectmask.h"
#include "ui_dialogconnectmask.h"
#include "../../WIFI/wifi.h"

DialogConnectMask::DialogConnectMask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConnectMask)
{
    ui->setupUi(this);
}

DialogConnectMask::~DialogConnectMask()
{
    delete ui;
}

void DialogConnectMask::setConnectStatus(QString txt)
{
    ui->connectStatus->setText(txt);
}
