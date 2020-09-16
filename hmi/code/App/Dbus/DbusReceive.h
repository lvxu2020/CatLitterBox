#ifndef DBUSRECEIVE_H
#define DBUSRECEIVE_H

#include <QObject>
#include <QDebug>
#include "../../LOG/Debug.h"

class DbusReceive : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.hmi")
public:
    DbusReceive();
public slots:
    void service_get(QString st);
};

#endif // DBUSRECEIVE_H
