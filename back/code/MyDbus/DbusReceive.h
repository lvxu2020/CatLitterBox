#ifndef DBUSRECEIVE_H
#define DBUSRECEIVE_H

#include "DbusAdapter.h"
#include "../Tool/Thread.h"
#include "../Debug/Debug.h"
#include "../Json/JsonAdapter.h"
#include "../Beat/Beat.h"
#include "../Hard/HardMaster.h"
#include "../Ota/Ota.h"
#include "../Ota/LogUpload.h"
#include <vector>
#include <string>

class ListenSig{
public:
    ListenSig(std::string interface, std::string signal):m_interface(interface), m_signal(signal) {};
    ListenSig(char* interface, char* signal):m_interface(interface), m_signal(signal) {};
    // 因为接口共用  所以先比较signal，如不同 && 后面不用执行了。提高了效率
    bool operator==(const ListenSig& R) {return m_signal == R.m_signal && m_interface == R.m_interface;};
    std::string m_interface;
    std::string m_signal;
};

class DbusReceive : public Thread
{

public:
    static DbusReceive* instance();
    bool addListenSig(const std::string& interface, const std::string& signal);
    bool addListenSig(const char* interface_ch, const char* signal_ch);
    static char* testPtr;
private:
    DbusReceive();
    ~DbusReceive();
    DbusReceive(const DbusReceive&) = delete; // 禁止拷贝
    DbusReceive& operator=(const DbusReceive&) = delete; // 禁止赋值
    static void creatSingle();
    static void destorySingle();
    void* run(void *arg);
    int signalMonitor();
    static DBusHandlerResult msgHandle(DBusConnection* connection, DBusMessage* msg, void* usr_data);
    std::vector<ListenSig>& getVecListenSig();
    void processTask(std::string& taskStr);
private:
    static DbusReceive* single;
    static pthread_once_t ponce_;
    static DBusConnection* connection;
    static DbusAdapter m_sendBus;
    std::vector<std::string> m_vecInterface;
    std::vector<ListenSig> m_vecListenSig;

};

#endif // DBUSRECEIVE_H
