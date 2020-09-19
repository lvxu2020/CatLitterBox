#include "Update.h"
#include "ui_Update.h"
#include "ToUI/ToUI.h"
#include "../Base/base.h"

Update::Update(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Update)
{
    ui->setupUi(this);
    connect(ToUI_Single::instance(), SIGNAL(sig_updateResault(bool)), this,SLOT(slot_updateResault(bool)));
    connect(ToUI_Single::instance(), SIGNAL(sig_updateTimerStart()), this,SLOT(slot_shutTimerStart()));
    // 升级无响应40秒后重启
    update = new QTimer(this);
    update->setInterval(30000);
    update->setSingleShot(true);//触发一次
    connect(update,SIGNAL(timeout()),this,SLOT(slot_scanOver()));
}

Update::~Update()
{
    delete ui;
}

void Update::slot_updateResault(bool res)
{
    if (res) {
        ui->label->setText("升级成功，即将重启");
    } else {
        ui->label->setText("升级失败，稍后重启");
    }
    mySleep(4000);
    system("sudo reboot");
}

void Update::slot_shutDowd()
{
    system("sudo reboot");
}

void Update::slot_shutTimerStart()
{
    update->start();
}
