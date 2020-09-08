#include "RecFromAir.h"
#include "ReadConf.h"
#include <unistd.h>

//线程部分函数
static void* runThread(void* arg) {
    return ((RecFromAir*) arg)->run(arg);
}

RecFromAir::RecFromAir() : m_tid(0), m_running(0), m_detached(0)
{

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
    system("echo 23 > /sys/class/gpio/export");
    do{//保证网络状态可用
        mySleep(3000);
    } while (!NetMonitor_Single::instance()->getNetStatus());
    system("echo out  > /sys/class/gpio/gpio23/direction");
    MqttClinet::getIntence()->setInit(ReadConf_Single::instance()->getAdd(),"1883",60,1);
    if (0 != MqttClinet::getIntence()->connectMqtt()) {
        DEBUG_E("MQTT lian jie shi bai\n");
        return NULL;
    }
    int qos = atoi(ReadConf_Single::instance()->getQos1().c_str());
    MqttClinet::getIntence()->subscribe(ReadConf_Single::instance()->getTopic1(), qos
    ,getTask);
    std::string remote;
    while (true) {
        remote = popTaskQ();
        std::cout<<remote<<std::endl;
        if (remote == "") {
            mySleep(1000);
        } else {
            analysisTask(remote);
        }

    }
    return NULL;
}

void RecFromAir::analysisTask(std::string remote)
{
    DEBUG_E("analysisTask in");
    size_t pos = remote.find_first_of(';');
    if (pos == std::string::npos) {
        return;
    }
    std::vector<std::string> vec = std::move(mySplit(remote,";"));
    if (vec.front() != ReadConf_Single::instance()->getID()) {
        return;
    }else {
        vec.erase(vec.begin());
        if (vec.front() == "11") {
            startChanshi();
        }
    }
}

void RecFromAir::startChanshi()
{
    system("echo 1 > /sys/class/gpio/gpio23/value");
    system("touch /home/debian/shoudao.c");
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
    printf("getTask : %s\n",push.c_str());
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








