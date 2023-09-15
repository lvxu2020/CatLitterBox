#include "RecFromAir.h"
#include "ReadConf.h"
#include <unistd.h>
#include "./Dbus/DbusReceive.h"

//线程部分函数
static void* runThread(void* arg) {
    return ((RecFromAir*) arg)->run(arg);
}

RecFromAir::RecFromAir() : m_tid(0), m_running(0), m_detached(0)
{
    MqttClinet::getIntence()->setInit(ReadConf_Single::instance()->getAdd(),"1883",60,1);
}

RecFromAir::~RecFromAir() {
    if (m_running == 1 && m_detached == 0) {
        pthread_detach(m_tid);
    }
    if (m_running == 1) {
        pthread_cancel(m_tid);
    }
}

int RecFromAir::start() {
    int result = pthread_create(&m_tid, NULL, runThread, this);
    if (result == 0) {
        m_running = 1;
    }
    return result;
}

int RecFromAir::join() {
    int result = -1;
    if (m_running == 1) {
        result = pthread_join(m_tid, NULL);
        if (result == 0) {
            m_running = 0;
        }
    }
    return result;
}

int RecFromAir::detach() {
    int result = -1;
    if (m_running == 1 && m_detached == 0) {
        result = pthread_detach(m_tid);
        if (result == 0) {
            m_detached = 1;
        }
    }

    return result;
}

pthread_t RecFromAir::self() {
    return pthread_self();
}

void* RecFromAir::run(void *arg)
{
    DEBUG_I("RecFromAir in");
    m_thredLife = true;
    do {//保证网络状态可用
        mySleep(2000);
    } while (!NetMonitor_Single::instance()->getNetStatus());
/*    // 此线程断网重连，先断开再连接。
    MqttClinet::getIntence()->unConnectMqtt()*/;
    if (0 != MqttClinet::getIntence()->connectMqtt()) {
        DEBUG_E("MQTT_rec connect fail\n");
        return NULL;
    }
    int qos = atoi(ReadConf_Single::instance()->getQos1().c_str());
    // 此线程断网重连，先取消再订阅。
    MqttClinet::getIntence()->unSubscribe(ReadConf_Single::instance()->getTopic1());
    MqttClinet::getIntence()->subscribe(ReadConf_Single::instance()->getTopic1(), qos
    , getTask);

    std::string remote;
    while (NetMonitor_Single::instance()->getNetStatus()) {
        remote = popTaskQ();
        std::cout<<remote<<std::endl;
        if (remote == "") {
            mySleep(1000);
        } else {
            analysisTask(remote);
        }

    }
    m_thredLife = false;
    return NULL;
}

void RecFromAir::analysisTask(std::string remote)
{
    DEBUG_I("analysisTask in");
    size_t pos = remote.find_first_of(';');
    if (pos == std::string::npos) {
        return;
    }
    std::vector<std::string> vec = std::move(mySplit(remote,";"));
    DEBUG_D("the vec size is %d !, [2]:%s [3]:%s", vec.size(), vec.at(2).c_str(), vec.at(3).c_str());
    if (vec.size() != 4 || vec.at(1) != ReadConf_Single::instance()->getID()) {
        return;
    }else {
        Node root;
        switch (atoi(vec.at(2).c_str())) {
        case 0: {
            JsonAdapter::addValueToNode(root, "id", "0");
            JsonAdapter::addValueToNode(root, "data", vec.at(3).c_str());
        }
            break;
        case 1:{
            JsonAdapter::addValueToNode(root, "id", "1");
            JsonAdapter::addValueToNode(root, "data", vec.at(3).c_str());
        }
            break;
        case 2:{
            JsonAdapter::addValueToNode(root, "id", "2");
            JsonAdapter::addValueToNode(root, "data", vec.at(3).c_str());
        }
            break;
        case 3:{
            JsonAdapter::addValueToNode(root, "id", "3");
            JsonAdapter::addValueToNode(root, "data", vec.at(3).c_str());
        }
            break;
        case 4:{
            JsonAdapter::addValueToNode(root, "id", "4");
            JsonAdapter::addValueToNode(root, "data", vec.at(3).c_str());
        }
            break;
        case 5:{
            ReadConf_Single::instance()->setAirVerson(vec.at(3));
            ReadConf_Single::instance()->onAirVersonChange();
            goto not_send_back;
        }
            break;
        default:break;
        }
        std::string strToBack;
        JsonAdapter::getUnFormatStrFromNode(root,strToBack);
        if (strToBack != "") {
            DbusSend::sendASiganl("/", "code.hmi", "signal", strToBack.c_str());
        }
    }
not_send_back:
    return;
}

bool RecFromAir::threadIsActive()
{
    return m_thredLife;
}

void* RecFromAir::getTask(void *arg)
{
    MqttMess *p = (MqttMess*)arg;
    if (p->len <=0 && p->len >= 2048) {
        return NULL;
    }
    char buf[p->len];
    memcpy(buf,(char*)p->p,p->len);
    std::string push(buf);
    DEBUG_D("getTask : %s\n",push.c_str());
    RecFromAir_Single::instance()->addTaskQ(push);
    //不写返回值，系统各种崩溃
    return NULL;
}

bool RecFromAir::addTaskQ(std::string str)
{
    std::lock_guard<std::mutex> lck(m_mutex);
    if (task.size() >= TASK_Q_MAX) {
        return false;
    }
    task.push(str);
    return true;
}

std::string RecFromAir::popTaskQ( )
{
    std::lock_guard<std::mutex> lck(m_mutex);
    if (task.size() == 0) {
       return "";
    }
    std::string str = task.front();
    task.pop();
    return str;
}








