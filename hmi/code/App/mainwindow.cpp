#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "./WIFI/wifi.h"
#include "./UI/WIFI/dialogwificonnect.h"
#include "../Mqtt/ReadConf.h"
#include "../Mqtt/SendToAir.h"
#include "./ToUI/ToUI.h"
#include <QDebug>
#include <vector>
#include "../Base/base.h"
#include <QString>
#include "./Dbus/DbusSend.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scanMask = new DialogScanMask(this);
    updateMask = new Update(this);
    updateMask->hide();
    connect(WIFI_Single::instance(),SIGNAL(sig_scanOver(bool)),this,SLOT(slot_scanfOver(bool)));
    connect(this,SIGNAL(sig_scanWIFI()),WIFI_Single::instance(),SLOT(slot_scanWIFI()));
    connect(WIFI_Single::instance(),SIGNAL(sig_connectStatus(std::string)),this,SLOT(slot_wifiConnectChanged(std::string)));
    connect(NetMonitor_Single::instance(),SIGNAL(sig_netSatusChange(bool)),this,SLOT(slot_netStatusChange(bool)));
    connect(ReadConf_Single::instance(), SIGNAL(onAirVersonChange()), this,SLOT(slot_airVersonChange()));
    init();
}

MainWindow::~MainWindow()
{
    delete scanMask;
    delete updateMask;
    delete ui;
}

void MainWindow::init()
{
    //主页面中间工作部分
    //WIFI tab页
    //WIFI链接状态
    ui->WIFIConnect->setText("未链接");
    ui->netWork1->setText("网络不可用");
    ui->label->setText("设备号:");
    ui->WIFIName->setText("");
    NetMonitor_Single::instance()->monitorTimerStart();
    ui->shebeihao->setText(QString::fromStdString(ReadConf_Single::instance()->getID()));
    //主页面左上角版本信息
    ui->labVersonNum->setText(QString::fromStdString(ReadConf_Single::instance()->getVerson()));

}
void MainWindow:: clearQListWidget(QListWidget * ptr ,QList<QListWidgetItem*> & vec){
    if(ptr == nullptr || vec.size() == 0){
        return;
    }
    for(auto it = vec.begin();it !=vec.end();it++){
        ptr->removeItemWidget(*it);
    }
    qDeleteAll(vec.begin(), vec.end());
    vec.clear();
}

void MainWindow::slot_scanfOver( bool result){

    if(!result){
        mySleep(1000);
        scanMask->close();
        return;
    }
    clearQListWidget(ui->listWIFI,lstItem);
    for(auto it = lstItem.begin();it !=lstItem.end();it++){
        ui->listWIFI->removeItemWidget(*it);
    }
    qDeleteAll(lstItem.begin(), lstItem.end());
    lstItem.clear();
    for(auto val : WIFI_Single::instance()->getWifiNameVec()){
        QListWidgetItem* pItem=new QListWidgetItem();
        lstItem.push_back(pItem);
        pItem->setText(QString::fromStdString(val));//设置文字标签
        QFont font;
        font.setPointSize(18);
        pItem->setFont(font);
        pItem->setFlags(Qt::ItemIsSelectable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
        ui->listWIFI->addItem(pItem);
    }
    mySleep(1000);
    scanMask->close();

}

void MainWindow::slot_wifiConnectChanged(std::string name)
{
    if(QString::fromStdString(name) == ""){
        ui->WIFIConnect->setText("未连接");
        ui->WIFIName->setText("");
    }else{
       ui->WIFIConnect->setText("已连接");
       ui->WIFIName->setText(QString::fromStdString(name));
    }

}

void MainWindow::on_WIFIScan_clicked()
{
    emit sig_scanWIFI();

    scanMask->show();
}



void MainWindow::on_listWIFI_clicked(const QModelIndex &index)
{
    WIFI_Single::instance()->setConnectingWIFI(ui->listWIFI->currentItem()->text().toStdString());
    DialogWIFIConnect dialog(this);
    dialog.exec();

}

void MainWindow::on_connectWIFI_clicked()
{

}

void MainWindow::slot_netStatusChange(bool net)
{
    static bool status = false;
    if (status == net) {
        return;
    }else {
        status = net;
    }
    if (net) {
        ui->netWork1->setText("网络可用");
    }else {
        ui->netWork1->setText("网络不可用");
    }

}

void MainWindow::slot_airVersonChange()
{
    DEBUG_D("slot_airVersonChange");
    ui->label_lastVerson->setText(QString::fromStdString(ReadConf_Single::instance()->getAirVerson()));
}

void MainWindow::on_Btn_update_clicked()
{
    int newV = QString::fromStdString(ReadConf_Single::instance()->getAirVerson()).toInt();
    int oldV = QString::fromStdString(ReadConf_Single::instance()->getVerson()).toInt();
    if (newV <= oldV) return;

    updateMask->show();
    QString str = ";" + QString::fromStdString(ReadConf_Single::instance()->getID()) + ";2;1;";
    SendToAir_Single::instance()->addTaskQ(str.toStdString());

}

void MainWindow::on_Btn_air_version_clicked()
{
    QString str = ";" + QString::fromStdString(ReadConf_Single::instance()->getID()) + ";0;1;";
    SendToAir_Single::instance()->addTaskQ(str.toStdString());
}
