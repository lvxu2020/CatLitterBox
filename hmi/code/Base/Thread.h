/*
 Thread.h

 Header for a Java style thread class in C++.
 */

#ifndef __thread_h__
#define __thread_h__

#include <pthread.h>

class Thread {
public:
    Thread();
    virtual ~Thread();

    int start();
    int join();
    int detach();
    pthread_t self();

    virtual void* run(void *arg) = 0;

private:
    pthread_t m_tid;
    int m_running;
    int m_detached;
};

#endif
