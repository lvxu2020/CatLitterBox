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

class SendToAir
{
public:
    SendToAir();
    ~SendToAir();
    int start();
    int join();
    int detach();
    pthread_t self();
    void* run(void *arg);
private:
    pthread_t m_tid;
    int m_running;
    int m_detached;
    std::mutex m_mutex;
    friend class Singleton<SendToAir>;
};

typedef Singleton<SendToAir> SendToAir_Single;    // 方便使用

#endif // SENDTOAIR_H
