#ifndef DBUSSEND_H
#define DBUSSEND_H

#include <QObject>
#include <QString>
#include <QtDBus>
#include "../LOG/Debug.h"


class DbusSend
{
public:
    DbusSend();
    static void sendASiganl(QString path, QString interface, QString signal, QString value);
};

#endif // DBUSSEND_H
