#ifndef _MQTTRECEIVE_H_
#define _MQTTRECEIVE_H_

#include "../Debug/Debug.h"
#include "../Config/config.h"
#include "MQTTClient.h"
#include "../Task/taskFromTerminal.h"

typedef struct {
    // "tcp://%s:%d",address,port
    char address[64];
    long timeout;
    int keepAliveInterval;
    int cleansession;
    char pub_id[128];
    char topic[32];
    int qos;
}INIT_RECV_PRAM;

bool qmtt_recv_init(INIT_RECV_PRAM * pram);
int msgarrvd(void *context, char *topic_name, int topic_len, MQTTClient_message *message);
void connlost(void *context,char *cause);

#endif
