#include "mqttMsg.h"

#define PORT 1883
#define ADDRESS "192.168.1.152"
#define TOPIC_S_C "sToC_"
#define TOPIC_C_S "cToS"

threadpool_t *pool = NULL;
//每次用到此值时从寄存器重新读取，而不是在缓存中拿数据
volatile MQTTClient_deliveryToken deliveredtoken;
//队列指针地址空间
parmElement pArr[POOL_QUEUE_MAX];

void * mqttMsgRec(void *p)
{

    MQTTClient client;
    const long timeout = 5000L;
    char connectBuf[128];
    char sub_id[128] = {'\0'};
    getID(sub_id);

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    if (opt_init(connectBuf) < 0) {
        printf("opt_init failure:%s\n",strerror(errno));
        return 1;
    }
    printf("du qv %s\n",connectBuf);
    //创建链接
    MQTTClient_create(&client,connectBuf,sub_id,MQTTCLIENT_PERSISTENCE_NONE,NULL);
    conn_opts.keepAliveInterval=20;
    conn_opts.cleansession=1;
    //设置回调函数
    MQTTClient_setCallbacks(client,NULL,connlost,msgarrvd,NULL);
    int rc;
    if((rc=MQTTClient_connect(client,&conn_opts))!=MQTTCLIENT_SUCCESS){
        printf("MQTTClient_connect failure:%s\n",strerror(errno));
        return -1;
    }
    //订阅主题
    getSubscribe(client);
    while (pool == NULL) {
        /*创建线程池，池里最小3个线程，最大100，队列最大100*/
        pool = threadpool_create(POOL_PTH_MIN,POOL_PTH_MAX,POOL_QUEUE_MAX);
        sleep(1);
    }
    //清零线程池中回调函数的参数数组。
    bzero( pArr, sizeof(pArr));
    printf("pool creat succeed!\n");
    while (1) {
        printf("  mqttMsgRec sleep 20\n");
        sleep(20);
    }

}

void * mqttMsgSend(void *p)
{

    MQTTClient client;
    const int qos = 0;
    const long timeout = 1000L;
    qData task;
    int port = PORT;
    char address[128] = {ADDRESS};
    char address_s[128];
    char pub_id[128] = {'\0'};
    getID(pub_id);
    char topic[128] = {'\0'};
    char sendBuf[128] = {'\0'};
    snprintf(address_s,sizeof(address_s),"tcp://%s:%d",address,port);
    MQTTClient_connectOptions conn_opts=MQTTClient_connectOptions_initializer;
    MQTTClient_message publish_msg=MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    conn_opts.keepAliveInterval=60;
    conn_opts.cleansession=1;
    MQTTClient_create(&client,address_s,pub_id,MQTTCLIENT_PERSISTENCE_NONE,NULL);
    int rv;
    if((rv=MQTTClient_connect(client,&conn_opts))!=MQTTCLIENT_SUCCESS){
        printf("MQTTClient_connect error:%s\n",strerror(errno));
        return -1;
    }
    publish_msg.qos=qos;
    publish_msg.retained=0;
    myQueue *sendQ_ptr = getQueue();

    while (1) {
        bzero(&task,sizeof(task));
        if( !getPubTask(sendQ_ptr,&task) ){
            continue;
        }
        bzero(topic,sizeof(topic));
        sprintf(topic,"%s%d",TOPIC_S_C,task.N);
        bzero(sendBuf,sizeof(sendBuf));
        sprintf(sendBuf,"%d;%s",task.id,task.buf);
        printf("send topic: %s,,,%s",topic,sendBuf);
        publish_msg.payload=(void *)sendBuf;
        publish_msg.payloadlen=strlen(sendBuf)+1;
        MQTTClient_publishMessage(client,topic,&publish_msg,&token);
        rv=MQTTClient_waitForCompletion(client,token,timeout);
    }

}

void getID(char *time)
{
    struct timeval tv;
    gettimeofday(&tv,NULL);//获取1970-1-1到现在的时间结果保存到tv中
    snprintf(time,128,"id:%ld,%ld",tv.tv_sec,tv.tv_usec);
}

