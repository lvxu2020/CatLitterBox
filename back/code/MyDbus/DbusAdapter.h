#ifndef DBUSADAPTER_H
#define DBUSADAPTER_H

#include <dbus/dbus.h>
#include "../Tool/Thread.h"
#include "../Debug/Debug.h"
#include "../Json/JsonAdapter.h"
#include <string>


class DbusAdapter
{
public:
    DbusAdapter();
    static DBusConnection* sessionBus();
    //给连接名分配一个可记忆名字test.singal.dest作为Bus name，这个步骤不是必须的,但推荐这样处理
    bool setDbusWellKnownName(const std::string& name);
    bool setDbusWellKnownName(const char* name);
    //第一个参数路径名第一个字符必须以 "/" 开头
    static bool sendASignal(const std::string& object, const std::string& interface, const std::string& signal, const std::string& message);
    static bool sendASignal(const char* object, const char* interface, const char* signal, const char* message);
private:
    static DBusConnection* connection;
};

#endif // DBUSADAPTER_H
