#ifndef OTA_H
#define OTA_H

#include <mutex>
#include "../Tool/Thread.h"
#include "../MyDbus/DbusAdapter.h"
#include "HttpRequest.h"

#define DOWNLOAD_PATH_1 ""
#define DOWNLOAD_PATH_2 ""

class Ota : public Thread
{
public:
    static Ota* instance();
    void setHttpPath(std::string path);
private:
    Ota();
    ~Ota();
    Ota(const Ota&) = delete; // 禁止拷贝
    Ota& operator=(const Ota&) = delete; // 禁止赋值
    static void creatSingle();
    static void destorySingle();
    void* run(void *arg);
private:
    static Ota* single;
    static pthread_once_t ponce_;
    std::string m_httpPath;
};

#endif // OTA_H
