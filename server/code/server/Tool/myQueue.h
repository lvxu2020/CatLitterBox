#ifndef _MYQUEUE_H_
#define _MYQUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include "assert.h"
//队列容量
#define BUF_MAX 20
#define MQ_DATA_MAX 20


pthread_mutex_t mut;
typedef struct
{
    int N;//组
    int id;//板子id
    char buf[BUF_MAX];
}qData;

typedef struct
{
    qData data[20];
    int front,rear;
}myQueue;

//单例
myQueue *getQueue();
//队列初始化
void initQueue();
//判断是否空。
int is_empty(myQueue *p);
//循环队列判断是否满队：
bool full_queue(myQueue *p);
//入队： 使用队尾指针。 成功入队：返回0，失败：返回-1.
bool Enter_Queue(myQueue *p,qData *in);
//出队：使用队头指针 front  成功出队：返回0，失败：返回-1
bool De_Queue(myQueue *p,qData *out);
//归零数据
void Empty_Queue(myQueue *p);
//销毁队列
void Destory_Queue(myQueue *p);



#endif  //_MYQUEUE_H_
