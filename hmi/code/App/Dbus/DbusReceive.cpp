#include "DbusReceive.h"
#include <QDBusMessage>
#include <QtDBus>

void DbusReceive::service_get(QString st)
{
    qDebug() << "Message get from client: "<< st;
}

DbusReceive::DbusReceive()
{
    QDBusConnection::sessionBus().unregisterService("com.citos.service");
    QDBusConnection::sessionBus().registerService("com.citos.service");
    QDBusConnection::sessionBus().registerObject("/citos/path", this,QDBusConnection :: ExportAllSlots | QDBusConnection :: ExportAllSignals);
    QDBusConnection::sessionBus().connect(QString(), QString("/citos/path"), "code.hmi", "signal", this, SLOT(service_get(QString)));

}
