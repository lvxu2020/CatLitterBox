#include "taskFromTerminal.h"
#include <stdlib.h>
#include <fcntl.h>

#define GROUP_LINE 10

/********************************
 * ;1;1;data;
 * 上面为报文格式.
 * 解析出想要的数据结构
 ****************************** */
bool analysis_task_from_terminal(void *data,int len)
{
    DEBUG_D("analysis_task_from_terminal in");
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

    Task *task = (Task*)malloc(sizeof(Task));
    bzero(task, sizeof(Task));
    task->id = atoi(id);
    strncpy(task->data, value, sizeof(value));
    switch (atoi(cmd)) {
    case 0:{// 请求查看版本
        threadpool_add(pool, verson_task_deal, task);
    }
        break;
    case 1:{// 板子数据上报，写在本地文件中
        threadpool_add(pool, terminal_data_up_deal, task);
    }
        break;
    case 2:{// 请求下载路径
        threadpool_add(pool, request_download_deal, task);
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

void * verson_task_deal(void *arg)
{
    DEBUG_D("verson_task_deal in");
    Task *task = (Task*)arg;
    char send_buf[64] = {0};
    int N = task->id % GROUP_LINE;
    char topic_buf[16] = {0};
    sprintf(topic_buf, "sToC_%d", N);
    sprintf(send_buf, ";%d;5;%s;", task->id, config_arr[app_verson]);
    send_message(topic_buf, send_buf, strlen(send_buf) + 1);
    if (task) {
        free(task);
    }
    return NULL;
}

void * terminal_data_up_deal(void *arg)
{
    DEBUG_D("terminal_data_up_deal in");
    Task *task = (Task*)arg;
    char path[128] = {'\0'};
    sprintf(path,"%s/%d/%s", config_arr[server_usr_path], task->id, "status");
    int fd = open(path, O_WRONLY);
    if (fd < 0) {
        DEBUG_E("open %s fail", path);
    }
    lseek(fd, 0, SEEK_SET);//定位在文档开头
    write(fd, task->data, strlen(task->data));
    // 防止日后数据长短变化，有残留数据
    write(fd, "     ", strlen("     "));
    close(fd);
    if (task) {
        free(task);
    }
    return NULL;
}

void * request_download_deal(void *arg)
{
    DEBUG_D("request_download_deal in");
    Task *task = (Task*)arg;
    char send_buf[64] = {0};
    int N = task->id % GROUP_LINE;
    char topic_buf[16] = {0};
    sprintf(topic_buf, "sToC_%d", N);
    sprintf(send_buf, ";%d;2;%s;", task->id, config_arr[http_down_path]);
    send_message(topic_buf, send_buf, strlen(send_buf) + 1);
    if (task) {
        free(task);
    }
    return NULL;
}
