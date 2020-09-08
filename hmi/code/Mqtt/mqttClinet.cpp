#include "mqttClinet.h"
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

pthread_once_t MqttClinet::ponce_ = PTHREAD_ONCE_INIT;
MqttClinet* MqttClinet::value_ = NULL;


MqttClinet::MqttClinet()
{
    struct timeval tv;
    char time[64];
    gettimeofday(&tv,NULL);//获取1970-1-1到现在的时间结果保存到tv中
    snprintf(time,64,"id:%ld,%ld",tv.tv_sec,tv.tv_usec);
    mqttID = time;
}

MqttClinet::~MqttClinet()
{
    if (isInit) {
        int rc;
        if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS)
        {
            printf("Failed to disconnect, return code %d\n", rc);
            rc = EXIT_FAILURE;
        }
        MQTTClient_destroy(&client);
    }
}

MqttClinet* MqttClinet::getIntence()
{
    pthread_once(&ponce_, &MqttClinet::single);
    assert(value_ != NULL);
    return value_;
}

void MqttClinet::single()
{
    value_ = new MqttClinet();
    ::atexit(destroy);
}

void MqttClinet::destroy()
{
    delete value_;
    value_ = NULL;
}

void MqttClinet::setInit(std::string serverAdd, std::string port, int interval, int session)
{
    conn_opts = MQTTClient_connectOptions_initializer;
    addPort = "tcp://" + serverAdd + ":" + port;
    conn_opts.keepAliveInterval = interval;
    conn_opts.cleansession = session;
    isInit = true;
}


void MqttClinet::connlost(void *context,char *cause)
{
    printf("Connection lost\n");
    printf("cause %s\n",cause);
    MqttClinet::getIntence()->ConnectedServer = false;
}

std::string MqttClinet::getID()
{
    return mqttID;
}

int MqttClinet::connectMqtt()
{
    if (!isInit) {
        ConnectedServer = false;
        return NOT_INIT;
    }

    MQTTClient_create(&client,addPort.c_str(),mqttID.c_str(),MQTTCLIENT_PERSISTENCE_NONE,NULL);
    MQTTClient_setCallbacks(client,NULL,connlost,msgarrvd,NULL);
    int rc;
    if((rc=MQTTClient_connect(client,&conn_opts))!=MQTTCLIENT_SUCCESS){
        printf("MQTTClient_connect failure:%s\n",strerror(errno));
        ConnectedServer = false;
        return CONNECT_FAIL;
    }
    ConnectedServer = true;
    return 0;
}

int MqttClinet::subscribe(std::string topic, int qos, void *(*fun)(void *))
{
    if (fun == nullptr) {
        return -1;
    }
    callbackVec.insert(make_pair(topic,fun));
    if (MQTTCLIENT_SUCCESS != MQTTClient_subscribe(client,topic.c_str(),qos)) {
        DEBUG_E(" MqttClinet dingyue shibai");
        return 0;
    }
    DEBUG_I("MqttClinet dingyue succeed");
    return 1;
}

int MqttClinet::msgarrvd(void *context,char *topicName,int topicLen,MQTTClient_message *message)
{

    printf("Message arrived \n");
    printf("topic:%s\n",topicName);
    printf("Original Message :%s\n",message->payload);
    std::string topicNameStr = topicName;
    std::map<std::string, void *(*)(void *)>::iterator iter;
    iter = MqttClinet::getIntence()->callbackVec.find(std::string(topicName));
    if (MqttClinet::getIntence()->callbackVec.empty() ||
            iter == MqttClinet::getIntence()->callbackVec.end()) {
        MQTTClient_freeMessage(&message);
        MQTTClient_free(topicName);
        printf("meiyou call back \n");
        return 0;
    }
    MqttMess mess;
    mess.len = message->payloadlen;
    mess.p = message->payload;
    (*iter->second)((void*)&mess);




    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;

}
















