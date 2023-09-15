#include "dialogscanmask.h"
#include "ui_dialogscanmask.h"

DialogScanMask::DialogScanMask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogScanMask)
{
    ui->setupUi(this);
}

DialogScanMask::~DialogScanMask()
{
    delete ui;
}
