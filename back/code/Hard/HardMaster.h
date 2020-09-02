#ifndef HARDMASTER_H
#define HARDMASTER_H


#include <mutex>
#include "../Tool/Thread.h"
#include "../MyDbus/DbusAdapter.h"

class HardMaster : public Thread
{
public:
    static HardMaster* instance();
    void processTask(int id);
private:
    HardMaster();
    ~HardMaster();
    HardMaster(const HardMaster&) = delete; // 禁止拷贝
    HardMaster& operator=(const HardMaster&) = delete; // 禁止赋值
    static void creatSingle();
    static void destorySingle();
    void* run(void *arg);
    void openGpio23();
    void closeGpio23();
    static HardMaster* single;
    static pthread_once_t ponce_;
};

#endif // HARDMASTER_H
