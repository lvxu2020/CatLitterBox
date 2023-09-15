#ifndef _TASKFROMWEB_H_
#define _TASKFROMWEB_H_

#include "../Debug/Debug.h"
#include "../Config/config.h"
#include "../Mqtt/mqttSend.h"
#include "pool.h"

extern threadpool_t *pool;

typedef struct {
    int id;
    char data[5];
}Task_w;

bool analysis_task_from_web(void *data, int len);
void* remote_motor_deal(void *arg);
void* remote_led_deal(void *arg);
void* remote_status_upload(void *arg);

#endif
