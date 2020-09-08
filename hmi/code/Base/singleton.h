#ifndef SINGLETONTEMPLATE_H
#define SINGLETONTEMPLATE_H

#include <assert.h>
#include <pthread.h>
#include <stdlib.h> // atexit

template<typename T>

class Singleton {
public:
    static T* instance() {
        pthread_once(&ponce_, &Singleton::init);
        assert(value_ != NULL);
        return value_;
    }

private:
    static void init() {
        value_ = new T();
        ::atexit(destroy);
    }

    static void destroy() {
        delete value_;
        value_ = NULL;
    }

private:
    Singleton() = delete;
    ~Singleton() = delete;
    Singleton(const Singleton&) = delete; // 禁止拷贝
    Singleton& operator=(const Singleton&) = delete; //禁止赋值

private:
    static pthread_once_t ponce_;
    static T*             value_;
};

template<typename T>
pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::value_ = NULL;

#endif // SINGLETON_H
