#ifndef SENDTOAIR_H
#define SENDTOAIR_H

#include <vector>
#include <queue>
#include "../Base/Thread.h"
#include "../App/NetMonitor/netMonitor.h"
#include "../Mqtt/mqttClinet.h"
#include "../LOG/Debug.h"
#include "../Base/base.h"
#include <pthread.h>
#include "../App/NetMonitor/netMonitor.h"
#include "../Mqtt/mqttSend.h"
#include "ReadConf.h"


#define TASK_Q_MAX 100

class SendToAir
{
public:
    SendToAir();
    ~SendToAir();
    int start();
    int join();
    int detach();
    pthread_t self();
    bool addTaskQ(std::string str);
    std::string popTaskQ();
    bool threadIsActive();
    void* run(void *arg);
private:
    pthread_t m_tid;
    int m_running;
    int m_detached;
    std::mutex m_mutex;
    bool m_threadLife;
    std::queue<std::string> task;
    friend class Singleton<SendToAir>;
};

typedef Singleton<SendToAir> SendToAir_Single;    // 方便使用

#endif // SENDTOAIR_H
