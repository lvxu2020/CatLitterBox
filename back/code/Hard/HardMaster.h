#ifndef HARDMASTER_H
#define HARDMASTER_H


#include <mutex>
#include "../Tool/Thread.h"
#include "../MyDbus/DbusAdapter.h"
#include "../Debug/Debug.h"

class HardMaster : public Thread
{
public:
    static HardMaster* instance();
    void processTask(int id);
    void openGpio23();
    void closeGpio23();
    void openLed();
    void closeLed();
    int getHardStatus();
private:
    HardMaster();
    ~HardMaster();
    HardMaster(const HardMaster&) = delete; // 禁止拷贝
    HardMaster& operator=(const HardMaster&) = delete; // 禁止赋值
    static void creatSingle();
    static void destorySingle();
    void* run(void *arg);
    bool isExist(const std::string &pathname);
private:
    static HardMaster* single;
    static pthread_once_t ponce_;
    std::string m_gpio23;
    std::string m_led;
};

#endif // HARDMASTER_H
