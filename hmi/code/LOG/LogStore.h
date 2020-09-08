#pragma once

#include <pthread.h>

class LogStore
{
public:
    LogStore();
    ~LogStore();
    int start();
    int join();
    int detach();
    pthread_t self();

    virtual void* run(void *arg);

private:
    pthread_t m_tid;
    int m_running;
    int m_detached;
};
