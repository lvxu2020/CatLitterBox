#include "DbusAdapter.h"
#include <string.h>


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
//    connection = dbus_bus_get(DBUS_BUS_SESSION, &err);// 每次申请得到相同同句柄
    connection = dbus_bus_get_private(DBUS_BUS_SESSION, &err);// 每次申请得到不同句柄
    if(dbus_error_is_set(&err)){
        fprintf(stderr, "ConnectionErr : %s\n", err.message);
        dbus_error_free(&err);
    }
    return connection;
}

bool DbusAdapter::setDbusWellKnownName(const std::string& name)
{
    DBusError err;
    dbus_error_init(&err);
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
    dbus_error_init(&err);
    int ret = dbus_bus_request_name(connection, name, DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "Name Err :%s\n", err.message);
        dbus_error_free(&err);
    }
    return (ret == DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER)? true: false;
}

bool DbusAdapter::sendASignal(const std::string& object, const std::string& interface, const std::string& signal, const std::string& message)
{
    if (connection == nullptr || object[0] != '/') {
        return false;
    }
    DBusMessage * msg;
    DBusMessageIter arg;
    //设置 对象，接口，信号
    if ((msg =dbus_message_new_signal(object.c_str(), interface.c_str(), signal.c_str()))== NULL) {
        fprintf(stderr,"MessageNULL\n");
        return false;
    }
    //设置要发送内容,   字符串形式
    dbus_message_iter_init_append(msg, &arg);
    dbus_uint32_t  serial = 0;
    const char* sigValue = message.c_str();
    if (!dbus_message_iter_append_basic(&arg,DBUS_TYPE_STRING, &sigValue)) {
        fprintf(stderr,"Out OfMemory!\n");
        return false;
    }
//    // 设置发送内容，   数组形式
//    if (message.size()) {
//        char strArg[message.size() + 1];
//        char* arry;
//        int len = message.size() + 1; //给 '\0' 留一位
//        memset(strArg,'/0',len);
//        memcpy(strArg,message.c_str(),len);
//        if(!dbus_message_append_args(msg,DBUS_TYPE_ARRAY,DBUS_TYPE_BYTE,
//                                     &arry, //数组地址的地址
//                                     len,//数组元素个数
//                                     DBUS_TYPE_INVALID))
//        {
//            fprintf(stderr,"MessageNULL\n");
//        }
//    }
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
    if (connection == nullptr || object[0] != '/') {
        return false;
    }
    DBusMessage * msg;
    DBusMessageIter arg;
    //设置 对象，接口，信号
    if ((msg =dbus_message_new_signal(object, interface, signal))== NULL) {
        fprintf(stderr,"MessageNULL\n");
        return false;
    }
    //设置要发送内容,   字符串形式
    dbus_message_iter_init_append(msg, &arg);
    dbus_uint32_t  serial = 0;
    if (!dbus_message_iter_append_basic(&arg,DBUS_TYPE_STRING, &message)) {
        fprintf(stderr,"Out OfMemory!\n");
        return false;
    }   
//    // 设置发送内容，   数组形式
//    if (message.size()) {
//        char strArg[message.size() + 1];
//        char* arry;
//        int len = message.size() + 1; //给 '\0' 留一位
//        memset(strArg,'/0',len);
//        memcpy(strArg,message.c_str(),len);
//        if(!dbus_message_append_args(msg,DBUS_TYPE_ARRAY,DBUS_TYPE_BYTE,
//                                     &arry, //数组地址的地址
//                                     len,//数组元素个数
//                                     DBUS_TYPE_INVALID))
//        {
//            fprintf(stderr,"MessageNULL\n");
//        }
//    }
    //发送内容
    if ( !dbus_connection_send(connection, msg, &serial)) {
        fprintf(stderr,"Out of Memory!\n");
        return false;
    }
    dbus_connection_flush(connection);
    dbus_message_unref(msg);
    return true;
}





