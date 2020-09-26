#ifndef _MQTTSENG_H_
#define _MQTTSENG_H_

#include "../Debug/Debug.h"
#include "../Config/config.h"
#include "MQTTClient.h"

typedef struct {
    MQTTClient client;
    char address[128];
    long timeout;
    int keepAliveInterval;
    int cleansession;
    char pub_id[128];
    // MQTTClient_connectOptions_initializer
    MQTTClient_connectOptions conn_opts;
    // MQTTClient_message_initializer
    MQTTClient_message publish_msg;
    int qos;
    int retained;
    MQTTClient_deliveryToken token;

}INIT_PARM;

bool qmtt_send_init(INIT_PARM *);



#endif
