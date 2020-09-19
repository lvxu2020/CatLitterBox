#ifndef READCONF_H
#define READCONF_H

#include <iostream>
#include <string>
#include "../Base/base.h"
#include "../Base/singleton.h"
#include <QObject>


#define NET_CONF_PATH  "/home/debian/Cat/net.conf"

class ReadConf : public QObject
{
    Q_OBJECT
public:
    ReadConf();
    std::string getID();
    std::string getAdd();
    std::string getPort();
    std::string getTopic1();
    std::string getQos1();
    std::string getVerson();
    std::string getAirVerson();
    void setAirVerson(std::string ver);
signals:
    void onAirVersonChange();
private:
    //ree
    std::string m_id;
    std::string m_add;
    std::string m_port;
    std::string m_topic1;
    std::string m_qos1;
    std::string m_verson;
    std::string m_airVerson;
    friend class Singleton<ReadConf>;
};

typedef Singleton<ReadConf> ReadConf_Single;    // 方便使用

#endif // READCONF_H
