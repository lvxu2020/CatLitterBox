#ifndef WIFI_H
#define WIFI_H

#include "../../Base/singleton.h"
#include <QObject>
#include <QTimer>
#include "../../Base/Thread.h"
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <mutex>


class WIFI : public QObject
{
    Q_OBJECT
public:
    void init();
    //头文件直接写出函数体（不包含循环），编译器会默认将函数编译成内联函数
    const std::vector<std::string>& getWifiNameVec() const { return mWifiNameVec; };
    void addWifiNameVec(const std::string str);
    void setConnectingWIFI(std::string str);
    std::string getConnectingWIFI();
    std::map<int,std::string>& getWifiConfig();
    void eraseWifiConfig(int);
signals:
    void sig_scanOver(bool);
    void sig_connectOver(bool);
    void sig_connectStatus(std::string);
public slots:
    void slot_scanWIFI();
    void slot_scanOver();
    void slot_wifiMonitor();
private:
    WIFI();
    ~WIFI();
    void read_wpaConfig();

    QTimer* scanBarTimer = nullptr;
    QTimer* mWifiMonitor = nullptr;
    std::vector<std::string> mWifiNameVec;
    std::string mConnectingWifi;
    std::string mConnectedWifi;
    std::map<int,std::string> mWifiConfig;
    std::mutex mWifiNameVecMtx;
    friend class Singleton<WIFI>;
};

typedef Singleton<WIFI> WIFI_Single;    // 方便使用

#endif // WIFI_H
