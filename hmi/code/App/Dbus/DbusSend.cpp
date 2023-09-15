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
        DEBUG_D("sendASiganl succeed :%s",value.toStdString().c_str());
    } else {
        DEBUG_D("sendASiganl fail :%s",value.toStdString().c_str());
    }
}
