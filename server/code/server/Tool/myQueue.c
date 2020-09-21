#include "myQueue.h"

static pthread_once_t  ponce_;
static myQueue *ptr = NULL;
//获取单例
myQueue *getQueue()
{
    //只执行一次
    pthread_once(&ponce_, &initQueue);
    return ptr;
}

void initQueue()
{
    ptr = (myQueue *)malloc(sizeof(myQueue));
    Empty_Queue(ptr);
}

//判断是否空。
int is_empty(myQueue *p)
{
    bool res;
    pthread_mutex_lock(&mut);
    if (p->front == p->rear) {
        res = true;
    }else {
        res = false;
    }
    pthread_mutex_unlock(&mut);
    return res;
}

//循环队列判断是否满队：
bool full_queue(myQueue *p)
{
    bool res;
    pthread_mutex_lock(&mut);
    if ((p->rear + 1) % MQ_DATA_MAX == p->front) {
        res = true;
    }else {
        res = false;
    }
    pthread_mutex_unlock(&mut);
    return res;
}

//入队： 使用队尾指针。 成功入队：返回0，失败：返回-1.
bool Enter_Queue(myQueue *p,qData *in)
{

    if(full_queue(p))
    return false;
    pthread_mutex_lock(&mut);
    memcpy(p->data[p->rear].buf, in->buf, BUF_MAX);
    p->data->N = in->N;
    p->data->id = in->id;
    p->rear ++;
    p->rear = p->rear % MQ_DATA_MAX;
    pthread_mutex_unlock(&mut);
    return true;
}

//出队：使用队头指针 front  成功出队：返回0，失败：返回-1
bool De_Queue(myQueue *p,qData *out)
{
    if(is_empty(p))
    return false;
    pthread_mutex_lock(&mut);
    memcpy(out->buf, p->data[p->front].buf, BUF_MAX);
    out->N = p->data->N;
    out->id = p->data->id;
    p->front ++;
    p->front = p->front % MQ_DATA_MAX;
    pthread_mutex_unlock(&mut);
    return true;
}

//归零数据
void Empty_Queue(myQueue *p)
{
    if(p == NULL){
        return;
    }
    pthread_mutex_lock(&mut);
    memset(p->data,'\0',sizeof(p->data));
    p->front = 0;
    p->rear = 0;
    pthread_mutex_unlock(&mut);
}

//销毁队列
void Destory_Queue(myQueue *p)
{
    pthread_mutex_lock(&mut);
    free(ptr);
    ptr = NULL;
    pthread_mutex_unlock(&mut);
}