int opt_init( char buf[])
{
    char conf[CONFIG_H][CONFIG_L] = {'\0'};
    char tempBuf[CONFIG_L] = {'\0'};
    FILE *fp = fopen(CONFIG_PATH,"r");
    if (NULL == fp) {
        sprintf(buf,"tcp://127.0.0.1:1883");
        printf("open server.conf faled\n");
    }else {
        int i = 0;
        while (i < CONFIG_H  && fgets(tempBuf,CONFIG_L,fp) != NULL ) {
            char *p = tempBuf;
            bool start = false;
            int j = 0;
            while (*p != '\0') {
               if (*p == ':') {
                  start = true;
                  p++;
               }
               if (start && *p != '\n') {
                   conf[i][j++] = *p;
               }
               p++;
            }
            bzero(tempBuf,sizeof(tempBuf));
            i++;
        }
        sprintf(buf,"tcp://%s:%s",conf[0],conf[1]);
        fclose(fp);
    }
    return 1;
}

int getSubscribe(MQTTClient client)
{
    FILE *fp = fopen(TOPIC_PATH,"r");
    if (NULL == fp) {
        return 0;
    }else {
        char tempBuf[64] = {'\0'};
        char tempQos[2] = {'\0'};
        topicQos top[TOPIC_H];
        bzero(top,sizeof(top));
        int i = 0;
        while (i < TOPIC_H  && fgets(tempBuf,TOPIC_L,fp) != NULL ) {

            char *p = tempBuf;
            if (0 != memcmp(p,"qos:",strlen("qos:"))) {
                break;
            }
            int j = 0;
            while (*p != ';' && *p != '\0') {
                if (j++ >= strlen("qos:")) {
                    tempQos[0] = *p;
                    printf("qos is %s\n",tempQos);
                    top[i].qos = atoi(tempQos);
                    p += 2;
                    break;
                }
                p++;
            }
            j = 0;
            while (*p != '\n' && *p != '\0' && i < CONFIG_L) {
                top[i].topic[j++] = *p;
                p++;
            }
            printf("jiexi tpoic is %s\n",top[i].topic);
            bzero(tempBuf,sizeof(tempBuf));
            i++;
        }
        int connect = 0;
        while (connect != i) {
            printf("Subscribe to topic %s  using QOS %d\n",top[connect].topic,top[connect].qos);
            MQTTClient_subscribe(client,top[connect].topic,top[connect].qos);
            connect++;
        }
        return i;
    }



}

void delivered(void *context,MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n",dt);
    deliveredtoken=dt;
}

/*******connlost**********
 * 与服务器断开练级时此函数触发
 * 一般在重名id链接服务器时会
 * 触发此函数，可以在此函数内
 * 调用从连服务器，从新订阅消息
 * 的逻辑
 ************************/
void connlost(void *context,char *cause)
{
    printf("Connection lost\n");
    printf("cause %s\n",cause);
}

/********************************
 * cTos:125;1;数据;
 * 上面为报文格式.
 * 解析出想要的数据结构
 ****************************** */
bool analysisTask(DataFromClient *task,char *data)
{
    printf("analysisTask in %s\n",data);
    if (task == NULL) {
        return false;
    }
    char *p = data;
    char id[10] = {'\0'};
    char type[10] = {'\0'};
    if (0 != memcmp(p,C_TO_S_HEAD,strlen(C_TO_S_HEAD))) {
        return false;
    }
    p += strlen(C_TO_S_HEAD);
    int i = 0;
    while (*p != ';' && i < 9) {//小于9保证最后一位有\0
        id[i++] = *p++;
    }
    task->id = atoi(id);
    if(*p++ != ';'){
        return false;
    }
    i = 0;
    while (*p != ';' && i < 9) {//小于9保证最后一位有\0
        type[i++] = *p++;
    }
    task->type = atoi(type);
    if(*p++ != ';'){
        return false;
    }
    i = 0;
    while (*p != ';' && i < REC_BUF_MAX -1) {//保证最后一位有\0
        task->buf[i++] = *p++;
    }
    if(*p++ != ';'){
        return false;
    }else{
        switch (task->type) {
        case 0:{//消息上报，写在本地文件中
            task->fun = &statusUpdate;
        }break;
        case 1:{
            task->fun = &regiseterId;
        }break;
        default :break;
        }
        return true;
    }
}

/* *******板子第一次联网后，向服务器申请id********
* 数据格式：cTos:125;1；
****************************************** */
void *regiseterId(void *arg)
{
    DataFromClient *p = (DataFromClient *)arg;
    int n = strlen(p->buf);
    n = n < REC_BUF_MAX ? n: REC_BUF_MAX;

    return;
}

