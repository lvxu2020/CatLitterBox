#include "ReadConf.h"
#include <unistd.h>
#include <fstream>

#define RUNNING_PTAH "/home/debian/Back/running"
#define VERSION_PATH_1 "/home/debian/Cat/ver1/ver.conf"
#define VERSION_PATH_2 "/home/debian/Cat/ver2/ver.conf"

ReadConf::ReadConf()
{
    std::fstream fileStream, run;
    string file(NET_CONF_PATH);
    // 判断文件是否存在
    if(access(file.c_str(), 0) != 0) {
        DEBUG_E("%s not exist \n",NET_CONF_PATH);
    }
    // 打开文件
    fileStream.open(file.c_str(), std::fstream::in);
    if(!fileStream.is_open()) {
        DEBUG_E("%s not exist \n",NET_CONF_PATH);
    }
    std::string str;
    if(getline(fileStream,str)){
        m_id = str.substr(3,str.size());
    }
    if(getline(fileStream,str)){
        m_add = str.substr(strlen("address:"),str.size());
    }
    if(getline(fileStream,str)){
        m_port = str.substr(5,str.size());
    }
    if(getline(fileStream,str)){
        m_qos1 = str[6];
        m_topic1 = str.substr(8,str.size());
    }
    fileStream.close();
    run.open(RUNNING_PTAH);
    if (getline(run, str)) {

        if (str == "1") {
            std::fstream ver;
            ver.open(VERSION_PATH_1);
            if (getline(ver, str)) {
                m_verson = str;
            }
            ver.close();
        } else if (str == "2") {
            std::fstream ver;
            ver.open(VERSION_PATH_2);
            if (getline(ver, str)) {
                m_verson = str;
            }
            ver.close();
        }
    }
    run.close();
}

std::string ReadConf::getID()
{
    return m_id;
}

std::string ReadConf::getAdd()
{
    return m_add;
}

std::string ReadConf::getPort()
{
    return m_port;
}

std::string ReadConf::getTopic1()
{
    return m_topic1;
}

std::string ReadConf::getQos1()
{
    return m_qos1;
}

std::string ReadConf::getVerson()
{
    return m_verson;
}

std::string ReadConf::getAirVerson()
{
    return m_airVerson;
}

void ReadConf::setAirVerson(std::string ver)
{
    m_airVerson = ver;
}




