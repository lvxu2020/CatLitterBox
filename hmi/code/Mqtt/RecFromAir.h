#ifndef RECFROMAIR_H
#define RECFROMAIR_H

#include <vector>
#include <queue>
#include "../Base/Thread.h"
#include "../App/NetMonitor/netMonitor.h"
#include "../Mqtt/mqttClinet.h"
#include "../LOG/Debug.h"
#include "../Base/base.h"
#include "../App/Json/JsonAdapter.h"
#include "../App/Dbus/DbusSend.h"
#include <pthread.h>
#include "../App/NetMonitor/netMonitor.h"

class NetMonitor;
#define TASK_Q_MAX 100


class RecFromAir
{
public:
    RecFromAir();
    ~RecFromAir();
    int start();
    int join();
    int detach();
    pthread_t self();
    void* run(void *arg);
    static void* getTask(void *arg);
    bool addTaskQ(std::string str);
    std::string popTaskQ();
    void analysisTask(std::string remote);
    bool threadIsActive();
private:
    pthread_t m_tid;
    int m_running;
    int m_detached;
    bool m_thredLife;
    std::mutex m_mutex;
    friend class Singleton <NetMonitor>;
    std::queue<std::string> task;

};

typedef Singleton<RecFromAir> RecFromAir_Single;    // 方便使用

#endif // RECFROMAIR_H
