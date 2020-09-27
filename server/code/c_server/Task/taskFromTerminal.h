#ifndef _TASKFROMTERMINAL_H_
#define _TASKFROMTERMINAL_H_

#include "../Debug/Debug.h"
#include "../Config/config.h"
#include "../Mqtt/mqttSend.h"
#include "pool.h"

extern threadpool_t *pool;

typedef struct {
    int id;
    char data[5];
}Task;

bool analysis_task_from_terminal(void *data, int len);
void * verson_task_deal(void *arg);
void * terminal_data_up_deal(void *arg);
void * request_download_deal(void *arg);

#endif
