#include "SendToAir.h"
#include "./Dbus/DbusSend.h"
#include "./Dbus/DbusAdapter.h"

//线程部分函数
static void* runThread(void* arg) {
    return ((SendToAir*) arg)->run(arg);
}

SendToAir::SendToAir() : m_tid(0), m_running(0), m_detached(0)
{

}

SendToAir::~SendToAir() {
    if (m_running == 1 && m_detached == 0) {
        pthread_detach(m_tid);
    }
    if (m_running == 1) {
        pthread_cancel(m_tid);
    }
}

int SendToAir::start() {
    int result = pthread_create(&m_tid, NULL, runThread, this);
    if (result == 0) {
        m_running = 1;
    }
    return result;
}

int SendToAir::join() {
    int result = -1;
    if (m_running == 1) {
        result = pthread_join(m_tid, NULL);
        if (result == 0) {
            m_running = 0;
        }
    }
    return result;
}

int SendToAir::detach() {
    int result = -1;
    if (m_running == 1 && m_detached == 0) {
        result = pthread_detach(m_tid);
        if (result == 0) {
            m_detached = 1;
        }
    }

    return result;
}

pthread_t SendToAir::self() {
    return pthread_self();
}

bool SendToAir::threadIsActive()
{
    return m_threadLife;
}

void* SendToAir::run(void *arg)
{
    DEBUG_I("SendToAir in");
    m_threadLife = true;
    do{//保证网络状态可用
        mySleep(3000);
    } while (!NetMonitor_Single::instance()->getNetStatus());

    MqttSend send;

    send.setInit(ReadConf_Single::instance()->getAdd(),"1883",60,1);
    if (0 != send.connectMqtt()) {
        DEBUG_E("MQTT_send connect fail\n");
    }else{
        DEBUG_I("connectMqtt succeed\n");
        std::string remote;
        while (NetMonitor_Single::instance()->getNetStatus()) {
            remote = popTaskQ();
            std::cout<<remote<<std::endl;
            if (remote == "") {
                mySleep(1000);
            } else {
                DEBUG_D("%s", remote.c_str());
                send.sendMess("cToS",remote,0,0,100L);
            }
        }

    }
    m_threadLife = false;
    DEBUG_I("SendToAir out");
    return nullptr;
}

bool SendToAir::addTaskQ(std::string str)
{
    std::lock_guard<std::mutex> lck(m_mutex);
    if (task.size() >= TASK_Q_MAX) {
        return false;
    }
    task.push(str);
    return true;
}

std::string SendToAir::popTaskQ( )
{
    std::lock_guard<std::mutex> lck(m_mutex);
    if (task.size() == 0) {
       return "";
    }
    std::string str = task.front();
    task.pop();
    return str;
}

