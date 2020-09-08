/*
 Thread.cpp

 Definition of a Java style thread class in C++.

 在任何一个时间点上，线程是可结合的（joinable）或者是分离的（detached）。
 一个可结合的线程能够被其他线程收回其资源和杀死。在被其他线程回收之前，它的存储器资源（例如栈）是不释放的。
 相反，一个分离的线程是不能被其他线程回收或杀死的，它的存储器资源在它终止时由系统自动释放。
 默认情况下，线程被创建成可结合的。
 为了避免存储器泄漏，每个可结合线程都应该要么被显示地回收，即调用pthread_join，要么通过调用pthread_detach函数被分离。
 */

#include "Thread.h"

static void* runThread(void* arg) {
    return ((Thread*) arg)->run(arg);
}

Thread::Thread() :
    m_tid(0), m_running(0), m_detached(0) {
}

Thread::~Thread() {
    if (m_running == 1 && m_detached == 0) {
        pthread_detach(m_tid);
    }
    if (m_running == 1) {
        pthread_cancel(m_tid);
    }
}

int Thread::start() {
    int result = pthread_create(&m_tid, NULL, runThread, this);
    if (result == 0) {
        m_running = 1;
    }
    return result;
}

int Thread::join() {
    int result = -1;
    if (m_running == 1) {
        result = pthread_join(m_tid, NULL);
        if (result == 0) {
            m_running = 0;
        }
    }
    return result;
}

int Thread::detach() {
    int result = -1;
    if (m_running == 1 && m_detached == 0) {
        result = pthread_detach(m_tid);
        if (result == 0) {
            m_detached = 1;
        }
    }
    return result;
}

pthread_t Thread::self() {
    return pthread_self();
}
