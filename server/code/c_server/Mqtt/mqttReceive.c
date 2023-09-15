#include "mqttReceive.h"
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdbool.h>

MQTTClient client_r;
MQTTClient_message publish_msg_r = MQTTClient_message_initializer;
MQTTClient_connectOptions conn_opts_r = MQTTClient_connectOptions_initializer;
long timeout_r = 5000L;
bool init_r = false;
// volatile每次用到此值时从寄存器重新读取，而不是在缓存中拿数据
//volatile MQTTClient_deliveryToken deliveredtoken;

bool qmtt_recv_init(INIT_RECV_PRAM * pram)
{
    DEBUG_I("qmtt_recv_init in");
    timeout_r = pram->timeout;
    conn_opts_r.keepAliveInterval = pram->keepAliveInterval;
    conn_opts_r.cleansession = pram->cleansession;
    //创建链接
    MQTTClient_create(&client_r,pram->address,pram->pub_id,MQTTCLIENT_PERSISTENCE_NONE,NULL);
    MQTTClient_setCallbacks(client_r,NULL,connlost,msgarrvd,NULL);
    int rc;
    if ((rc = MQTTClient_connect(client_r, &conn_opts_r)) != MQTTCLIENT_SUCCESS) {
        DEBUG_E("MQTTClient_connect failure:%s\n", strerror(errno));
        return false;
    }
    MQTTClient_subscribe(client_r, pram->topic, pram->qos);
    init_r = true;
    return true;
}

/* ********************************************************
 * 服务器收到消息会调用此回调函数
 * ******************************************************** */
int msgarrvd(void *context, char *topic_name, int topic_len, MQTTClient_message *message)
{
    DEBUG_D("topic:%s", topic_name);
    DEBUG_D("message :%s", message->payload);
    DEBUG_D("message len:%d", message->payloadlen);
    analysis_task_from_terminal(message->payload, message->payloadlen);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topic_name);
    return 1;
}

/* ***********************
 * 与服务器断开练级时此函数触发
 * 一般在重名id链接服务器时会
 * 触发此函数，可以在此函数内
 * 调用从连服务器，从新订阅消息
 * 的逻辑
 ************************* */
void connlost(void *context,char *cause)
{
    DEBUG_E("Connection lost\n");
    DEBUG_E("cause %s\n",cause);
}

/* **************************************
 * 消息接收状态，在MQTTClient_setCallbacks
 * 的最后一位参数。本程序不设置此函数。
 * ************************************ */
//void delivered(void *context,MQTTClient_deliveryToken dt)
//{
//    printf("Message with token value %d delivery confirmed\n",dt);
//    deliveredtoken=dt;
//}