/**************************
 * 周期性上传，丢几次没事
 * ********************** */
void *statusUpdate(void *arg)
{
    DataFromClient *p = (DataFromClient *)arg;
    int n = strlen(p->buf);
    n = n < REC_BUF_MAX ? n: REC_BUF_MAX;
    char path[128] = {'\0'};
    sprintf(path,"%s/%d/%s",USER_CONF_PATH,p->id,STATUS);
    int fd = open(path,O_WRONLY);
    if (fd < 0) {
        return ;
    }
    lseek(fd,0,SEEK_SET);//定位在文档开头
    write(fd,p->buf,n);
    close(fd);
}

bool getParmPtr(parmElement *arr, DataFromClient **p, int max)
{
    int i = 0;
    for(;i < max;i++){
        if(!arr[i].busy){
            *p = &arr[i].parm;
            arr[i].busy = true;
            return true;
        }
    }
    return false;
}

void freeParmPtr( parmElement *arr, int arr_N)
{
    //清零操作后 busy位置 责为false。不用再手动置零了。
    bzero(&arr[arr_N], sizeof(parmElement));
}

/*************************
*为了避免内存碎片，将程序需要盛情
* 的内存放在数组中，所已此函数执行
* 之后会讲所用的数组置为空闲
************************ */
void *functionVec( void *p)
{
    DataFromClient *ptr = (DataFromClient*)p;
    ptr->fun(ptr);
    freeParmPtr(pArr,ptr->arr_N);
}

/* *****************
 *解析收到的mqqt字符串
 *放入线程池的队列中
 **************** */
void pushTaskPool(char *data)
{
    lock_pool_mutex(pool);
    //从数组中拿空闲的指针。
    DataFromClient *p =NULL;
    //队列满了，请求不受理
    if ( is_pool_queue_full(pool) || !getParmPtr(pArr,&p,sizeof(pArr))) {
        unlock_pool_mutex(pool);
        return;
    }
    if(!analysisTask(p,data)){
        unlock_pool_mutex(pool);
        printf("analysisTask faile\n");
        return;
    }
    //解析后的数据放进队列，此p的数据结构中已经包含了执行任务的函数
    threadpool_add(pool, functionVec, (void *)p);/* 向任务队列中， 添加一个任务 */
    unlock_pool_mutex(pool);
}
//上文为了避免过多申请内存造成内存碎片，而采用新手法。我可真牛逼，哈哈哈
//void pushTaskPool(char *data)
//{
//    lock_pool_mutex(pool);
//    //队列中保存的是指针，所以得申请个空间存参数。线程昨晚任务会释放掉这个参数
//    DataFromClient *p = (DataFromClient *)malloc(sizeof(DataFromClient));
//    if (p == NULL) {
//        unlock_pool_mutex(pool);
//        return;
//    }
//    if(!analysisTask(p,data)){
//        unlock_pool_mutex(pool);
//        return;
//    }
//    printf("++++++++jiexichengong++++task.id is %d+\n",p->id);
//    switch (p->type) {
//    case 0:{//消息上报，写在本地文件中
//        threadpool_add(pool, statusUpdate, (void *)p);/* 向任务队列中， 添加一个任务 */
//    }break;
//    default :break;
//    }
//    unlock_pool_mutex(pool);
//}

int msgarrvd(void *context,char *topicName,int topicLen,MQTTClient_message *message)
{
    char *payloadptr = message->payload;
    char getMsgBuf[128] = {'\0'};
//    printf("Message arrived \n");
//    printf("topic:%s\n",topicName);
//    printf("Original Message :%s\n",message->payload);
//    printf("analysis Message :");
    int i;
    for (i=0; i < message->payloadlen; i++) {
        getMsgBuf[i] = *payloadptr;
        putchar(*payloadptr++);
    }
    printf("\ngetMsgBuf lvxu:%s\n",getMsgBuf);
    pushTaskPool(getMsgBuf);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;

}

bool getPubTask(myQueue *sendQ_ptr,qData *out)
{
    if(is_empty(sendQ_ptr)){
        //业务逻辑暂时没那么多，先加一秒延时挂机等待
        sleep(1);
        return false;
    }
    if(De_Queue(sendQ_ptr,out)){
        printf("getTask success! type:%d,%s\n",out->N,out->buf);
        return true;
    }else {
        return false;
    }
}








