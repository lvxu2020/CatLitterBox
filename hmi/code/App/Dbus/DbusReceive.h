#ifndef DBUSRECEIVE_H
#define DBUSRECEIVE_H

#include <QObject>
#include <QDebug>

class DbusReceive : public QObject
{
    Q_OBJECT
public:
    DbusReceive();
    void service_get(QString st);
};

#endif // DBUSRECEIVE_H
