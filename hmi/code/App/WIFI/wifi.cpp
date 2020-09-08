#include "wifi.h"

#include <QTime>
#include <QDebug>
#include <string.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <unistd.h>
#include <stdio.h>

#include "../../Base/base.h"

#define SCAN_WIFI_NAME "/home/debian/Cat/wifi.txt"
#define WIFI_CONFIG "/etc/wpa_supplicant/wpa_supplicant.conf"
#define WIFI_CONNECTED  "wpa_state=COMPLETED"


WIFI::WIFI()
{
    //wifi扫描功能timer（偷懒，代替手动创建线程，下同）
    scanBarTimer = new QTimer(this);
    scanBarTimer->setInterval(0);
    scanBarTimer->setSingleShot(true);//触发一次
    connect(scanBarTimer,SIGNAL(timeout()),this,SLOT(slot_scanOver()));
    //wifi 3秒一次监控联网状态，连接的wifi名称
    mWifiMonitor = new QTimer(this);
    mWifiMonitor->setInterval(3000);
    connect(mWifiMonitor,SIGNAL(timeout()),this,SLOT(slot_wifiMonitor()));
    mWifiNameVec.reserve(100);
    init();

}

void WIFI::init()
{
    system(" ifconfig wlan0 up ");
    read_wpaConfig();
    mWifiMonitor->start();
}

WIFI::~WIFI()
{
    delete scanBarTimer;
    delete mWifiMonitor;
}
void WIFI::read_wpaConfig()
{
    std::fstream fileStream;
    string file(WIFI_CONFIG);
    std::string data;

    // 判断文件是否存在
    if(access(file.c_str(), F_OK) != 0) {
        DEBUG_E("%s not exist \n",WIFI_CONFIG);
        return;
    }
    // 打开文件
    fileStream.open(file.c_str(), std::fstream::in);
    if(!fileStream.is_open()) {
        DEBUG_E("%s open failed\n",WIFI_CONFIG);
        return;
    }

    std::vector<std::string> wifiName;
    int wifiNameCount = 0;
    while(!(fileStream.peek() == EOF)){
        std::getline(fileStream, data);
        if(data.find("ssid=\"") != std::string::npos){
            wifiName = std::move(mySplit(data,"\""));
            if(wifiName.size()<2){
                data = "";
                DEBUG_E("name read fail!\n");
                continue;
            }
            mWifiConfig.insert(std::make_pair(wifiNameCount++,wifiName[1]));
            DEBUG_I("wpa_supplicant.conf is : %s \n",mWifiConfig[wifiNameCount - 1].c_str());
        }
        data = "";
    }
    fileStream.close();
}

std::map<int,std::string>&  WIFI::getWifiConfig()
{
    return mWifiConfig;
}
void WIFI::eraseWifiConfig(int num)
{
    std::map<int,std::string>::iterator key = mWifiConfig.find(num);
    if(key != mWifiConfig.end()){
        mWifiConfig.erase(key);
    }

}

void WIFI::slot_scanOver(){

    mWifiNameVec.clear();
    bool result = true;
    std::fstream fileStream;
    string file(SCAN_WIFI_NAME);
    std::string data,send;
    for(;;){
        // 判断文件是否存在
        if(access(file.c_str(), F_OK) != 0) {
            DEBUG_E("%s not exist \n",SCAN_WIFI_NAME);
            result = false;
            break;
        }
        // 打开文件
        fileStream.open(file.c_str(), std::fstream::in);
        if(!fileStream.is_open()) {
            DEBUG_E("%s open failed\n",SCAN_WIFI_NAME);
            result = false;
            break;
        }

        bool first = true;
        while(!(fileStream.peek() == EOF)){
            std::getline(fileStream, data);
            if(first){//配置文件第一行不是我们想要的，所以跳过
                first = false;
                continue;
            }
            size_t pos = data.find_last_of('\t');
            if(pos != std::string::npos){
                send = data.substr(pos+1);
                addWifiNameVec(send);
            }
            data = "";
            send = "";
        }
        fileStream.close();
        break;
    }
    if(!result){
        DEBUG_E("sao miao  wifi.conf shi bai le ");
    }
    emit sig_scanOver(result);
}

void WIFI::slot_wifiMonitor()
{
    FILE *fp = nullptr;
    char buf[256];
    std::string command("wpa_cli -i wlan0 status"),data;
    static std::string oldConnect("");
    bool connected = false;
    //查看网络链接状态
    if((fp = popen(command.c_str(),"r")) != nullptr){
        //C风格的安行读取
        while(fgets(buf, 256, fp) != nullptr){
            data = buf;
            if(data.find("wpa_state=COMPLETED") != std::string::npos ){
                connected = true;
            }
            if(data.find("ssid=") == 0){

                mConnectedWifi = data.substr(5,data.find('\0'));
            }
            memset(buf,'0',256);
        }
        pclose(fp);
        fp = nullptr;
    }
    if(!connected){
        sig_connectStatus("");
        return;
    }
    //每次校验都发信号 为解决有时连接已存在wifi状态不更新问题
    sig_connectStatus(mConnectedWifi);
//    //链接状态改变，触发信号
//    if(mConnectedWifi != oldConnect){
//        sig_connectStatus(mConnectedWifi);
//        oldConnect = mConnectedWifi;
//    }

}

void WIFI::slot_scanWIFI(){

    system(" wpa_cli -iwlan0 scan ");
    system(" wpa_cli -iwlan0 scan_results > /home/debian/Cat/wifi.txt ");
    //此处为了省力，用timer代替了线程
    scanBarTimer->start();

}

void WIFI::addWifiNameVec(const std::string str)
{
    std::lock_guard<std::mutex> lck(mWifiNameVecMtx);
    if(mWifiNameVec.size() >= 100){
        mWifiNameVec.clear();
    }
    mWifiNameVec.emplace_back(str);
}

void WIFI::setConnectingWIFI(std::string str)
{
    mConnectingWifi = str;
}

std::string WIFI::getConnectingWIFI()
{
    return mConnectingWifi;
}
