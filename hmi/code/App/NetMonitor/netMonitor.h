#ifndef NETMONITOR_H
#define NETMONITOR_H

#include "../../Base/singleton.h"
#include <QObject>
#include <QTimer>
#include <string>
#include "../../Mqtt/mqttSend.h"

#define BAIDU_IP "202.108.22.5"

class NetMonitor : public QObject
{
    Q_OBJECT
public:
    NetMonitor();
    bool getNetStatus();
    bool getNetConnection();
    int ping(std::string);
signals:
    void monitorTimerStart();
    void monitorTimerStop();
    void sig_netSatusChange(bool);
private slots:
    void slot_monitor();
    void slot_monitorTimerStart();
    void slot_monitorTimerStop();
private:
    bool netIsOK = false;
    QTimer* monitorPtr = nullptr;
    friend class Singleton<NetMonitor>;
};

typedef Singleton<NetMonitor> NetMonitor_Single;    // 方便使用

#endif // NETMONITOR_H
