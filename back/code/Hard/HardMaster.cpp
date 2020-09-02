#include "HardMaster.h"
#include <assert.h>
#include <stdlib.h> // atexit
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

HardMaster* HardMaster::single = nullptr;
pthread_once_t HardMaster::ponce_ = PTHREAD_ONCE_INIT;

HardMaster* HardMaster::instance()
{
    pthread_once(&HardMaster::ponce_, &HardMaster::creatSingle);
    assert(single != NULL); //假设成立，程序继续运行，否则报错终止
    return single;
}

void HardMaster::processTask(int id)
{
    switch (id) {
    case 0: {
        openGpio23();
    }
        break;
    case 1: {
        closeGpio23();
    }
        break;
    default: break;
    }
}

HardMaster::HardMaster()
{
    // 板子重启之后得重新配置引脚
    system("sudo echo 23 > /sys/class/gpio/export");
    system("sudo echo out  > /sys/class/gpio/gpio23/direction");
    FILE *fp;
    int rc;
    fp = popen("sudo echo 23 > /sys/class/gpio/export", "r");
    if (fp == nullptr) {
        printf("sudo echo 23 > /sys/class/gpio/export error");
    }
    fp = popen("sudo echo out  > /sys/class/gpio/gpio23/direction", "r");
    if (fp == nullptr) {
        printf("sudo echo out  > /sys/class/gpio/gpio23/direction");
    }
    pclose(fp);
}

HardMaster::~HardMaster()
{

}

void HardMaster::creatSingle()
{
    single = new HardMaster();
    ::atexit(destorySingle);
}

void HardMaster::destorySingle()
{
    delete single;
    single = nullptr;
}

void* HardMaster::run(void *arg)
{
    while (true) {
        sleep(5);
    }
    return nullptr;
}

void HardMaster::openGpio23()
{
//    system("sudo echo 1 > /sys/class/gpio/gpio23/value");
//    FILE *fp;
//    int rc;
//    fp = popen("sudo echo 1 > /sys/class/gpio/gpio23/value", "r");
//    if (fp == nullptr) {
//        printf("sudo echo 1 > /sys/class/gpio/gpio23/value error");
//    }
//    pclose(fp);
}

void HardMaster::closeGpio23()
{
    system("sudo echo 0 > /sys/class/gpio/gpio23/value");
}
