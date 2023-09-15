#ifndef _MQTTSEND_H_
#define _MQTTSEND_H_

#include "../Debug/Debug.h"
#include "../Config/config.h"
#include "MQTTClient.h"

typedef struct {
    // "tcp://%s:%d",address,port
    char address[64];
    long timeout;
    int keepAliveInterval;
    int cleansession;
    char pub_id[128];
    int qos;
    int retained;
}INIT_SEND_PRAM;

bool qmtt_send_init(INIT_SEND_PRAM * pram);
bool send_message(char *topic, void *send_buf, int buf_len);



#endif
