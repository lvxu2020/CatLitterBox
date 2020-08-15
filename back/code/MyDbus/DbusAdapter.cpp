#include "DbusAdapter.h"

DBusConnection* DbusAdapter::connection = nullptr;

DbusAdapter::DbusAdapter()
{
    if (connection == nullptr) {
        sessionBus();
    }
}

DBusConnection* DbusAdapter::sessionBus()
{
    if (connection != nullptr) {
        return connection;
    }
    DBusError err;
    dbus_error_init(&err);
    connection = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if(dbus_error_is_set(&err)){
        fprintf(stderr, "ConnectionErr : %s\n", err.message);
        dbus_error_free(&err);
    }
    return connection;
}

bool DbusAdapter::setDbusWellKnownName(const std::string& name)
{
    DBusError err;
    int ret = dbus_bus_request_name(connection, name.c_str(), DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "Name Err :%s\n", err.message);
        dbus_error_free(&err);
    }
    return (ret == DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) ? true : false;
}

bool DbusAdapter::setDbusWellKnownName(const char* name)
{
    DBusError err;
    int ret = dbus_bus_request_name(connection, name, DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "Name Err :%s\n", err.message);
        dbus_error_free(&err);
    }
    return (ret == DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER)? true: false;
}

bool DbusAdapter::sendASignal(const std::string& object, const std::string& interface, const std::string& signal, const std::string& message)
{
    if (connection == nullptr) {
        return false;
    }
    DBusMessage * msg;
    DBusMessageIter arg;
    //设置 对象，接口，信号
    if ((msg =dbus_message_new_signal(object.c_str(), interface.c_str(), signal.c_str()))== NULL) {
        fprintf(stderr,"MessageNULL\n");
        return false;
    }
    //设置要发送内容
    dbus_message_iter_init_append(msg, &arg);
    dbus_uint32_t  serial = 0;
    const char* sigValue = message.c_str();
    if (!dbus_message_iter_append_basic(&arg,DBUS_TYPE_STRING, &sigValue)) {
        fprintf(stderr,"Out OfMemory!\n");
        return false;
    }
    //发送内容
    if ( !dbus_connection_send(connection, msg, &serial)) {
        fprintf(stderr,"Out of Memory!\n");
        return false;
    }
    dbus_connection_flush(connection);
    dbus_message_unref(msg);
    return true;
}

bool DbusAdapter::sendASignal(const char* object, const char* interface, const char* signal, const char* message)
{
    if (connection == nullptr) {
        return false;
    }
    DBusMessage * msg;
    DBusMessageIter arg;
    //设置 对象，接口，信号
    if ((msg =dbus_message_new_signal(object, interface, signal))== NULL) {
        fprintf(stderr,"MessageNULL\n");
        return false;
    }
    //设置要发送内容
    dbus_message_iter_init_append(msg, &arg);
    dbus_uint32_t  serial = 0;
    if (!dbus_message_iter_append_basic(&arg,DBUS_TYPE_STRING, &message)) {
        fprintf(stderr,"Out OfMemory!\n");
        return false;
    }
    //发送内容
    if ( !dbus_connection_send(connection, msg, &serial)) {
        fprintf(stderr,"Out of Memory!\n");
        return false;
    }
    dbus_connection_flush(connection);
    dbus_message_unref(msg);
    return true;
}






