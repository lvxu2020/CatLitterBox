#include "control.h"

myQueue *sendQ_ptr = NULL;


void * remoteCmd(void *p)
{
   int recId = getMqId(MQ_KEY_PATH,MQ_KEY_CHAR);
   sendQ_ptr = getQueue();
   if (recId < 0) {
       return -1;
   }
   MSG recBuf;
   int n;
   printf("queue start !!!\n");
   while (1) {
        /* ******************************************
         *
         * 第四个参数mtype
         * 指定了函数从队列中所取的消息的类型。函数将从队列中搜索类型与之匹配的消息并将之
         * 返回。不过这里有一个例外。如果mtype 的值是零的话，函数将不做类型检查而自动返回
         * 队列中的最旧的消息。
         * 第五个参数依然是是控制函数行为的标志，取值可以是：
         * 0,表示忽略；
         * IPC_NOWAIT，如果消息队列为空，则返回一个ENOMSG，并将控制权交回调用函数
         * 的进程。如果不指定这个参数，那么进程将被阻塞直到函数可以从队列中得到符合条件的
         * 消息为止。如果一个client 正在等待消息的时候队列被删除，EIDRM 就会被返回。如果进
         * 程在阻塞等待过程中收到了系统的中断信号，EINTR 就会被返回。
         * MSG_NOERROR，如果函数取得的消息长度大于msgsz，将只返回msgsz 长度的信息，
         * 剩下的部分被丢弃了。如果不指定这个参数，E2BIG 将被返回，而消息则留在队列中不被
         * 取出。
         * 当消息从队列内取出后，相应的消息就从队列中删除了。
         * *************************************** */
        n = msgrcv(recId,&recBuf,MQ_MSGBUF_LEN,0,0);
        if (n != -1){
            processTask(&recBuf);
        }

   }

}

int getMqId(char *path,char ch)
{
    key_t key = ftok(MQ_KEY_PATH,MQ_KEY_CHAR);
    int msgid = msgget(key,O_RDWR);
    if (msgid >= 0) {
        msgctl(msgid,IPC_RMID,NULL);
        printf("clear\n");
    }
    msgid = msgget(key,IPC_CREAT|0664);
    return msgid;
}

int processTask(MSG *p)
{
    qData in;
    /* 把用户分组，省的mqtt服务器发送大量数据
     * 用户注册用户时会根据板子id号编进不同
     * 分组。GROUP_LINE 决定分组系数
     *
     * *******************************/
    in.N = p->type % GROUP_LINE;
    in.id = p->type;
    snprintf(in.buf,BUF_MAX,"%s",p->msg);
    if(full_queue(sendQ_ptr)){
       Empty_Queue(sendQ_ptr);
    }
    Enter_Queue(sendQ_ptr,&in);
}


