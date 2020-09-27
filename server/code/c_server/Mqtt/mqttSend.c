#include "mqttSend.h"
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

MQTTClient client;
MQTTClient_message publish_msg = MQTTClient_message_initializer;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
MQTTClient_deliveryToken token;
long timeout = 1000L;
bool init = false;

bool qmtt_send_init(INIT_SEND_PRAM *pram)
{
    DEBUG_I("qmtt_send_init start");
    timeout = pram->timeout;
    conn_opts.keepAliveInterval = pram->keepAliveInterval;
    conn_opts.cleansession = pram->cleansession;
    MQTTClient_create(&client, pram->address, pram->pub_id, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    int rv;
    if((rv = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS){
        DEBUG_E("MQTTClient_connect error:%s\n", strerror(errno));
        return false;
    }
    publish_msg.qos = pram->qos;
    publish_msg.retained = pram->retained;
    init = true;
    return true;
}

bool send_message(char *topic, void *send_buf, int buf_len)
{
    if (!init) {
       DEBUG_E("mqttSend not init!");
       return false;
    }
    publish_msg.payload = send_buf;
    publish_msg.payloadlen = buf_len;
    MQTTClient_publishMessage(client, topic, &publish_msg, &token);
    int rv=MQTTClient_waitForCompletion(client, token, timeout);
    if (rv != MQTTCLIENT_SUCCESS) {
        DEBUG_E("MQTTClient_waitForCompletion error:%s\n", strerror(errno));
        return false;
    }
    DEBUG_D("succeed send message %s",(char*)send_buf);
    return true;
}

