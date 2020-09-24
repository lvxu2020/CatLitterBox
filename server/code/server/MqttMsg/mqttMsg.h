#ifndef _MQTTMSG_H_
#define _MQTTMSG_H_

#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include "MQTTClient.h"
#include "../Tool/myQueue.h"
#include "../Tool/pool.h"

#define CONFIG_PATH "/home/lvxu/server.conf"
#define CONFIG_H 2  //读取server.cong 的buf行数
#define CONFIG_L 64
#define TOPIC_H 10
#define TOPIC_L 64
#define REC_BUF_MAX 128
#define C_TO_S_HEAD "cTos:"
#define USER_CONF_PATH "/home/lvxu/catServer/usr/"
#define TOPIC_PATH "/home/lvxu/serverTop"
#define STATUS "status"
#define POOL_PTH_MIN 3
#define POOL_PTH_MAX 100
#define POOL_QUEUE_MAX 100
#define QOS_1 1

typedef struct {
    int F_id; //负值
    char buf[20];//板子的唯一id
} regReq;

typedef struct {
    int qos;
    char topic[64];
} topicQos;

typedef struct{
    int port;
    char address[128];
    char id[128];
    char topic[128];
} NetParemeter;

typedef struct{
    int arr_N;
    int id;
    int type;
    void * (*fun)(void *);
    char buf[REC_BUF_MAX];

} DataFromClient;


typedef struct {
    bool busy;
    DataFromClient parm;
} parmElement;


void * mqttMsgRec(void *p);
void * mqttMsgSend(void *p);
int opt_init( char buf[]);
void getID(char *time);
void delivered(void *context,MQTTClient_deliveryToken dt);
void connlost(void *context,char *cause);
int msgarrvd(void *context,char *topicName,int topicLen,MQTTClient_message *message);
bool getPubTask(myQueue *sendQ_ptr,qData *out);
void pushTaskPool(char *data);
bool analysisTask(DataFromClient *task,char *data);
void *statusUpdate(void *arg);
void *regiseterId(void *arg);
bool getParmPtr(parmElement *arr, DataFromClient **p, int max);
void freeParmPtr( parmElement *arr, int arr_N);
void *functionVec( void *p);
int getSubscribe(MQTTClient client);

#endif  //_MQTTMSG_H_
