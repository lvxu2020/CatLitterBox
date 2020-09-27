#include "control.h"

void * remote_control(void *p)
{
    DEBUG_I("remote_control start");
    // 创建消息队列
    int msg_id = get_mq_id(MQ_KEY_PATH, MQ_KEY_CHAR);
    if (msg_id < 0) {
        DEBUG_E("msg_id creat fail");
        return NULL;
    }

    MSG rec_buf;
    int n;
    while (1) {
        /* ********************************************************************
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
         * ********************************************************************* */
        n = msgrcv(msg_id, &rec_buf, MQ_MSGBUF_LEN, 0, 0);
        if (n != -1) {
            process_task(&rec_buf);
        }
    }
    return NULL;
}

/* ************************
 * 功能：获取消息队列id
 * 输出path：约定的路径
 * 输入ch：约定的字符
 * 输出：成功id，失败负数
 * ************************ */
int get_mq_id(char *path, char ch)
{
    key_t key = ftok(MQ_KEY_PATH,MQ_KEY_CHAR);
    int msg_id = msgget(key,O_RDWR);
    if (msg_id >= 0) {
        msgctl(msg_id,IPC_RMID,NULL);
    }
    msg_id = msgget(key,IPC_CREAT|0664);
    return msg_id;
}

/* ********************************
 * 功能：处理消息队列中的消息分发到别处
 * 输入p；消息队列消息指针
 * 输出：空
 * ******************************** */
void process_task(MSG *p)
{
    analysis_task_from_web(p->msg, strlen(p->msg) + 1);
}
