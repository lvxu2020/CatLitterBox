#include "taskFromWeb.h"

#define GROUP_LINE 10

bool analysis_task_from_web(void *data, int len)
{
    DEBUG_D("analysis_task_from_web in ");
    char *data_p = (char*)data;
    int i = 0, cont = 0;
    for (; i < len; i++) {
        data_p[i] == ';' ? cont++ : 0;
    }
    if (cont != 4) {
        goto data_error;
    }

    char *p = data+1;
    char id[10] = {'\0'};
    char cmd[5] = {'\0'};
    char value[5] = {'\0'};
    i = 0;
    while (*p != ';' && i < 9) {
        id[i++] = *p++;
    }
    if (i == 9) goto data_error;

    i = 0; p++;
    while (*p != ';' && i < 4) {
        cmd[i++] = *p++;
    }
    if (i == 4) goto data_error;

    i = 0; p++;
    while (*p != ';' && i < 4) {
        value[i++] = *p++;
    }
    if (i == 4) goto data_error;

    Task_w *task = (Task_w*)malloc(sizeof(Task_w));
    bzero(task, sizeof(Task_w));
    task->id = atoi(id);
    strncpy(task->data, value, sizeof(value));
    switch (atoi(cmd)) {
    case 0:{// 远程控制电机
        threadpool_add(pool, remote_motor_deal, task);
    }
        break;
    case 1:{// 远程控制leg
        threadpool_add(pool, remote_led_deal, task);
    }
        break;
    case 4:{// 板子状态上报
        threadpool_add(pool, remote_status_upload, task);
    }
        break;
    default :{
        free(task);
    }break;
    }

    return true;
data_error:
    DEBUG_E("数据格式错误:%s", data_p);
    return false;
}

void * remote_motor_deal(void *arg)
{
    DEBUG_D("verson_task_deal in");
    Task_w *task = (Task_w*)arg;
    char send_buf[64] = {0};
    int N = task->id % GROUP_LINE;
    char topic_buf[16] = {0};
    sprintf(topic_buf, "sToC_%d", N);
    sprintf(send_buf, ";%d;0;%s;", task->id, task->data);
    send_message(topic_buf, send_buf, strlen(send_buf) + 1);
    if (task) {
        free(task);
    }
    return NULL;
}

void * remote_led_deal(void *arg)
{
    DEBUG_D("verson_task_deal in");
    Task_w *task = (Task_w*)arg;
    char send_buf[64] = {0};
    int N = task->id % GROUP_LINE;
    char topic_buf[16] = {0};
    sprintf(topic_buf, "sToC_%d", N);
    sprintf(send_buf, ";%d;1;%s;", task->id, task->data);
    send_message(topic_buf, send_buf, strlen(send_buf) + 1);
    if (task) {
        free(task);
    }
    return NULL;
}

void * remote_status_upload(void *arg)
{
    DEBUG_D("verson_task_deal in");
    Task_w *task = (Task_w*)arg;
    char send_buf[64] = {0};
    int N = task->id % GROUP_LINE;
    char topic_buf[16] = {0};
    sprintf(topic_buf, "sToC_%d", N);
    sprintf(send_buf, ";%d;4;%s;", task->id, task->data);
    send_message(topic_buf, send_buf, strlen(send_buf) + 1);
    if (task) {
        free(task);
    }
    return NULL;
}
