#include "DbusReceive.h"
#include <assert.h>
#include <stdlib.h> // atexit
#include <unistd.h>
#include <algorithm>
#include<string.h>


DbusReceive* DbusReceive::single = nullptr;
pthread_once_t DbusReceive::ponce_ = PTHREAD_ONCE_INIT;
DBusConnection* DbusReceive::connection = nullptr;
DbusAdapter DbusReceive::m_sendBus;

DbusReceive* DbusReceive::instance()
{
    pthread_once(&DbusReceive::ponce_, &DbusReceive::creatSingle);
    assert(single != NULL); //假设成立，程序继续运行，否则报错终止
    return single;
}

DbusReceive::DbusReceive()
{
    DBusError err;
    dbus_error_init(&err);
//    connection = dbus_bus_get(DBUS_BUS_SESSION, &err);// 每次申请得到相同同句柄
    connection = dbus_bus_get_private(DBUS_BUS_SESSION, &err);// 每次申请得到不同句柄
    if(dbus_error_is_set(&err)){
        fprintf(stderr, "ConnectionErr : %s\n", err.message);
        dbus_error_free(&err);
    }
    if (connection == nullptr) {
        DEBUG("DbusReceive can not connect");
    }
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
    while ( connection == nullptr) {
        sleep(1);
    }
    signalMonitor();
    return nullptr;
}

int DbusReceive::signalMonitor()
{
    DBusError err;
    dbus_error_init(&err);
    if (m_vecInterface.empty() || !dbus_connection_add_filter(connection, msgHandle, NULL, NULL)) {
        return 1;
    }
    for(auto it = m_vecInterface.begin(); it != m_vecInterface.end(); it++){
        std::string listenStr = "type=\'signal\',interface=\'" + *it + "\'";
        DEBUG("%s", listenStr.c_str());
        dbus_bus_add_match(connection, listenStr.c_str(), &err);
    }
    while (dbus_connection_read_write_dispatch(connection, -1)) {
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
            std::string taskStr(str);
            DbusReceive::instance()->processTask(taskStr);
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

void DbusReceive::processTask(std::string& taskStr)
{
    Node tast(taskStr);
    std::string str = JsonAdapter::parseNode(tast, "id");
    if (str == "") {
        DEBUG("parseNode fail");
        return;
    }
    switch (atoi(str.c_str())) {
    case 0: {
        str = JsonAdapter::parseNode(tast, "data");
        if (str == "0") {
            HardMaster::instance()->closeGpio23();
        } else if (str == "1") {
            HardMaster::instance()->openGpio23();
        }
    }
        break;
    case 1: {
        str = JsonAdapter::parseNode(tast, "data");
        if (str == "0") {
            HardMaster::instance()->closeLed();
        } else if (str == "1") {
            HardMaster::instance()->openLed();
        }
    }
        break;
    case 2: {

    }
        break;
    case 3: {
        str = JsonAdapter::parseNode(tast, "data");
        LogUpload::instance()->setHttpPath(str);
        LogUpload::instance()->start();
        LogUpload::instance()->detach();
    }
        break;
    case 4: {
        Node root;
        char num[10];
        sprintf(num, "%d", HardMaster::instance()->getHardStatus());
        JsonAdapter::addValueToNode(root, "id", "1");
        JsonAdapter::addValueToNode(root, "data", num);
        std::string send;
        JsonAdapter::getUnFormatStrFromNode(root, send);
        if (m_sendBus.sendASignal("/hmi/path", "code.back", "signal", send.c_str())) {
            DEBUG("bus send succeed :%s", send.c_str());
        }else {
            DEBUG("bus send fail :%s", send.c_str());
        }
    }
        break;
    default: break;
    }
}








