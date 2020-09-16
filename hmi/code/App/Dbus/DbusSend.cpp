#include "DbusSend.h"
#include <QDBusMessage>

DbusSend::DbusSend()
{

}

void DbusSend::sendASiganl(QString path, QString interface, QString signal , QString value)
{
    QDBusMessage message = QDBusMessage::createSignal(path, interface, signal);
    message << value;
    if (QDBusConnection::sessionBus().send(message)) {
        DEBUG_E("chenggong :%s",value.toStdString().c_str());
    } else {
        DEBUG_E("shibai:%s",value.toStdString().c_str());
    }
}
