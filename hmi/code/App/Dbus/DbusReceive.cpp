#include "DbusReceive.h"
#include <QDBusMessage>
#include <QtDBus>
#include <iostream>
#include "../Json/JsonAdapter.h"

void DbusReceive::service_get(QString st)
{
    std::cout << "Message get from client: "<< st.toStdString()<<std::endl;
    Node tast(st.toStdString());
    std::string str = JsonAdapter::parseNode(tast, "id");
    if (str == "") {
        DEBUG_E("parseNode fail");
        return;
    }
    switch (atoi(str.c_str())) {
    case 0: {

    }
        break;
    case 1: {

    }
        break;
    case 2: {

    }
        break;
    case 3: {

    }

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

