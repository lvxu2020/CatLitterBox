#include "HardMaster.h"
#include <assert.h>
#include <stdlib.h> // atexit
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include<unistd.h>


#define GPIO23_VALUE_PATH "/sys/class/gpio/gpio23/value"
#define LED_VALUE_PATH "/sys/class/leds/green/brightness"

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

    char lineBuf[10] = {0};
    while (true) {        
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::ifstream fGpio23(GPIO23_VALUE_PATH), fLed(LED_VALUE_PATH);
        memset(lineBuf,'\0',10);
        fGpio23.getline(lineBuf, 10);
        m_led = lineBuf;
        fGpio23.close();

        memset(lineBuf,'\0',10);
        fLed.getline(lineBuf, 10);
        m_gpio23 = lineBuf;
        fLed.close();
    }
    return nullptr;
}

bool HardMaster::isExist(const std::string &pathname)
{
    if (access(pathname.c_str(), F_OK) == 0) {
        return true;
    }

    return false;
}

void HardMaster::gpio23Change()
{
    std::ifstream fGpio23(GPIO23_VALUE_PATH);
    char lineBuf[10] = {0};
    memset(lineBuf,'\0',10);
    fGpio23.getline(lineBuf, 10);
    m_gpio23 = lineBuf;
    fGpio23.close();
}

void HardMaster::ledChange()
{
    std::ifstream led(LED_VALUE_PATH);
    char lineBuf[10] = {0};
    memset(lineBuf,'\0',10);
    led.getline(lineBuf, 10);
    m_led = lineBuf;
    led.close();
}

void HardMaster::openGpio23()
{
    FILE *fp;
    int rc;
    fp = popen("echo 1 > /sys/class/gpio/gpio23/value", "r");
    if (fp == nullptr) {
        printf("echo 1 > /sys/class/gpio/gpio23/value error");
    }
    pclose(fp);
    gpio23Change();
}

void HardMaster::closeGpio23()
{
    system("echo 0 > /sys/class/gpio/gpio23/value");
    gpio23Change();
}

void HardMaster::openLed()
{
    system("echo 255 > /sys/class/leds/green/brightness");
    ledChange();
}

void HardMaster::closeLed()
{
    system("echo 0 > /sys/class/leds/green/brightness");
    ledChange();
}

int HardMaster::getHardStatus()
{
    int num = 0;
    if (m_gpio23 == "0") {
        num &= 0xfe;
    }else {
        num |= 0x1;
    }

    if (m_led == "0") {
        num &= 0xfd;
    }else {
        num |= 0x2;
    }
    return num;
}
