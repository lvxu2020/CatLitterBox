#ifndef BEAT_H
#define BEAT_H

#include "../Tool/Thread.h"
#include "../MyDbus/DbusAdapter.h"
#include "../Json/JsonAdapter.h"
#include "../Debug/Debug.h"
#include <mutex>


class Beat : public Thread
{
public:
    static Beat* instance();
    void setRecBeat(bool val);
private:
    Beat();
    ~Beat();
    Beat(const Beat&) = delete; // 禁止拷贝
    Beat& operator=(const Beat&) = delete; // 禁止赋值
    static void creatSingle();
    static void destorySingle();
    void* run(void *arg);
    static Beat* single;
    static pthread_once_t ponce_;
    bool m_recBeat;
    std::mutex m_mtx;
};

#endif // BEAT_H
