#include "DbusReceive.h"
#include <assert.h>
#include <stdlib.h> // atexit
#include <unistd.h>
#include <algorithm>
#include<string.h>


DbusReceive* DbusReceive::single = nullptr;
pthread_once_t DbusReceive::ponce_ = PTHREAD_ONCE_INIT;


DbusReceive* DbusReceive::instance()
{
    pthread_once(&DbusReceive::ponce_, &DbusReceive::creatSingle);
    assert(single != NULL); //假设成立，程序继续运行，否则报错终止
    return single;
}

DbusReceive::DbusReceive()
{

}

DbusReceive::~DbusReceive()
{

}

void DbusReceive::creatSingle()
{
    single = new DbusReceive();
    ::atexit(destorySingle);
}

void DbusReceive::destorySingle()
{
    delete single;
    single = nullptr;
}

void* DbusReceive::run(void *arg)
{
    DEBUG("DbusReceive::run\n");
    while ( DbusAdapter::sessionBus() == nullptr) {
        sleep(1);
    }
    signalMonitor();
}

int DbusReceive::signalMonitor()
{
    DBusError err;
    dbus_error_init(&err);
    if (m_vecInterface.empty() || !dbus_connection_add_filter(DbusAdapter::sessionBus(), msgHandle, NULL, NULL)) {
        return 1;
    }
    for(auto it = m_vecInterface.begin(); it != m_vecInterface.end(); it++){
        std::string listenStr = "type=\'signal\',interface=\'" + *it + "\'";
        DEBUG("%s", listenStr.c_str());
        dbus_bus_add_match(DbusAdapter::sessionBus(), listenStr.c_str(), &err);
    }
    while (dbus_connection_read_write_dispatch(DbusAdapter::sessionBus(), -1)) {
    }
    DEBUG("erro");
    return 0;
}

bool DbusReceive::addListenSig(const std::string& interface, const std::string& signal)
{
    auto it = std::find(m_vecInterface.begin(), m_vecInterface.end(), interface);
    if (it == m_vecInterface.end()) {
        m_vecInterface.push_back(interface);
    }
    ListenSig temp(interface,signal);
    auto sig = std::find(m_vecListenSig.begin(), m_vecListenSig.end(), temp);
    if (sig == m_vecListenSig.end()) {
        m_vecListenSig.push_back(temp);
    }
}

bool DbusReceive::addListenSig(const char* interface_ch, const char* signal_ch)
{
    std::string interface(interface_ch), signal(signal_ch);
    auto it = std::find(m_vecInterface.begin(), m_vecInterface.end(), interface);
    if (it == m_vecInterface.end()) {
        m_vecInterface.push_back(interface);
    }
    ListenSig temp(interface,signal);
    auto sig = std::find(m_vecListenSig.begin(), m_vecListenSig.end(), temp);
    if (sig == m_vecListenSig.end()) {
        m_vecListenSig.push_back(temp);
    }
}
static bool testFlag = true;
char* DbusReceive::testPtr = "hello";
DBusHandlerResult DbusReceive::msgHandle(DBusConnection* connection, DBusMessage* msg, void* usr_data)
{

    DBusError err;
    dbus_error_init(&err);
//    // 获取信号来源路径,不做路径检查。暂时屏蔽
//    std::string msgPath = dbus_message_get_path(msg);
    // 数组类型消息接收
    char* str = nullptr;
    for (auto it = DbusReceive::instance()->getVecListenSig().begin(); it != DbusReceive::instance()->getVecListenSig().end(); it++) {

        if (FALSE == dbus_message_get_args(msg, &err, DBUS_TYPE_STRING, &str, DBUS_TYPE_INVALID))
        {
            DEBUG("fail");
            return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
        }

        if (dbus_message_is_signal(msg, it->m_interface.c_str(), it->m_signal.c_str())) {
            DEBUG("get it:%s",str);
//            dbus_free(str); // 经过测试dbus-1 释放会报错。
            return DBUS_HANDLER_RESULT_HANDLED;
        }
    }
//    // 数组类型消息接收
//    void *ptr;
//    int n_elements = 0;
//    for (auto it = DbusReceive::instance()->getVecListenSig().begin(); it != DbusReceive::instance()->getVecListenSig().end(); it++) {

//        if (FALSE == dbus_message_get_args(msg, &err, DBUS_TYPE_ARRAY, DBUS_TYPE_BYTE, &ptr, &n_elements, DBUS_TYPE_INVALID))
//        {
//            DEBUG("fail");
//            return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
//        }
//        if (n_elements == 0) {
//            return DBUS_HANDLER_RESULT_HANDLED;
//        }

//        if (dbus_message_is_signal(msg, it->m_interface.c_str(), it->m_signal.c_str())) {
//            DEBUG("get it");
//            return DBUS_HANDLER_RESULT_HANDLED;
//        }
//    }

}

std::vector<ListenSig>& DbusReceive::getVecListenSig()
{
    return m_vecListenSig;
}








