#include "mqttSend.h"
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>


MqttSend::MqttSend()
{
    struct timeval tv;
    char time[64];
    gettimeofday(&tv,NULL);//获取1970-1-1到现在的时间结果保存到tv中
    snprintf(time,64,"id:%ld,%ld",tv.tv_sec,tv.tv_usec);
    mqttID = time;
}

MqttSend::~MqttSend()
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

void MqttSend::setInit(std::string serverAdd, std::string port, int interval, int session)
{
    conn_opts = MQTTClient_connectOptions_initializer;
    publish_msg = MQTTClient_message_initializer;
    addPort = "tcp://" + serverAdd + ":" + port;
    conn_opts.keepAliveInterval = interval;
    conn_opts.cleansession = session;
    isInit = true;
}

int MqttSend::connectMqtt()
{
    if (!isInit) {
        ConnectedServer = false;
        return NOT_INIT;
    }

    MQTTClient_create(&client,addPort.c_str(),mqttID.c_str(),MQTTCLIENT_PERSISTENCE_NONE,NULL);
    int rc;
    if((rc=MQTTClient_connect(client,&conn_opts))!=MQTTCLIENT_SUCCESS){
        printf("MQTTClient_connect failure:%s\n",strerror(errno));
        ConnectedServer = false;
        return CONNECT_FAIL;
    }
    ConnectedServer = true;
    return 0;
}

void MqttSend::sendMess(std::string topic, std::string text, int retained, int qos, long timeOut)
{
    publish_msg.qos = qos;
    publish_msg.retained = retained;
    publish_msg.payload=(void *)text.c_str();
    publish_msg.payloadlen=text.size() + 1;//把 '\0' 带过去
    MQTTClient_publishMessage(client,topic.c_str(),&publish_msg,&token);
    int rv = MQTTClient_waitForCompletion(client,token,timeOut);//rv 暂时不做判断
}
