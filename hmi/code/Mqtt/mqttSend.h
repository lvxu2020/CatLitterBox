#ifndef MQTTSEND_H
#define MQTTSEND_H


#include <string>
#include "MQTTClient.h"

#define NOT_INIT -9
#define CONNECT_FAIL -8

class MqttSend
{
public:
    MqttSend();
    ~MqttSend();
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
    //发送消息。主题，内容，保留时间，服务质量，超时等待时间(毫秒)
    void sendMess(std::string topic, std::string text, int retained, int qos, long timeOut);
private:
    MQTTClient client;
    volatile MQTTClient_deliveryToken deliveredtoken;
    MQTTClient_message publish_msg;
    MQTTClient_deliveryToken token;
    MQTTClient_connectOptions conn_opts;
    std::string addPort;
    std::string mqttID;
    bool isInit = false;
    bool ConnectedServer = false;
};

#endif // MQTTSEND_H
