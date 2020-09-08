#ifndef LOGUPLOAD_H
#define LOGUPLOAD_H

#include <mutex>
#include "../Tool/Thread.h"
#include "../MyDbus/DbusAdapter.h"
#include "HttpRequest.h"
#include "../Debug/Debug.h"

#define LOG_PATH "/tmp/CatLog/Log_0"

class LogUpload : public Thread
{
public:
    static LogUpload* instance();
    void setHttpPath(std::string path);
private:
    LogUpload();
    ~LogUpload();
    LogUpload(const LogUpload&) = delete; // 禁止拷贝
    LogUpload& operator=(const LogUpload&) = delete; // 禁止赋值
    static void creatSingle();
    static void destorySingle();
    void* run(void *arg);
private:
    static LogUpload* single;
    static pthread_once_t ponce_;
    std::string m_httpPath;
};

#endif // LOGUPLOAD_H
