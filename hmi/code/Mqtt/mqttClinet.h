#ifndef MQTTCLINET_H
#define MQTTCLINET_H

#include <string>
#include <map>
#include "MQTTClient.h"
#include <pthread.h>
#include <assert.h>
#include <stdlib.h> // atexit
#include <fstream>
#include <iostream>
#include <mutex>
#include "../LOG/Debug.h"

#define NOT_INIT -9
#define CONNECT_FAIL -8



typedef struct{
    int len;
    void *p;
} MqttMess;

class MqttClinet
{
public:

    static MqttClinet *getIntence();
    //MQTT接口使用的函数得是静态的。
    static void connlost(void *context,char *cause);
    static int msgarrvd(void *context,char *topicName,int topicLen,MQTTClient_message *message);
    std::string getID();
    /*******session
    一般来说，客户端在请求连接服务器时总是将清除会话标志设置为0或1，在建立会话连接后
    ，这个值就固定了，当然这个值的选择取决于具体的应用，如果清除会话标志设置为1，那么客户
    端不会收到旧的应用消息，而且在每次连接成功后都需要重新订阅相关的主题。清除会话标志设
    置为0的客户端在重新连接后会收到所有在它连接断开期间（其他发布者）发布的QoS1和QoS2级别
    的消息。因此，要确保不丢失连接断开期间的消息，需要使用QoS1或 QoS2级别，同时将清除
    会话标志设置为0。
    */
    /**************interval***********
     * 参考：
     * TCP keepAlive和KeepAliveInterval参数详解-转
     *
     * keep-alive 将在每 KeepAliveInterval 秒重发一次。KeepAliveInterval 默认为1秒。
    * *******************************/
    //ip地址，端口，链接超时时间,session
    void setInit(std::string serverAdd, std::string port, int interval, int session);
    //链接mqtt服务器
    int connectMqtt();
    //订阅主题,服务质量，设置回调函数(此回调函数生命周期尽量短暂。以免影响下一条消息接收)
    int subscribe(std::string topic, int qos, void *(*fun)(void *));
private://单利
    static void single();
    static void destroy();
    static pthread_once_t ponce_;
    static MqttClinet* value_;
    MqttClinet();
    ~MqttClinet();
private:
    MQTTClient client;
    volatile MQTTClient_deliveryToken deliveredtoken;
    MQTTClient_connectOptions conn_opts;
    std::string addPort;
    std::string mqttID;
    std::map<std::string, void *(*)(void *)> callbackVec;
    bool isInit = false;
    bool ConnectedServer = false;

};

#endif // MQTTCLINET_H
