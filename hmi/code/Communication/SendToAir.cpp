#include "SendToAir.h"

//线程部分函数
static void* runThread(void* arg) {
    return ((SendToAir*) arg)->run(arg);
}

SendToAir::SendToAir() : m_tid(0), m_running(0), m_detached(0)
{

}

SendToAir::~SendToAir() {
    if (m_running == 1 && m_detached == 0) {
        pthread_detach(m_tid);
    }
    if (m_running == 1) {
        pthread_cancel(m_tid);
    }
}

int SendToAir::start() {
    int result = pthread_create(&m_tid, NULL, runThread, this);
    if (result == 0) {
        m_running = 1;
    }
    return result;
}

int SendToAir::join() {
    int result = -1;
    if (m_running == 1) {
        result = pthread_join(m_tid, NULL);
        if (result == 0) {
            m_running = 0;
        }
    }
    return result;
}

int SendToAir::detach() {
    int result = -1;
    if (m_running == 1 && m_detached == 0) {
        result = pthread_detach(m_tid);
        if (result == 0) {
            m_detached = 1;
        }
    }

    return result;
}

pthread_t SendToAir::self() {
    return pthread_self();
}

void* SendToAir::run(void *arg)
{
    do{//保证网络状态可用
        mySleep(3000);
    } while (!NetMonitor_Single::instance()->getNetStatus());
    MqttSend test;
    test.setInit(ReadConf_Single::instance()->getAdd(),"1883",60,1);

    if (0 != test.connectMqtt()) {
        DEBUG_E("lian jie  shi bai tie zi\n");
    }else{
        DEBUG_I("connectMqtt chenggong\n");
        while (1) {
            char timeStr[100] = {0};
            auto timeNow = std::chrono::system_clock::now();
            auto microtime = std::chrono::duration_cast<std::chrono::microseconds>(timeNow.time_since_epoch()).count();
            int msec = (microtime / 1000) % 1000;

            auto tt = std::chrono::system_clock::to_time_t(timeNow);
            struct tm* ptm = localtime(&tt);

            sprintf(timeStr, "cTos:1;0;[%d-%02d-%02d %02d:%02d:%02d:%03d %s;]",
                    ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, msec, std::to_string(microtime).c_str());
            DEBUG_I("%s",timeStr);
            test.sendMess("cToS",timeStr,0,0,100L);
            mySleep(2000);
        }

    }

}
