#include "LogStore.h"

#include <chrono>
#include <thread>
#include "LogRecoder.h"

#include "Debug.h"



//线程部分函数

static void* runThread(void* arg) {
    return ((LogStore*) arg)->run(arg);
}

LogStore::LogStore() :
    m_tid(0), m_running(0), m_detached(0) {
}

LogStore::~LogStore() {
    if (m_running == 1 && m_detached == 0) {
        pthread_detach(m_tid);
    }
    if (m_running == 1) {
        pthread_cancel(m_tid);
    }
}

int LogStore::start() {
    int result = pthread_create(&m_tid, NULL, runThread, this);
    if (result == 0) {
        m_running = 1;
    }
    return result;
}

int LogStore::join() {
    int result = -1;
    if (m_running == 1) {
        result = pthread_join(m_tid, NULL);
        if (result == 0) {
            m_running = 0;
        }
    }
    return result;
}

int LogStore::detach() {
    int result = -1;
    if (m_running == 1 && m_detached == 0) {
        result = pthread_detach(m_tid);
        if (result == 0) {
            m_detached = 1;
        }
    }
    return result;
}

pthread_t LogStore::self() {
    return pthread_self();
}

//自定义的run函数

void *LogStore::run(void *arg)
{
    // 初始化log存储文件夹
    while (1) {
        if (!LogRecoder::instance().initLogDir()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            if (LogRecoder::instance().initLogDir()) {
                break;
            }
        }
    }

    // 获取当前文件 用于保存log数据
    std::string currentLogFile = LogRecoder::instance().getCurrentLogFile();

    while (1) {

        // 判断文件是否存在
        if (!LogRecoder::instance().isExist(currentLogFile)) {
            if (!LogRecoder::instance().initLogDir()) {
                continue;
            }

            currentLogFile = LogRecoder::instance().getCurrentLogFile();

            // 判断文件是否存在
            if (!LogRecoder::instance().isExist(currentLogFile)) {
                continue;
            }
        }

        // 判断当前文件大小是否大于FILE_SIZE_MAXIMUM，是 则重新创建文件
        if (LogRecoder::instance().getFileSize(currentLogFile) > LOG_FILE_SIZE_MAXIMUM) {
            currentLogFile = LogRecoder::instance().createLogFile();
            if (!LogRecoder::instance().isExist(currentLogFile)) {
                continue;
            }
        }

        // 将log数据写入文件
        LogRecoder::instance().writeToFile(currentLogFile);

        // 阻塞延时2s
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }

    return nullptr;
}
