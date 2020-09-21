#include "register.h"
#include <sys/ipc.h>
#include <strings.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <stdbool.h>






union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};


void * registerUsr(void *p)
{
    //信号量

    //创建信号量 用于cgi向服务器申请操作共享内存
    int sem_req_shm = getSemId(SEM_REQ_SHM_PATH,SEM_REQ_SHM_CHAR);
    if(sem_req_shm < 0)
    {
        perror("semget");
        return -1;
    }
    //设置信号灯,数量为1
    if(semctl(sem_req_shm, 0, SETVAL, 1) == -1){
        return 0;
    }

    //创建信号量 用于cgi向共享内存写入任务
    int sem_task = getSemId(SEM_CGIWRITE_PATH,SEM_CGIWRITE_CHAR);
    if(sem_task < 0)
    {
        perror("semget");
        return -1;
    }
    //设置信号灯,数量为1
    if(semctl(sem_task, 0, SETVAL, 1) == -1){
        return 0;
    }
    //P操作，让信号量为零，等待cgi那边释放，实现同步
    semaphore_p(sem_task);

    //创建信号量 用于服务器回应cgi任务处理的结果
    int sem_return = getSemId(SEM_RETURNCGI_PATH,SEM_RETURNCGI_CHAR);
    if(sem_return < 0)
    {
        perror("semget");
        return -1;
    }
    //设置信号灯,数量为1
    if(semctl(sem_return, 0, SETVAL, 1) == -1){
        return 0;
    }


    //共享内存
    char buffer[128] = {0};
    key_t key = ftok(SHM_PATH,SHM_CHAR);
    int shmid = shmget(key,0,0);
    if (shmid > 0) {//存在旧内存删掉
        shmctl(shmid,IPC_RMID,0);
    }
    shmid = shmget(key,SHM_MAX,IPC_CREAT|0644);    // 1. 创建
    if(shmid < 0) {
        perror("shmget");
        return -1;
    }
    char *shmBuf = shmat(shmid,NULL,0);   // 2. 映射
    if(shmBuf == NULL)
    {
        perror("shmat");
        return -1;
    }
    printf("shm start\n");
    bool first = true;
    while (1) {

        /* P操作，让信号量为零，完成任务处理后释放型号量
        *  上文建立信号量，第一次进循环后被P，所以不会阻塞
        *  接下来其他程序P操作后程序凋亡会自动释放信号量
        *  所以在此处再一次进行P操作，确保其他程序在处理完task
        *  之后才能进行P，从而实现同步。而且每个程序进行
        *  进行共享内存操作前都经过sem_req_shm的同步。
        */
        semaphore_p(sem_return);
        //等待cgi写入任务
        semaphore_p(sem_task);
        memset(buffer,0,sizeof(buffer));
        if (strlen(shmBuf) != 0) {
            strcpy(buffer,shmBuf);
            int res = analysis(buffer);
            if(res == SHM_TASK_TRTURN_OK){
                bzero(shmBuf,SHM_MAX);
                strcpy(shmBuf,SHM_CHECK_OK);
//                printf("send:%s\n",shmBuf);
            }
//            printf("recv:%s\n",buffer);

        }else{
            strcpy(shmBuf,"0");
        }
        /*释放处理结果信号量，信号量会按顺序存在链表内。此处释放会
        * 被外部先进行等待的P操作先截获，而不是本代码的上文的P截获。
        * sem_req_shm的存在使得只有一个外部程序的P和上文的P争抢
        * 信号量。同步是安全的
        */
        semaphore_v(sem_return);
    }

    return 0;
}

int semaphore_p(int semId)
{
    //对信号量做减1操作，即等待P（sv）
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1;//P()
    sem_b.sem_flg = SEM_UNDO;
    if(semop(semId, &sem_b, 1) == -1){
        fprintf(stderr, "semaphore_p failed\n");
        return 0;
    }
    return 1;
}

int semaphore_v(int semId)
{
    //对信号量做加1操作，即释放信号量V（sv）
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1;//P()
    sem_b.sem_flg = SEM_UNDO;
    if(semop(semId, &sem_b, 1) == -1){
        return 0;
    }
    return 1;

}

int getSemId(char *path,char ch)
{
    key_t key = ftok(path,ch);
    int semId = semget(key,1,0666  | IPC_CREAT);
    if (semId < 0) {
        return semId;
    }else{
        semctl(semId,0,IPC_RMID,0);
        semId = semget(key,1,0666  | IPC_CREAT);
    }
    return semId;
}

int analysis(char *buf)
{
    char *p = buf;
    if (0 != memcmp(p,"reg:",strlen("reg:"))) {
        return 0;
    }
    p += strlen("reg:");
    char id[10] = {'\0'};
    char name[30] = {'\0'};
    char pwd[20] = {'\0'};
    int i = 0;
    while (*p != ';') {
        id[i++] = *p++;
    }
    if (*p++ != ';') {
        return 0;
    }
    i = 0;
    while (*p != ';') {
        name[i++] = *p++;
    }
    if (*p++ != ';') {
        return 0;
    }
    i = 0;
    while (*p != ';') {
        pwd[i++] = *p++;
    }
    if (*p++ != ';') {
        return 0;
    }

    char tempBuf[128] = {'\0'};
    //创建以id为名字的文件集。
    sprintf(tempBuf,"%s/%s",USER_CONF_PATH,id);
    if(0 != mkdir(tempBuf,0777)){
        return 0;
    }
    //创建用户配置文本和设备状态上报文本。
    memset(tempBuf,'\0',sizeof(tempBuf));
    sprintf(tempBuf,"%s/%s/%s",USER_CONF_PATH,id,STATUS);
    if (creat(tempBuf,0777) < 0) {
        return 0;
    }
    memset(tempBuf,'\0',sizeof(tempBuf));
    sprintf(tempBuf,"%s/%s/%s",USER_CONF_PATH,id,CONF);
    if (creat(tempBuf,0777) < 0) {
        return 0;
    }
    int fd = open(tempBuf,O_WRONLY);
    if (fd < 0) {
        return 0;
    }
    memset(tempBuf,'\0',sizeof(tempBuf));
    sprintf(tempBuf,"usr:%s\npwd:%s\n",name,pwd);

    if (-1 != write(fd,tempBuf,strlen(tempBuf))) {
        close(fd);
        return SHM_TASK_TRTURN_OK;
    }else{
        close(fd);
        return 0;
    }


}
