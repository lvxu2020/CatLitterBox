#ifndef DBUSADAPTER_H
#define DBUSADAPTER_H

#include <dbus/dbus.h>
#include "../Tool/Thread.h"
#include "../Debug/Debug.h"
#include <string>


class DbusAdapter
{
public:
    DbusAdapter();
    DBusConnection* sessionBus();
    bool setDbusWellKnownName(const std::string&);
    bool setDbusWellKnownName(const char*);
    bool sendASignal(const std::string&, const std::string&, const std::string&, const std::string&);
    bool sendASignal(const char*, const char*, const char*, const char*);
private:
    static DBusConnection* connection;
};

#endif // DBUSADAPTER_H
