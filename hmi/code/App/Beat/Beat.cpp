#include "Beat.h"
#include <assert.h>
#include <stdlib.h> // atexit
#include <unistd.h>
#include <thread>


Beat* Beat::single = nullptr;
pthread_once_t Beat::ponce_ = PTHREAD_ONCE_INIT;

Beat* Beat::instance()
{
    pthread_once(&Beat::ponce_, &Beat::creatSingle);
    assert(single != NULL); //假设成立，程序继续运行，否则报错终止
    return single;
}

void Beat::setRecBeat(bool val)
{
    std::lock_guard<std::mutex> lck(m_mtx);
    m_recBeat = val;
}

Beat::Beat()
{
    m_recBeat = false;
}

Beat::~Beat()
{

}

void Beat::creatSingle()
{
    single = new Beat();
    ::atexit(destorySingle);
}

void Beat::destorySingle()
{
    delete single;
    single = nullptr;
}

void* Beat::run(void *arg)
{
    return nullptr;
    Node root;
    JsonAdapter::addValueToNode(root, "id", "0");
    std::string send;
    JsonAdapter::getUnFormatStrFromNode(root, send);
    root.~Node();
    while (true) {
        DbusSend::sendASiganl("/", "code.hmi", "signal", send.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
    return nullptr;
}
