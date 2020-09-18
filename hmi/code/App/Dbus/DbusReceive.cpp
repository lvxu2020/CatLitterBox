#include "DbusReceive.h"
#include <QDBusMessage>
#include <QtDBus>
#include <iostream>
#include "../Json/JsonAdapter.h"


void DbusReceive::service_get(QString st)
{
    Node tast(st.toStdString());
    std::string str = JsonAdapter::parseNode(tast, "id");
    if (str == "") {
        DEBUG_E("parseNode fail");
        return;
    }

    switch (atoi(str.c_str())) {
    case 0: {// OTA

    }
        break;
    case 1: {// 硬件消息
        std::string send(";");
        send += ReadConf_Single::instance()->getID() + ";" + str;
        std::string str = JsonAdapter::parseNode(tast, "data");
        if (str != "") {
            send += ";" + str + ";";
            DEBUG_D("bus send:%s",send.c_str());
            SendToAir_Single::instance()->addTaskQ(send);
        }
    }
        break;
    case 2: {// Log上传

    }
        break;

    default: break;
    }
}

DbusReceive::DbusReceive()
{
    QDBusConnection::sessionBus().unregisterService("com.hmi.service");
    QDBusConnection::sessionBus().registerService("com.hmi.service");
    QDBusConnection::sessionBus().unregisterObject("/hmi/path");
    QDBusConnection::sessionBus().registerObject("/hmi/path", this, QDBusConnection \
    ::ExportAllSlots | QDBusConnection::ExportAllSignals);
    QDBusConnection::sessionBus().connect(QString(), \
    QString("/hmi/path"), "code.back", "signal", this, SLOT(service_get(QString)));
}

