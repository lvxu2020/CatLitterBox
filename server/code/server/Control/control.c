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
    /*把用户分组，省的mqtt服务器发送大量数据
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


