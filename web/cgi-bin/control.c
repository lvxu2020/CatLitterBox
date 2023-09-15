#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/ipc.h>
#include <strings.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <unistd.h>
#include <fcntl.h>

#define COMMAND_MAX 50
#define MQ_KEY_PATH "/lib"
#define MQ_KEY_CHAR 'A'
#define MQ_MSGBUF_LEN 50
#define NUM 10
#define REMOTE_HEAD "radio"

/********命令种类**********
 * 字符串格式：（分号“；”是格式内一部分）
 * ；设备号；命令id；命令值；
 * ******命令种类 end ****/
#define CMD_CHANSHI_ID 1

typedef struct mqbuf
{
        long id;
        char msg[MQ_MSGBUF_LEN];
}MSG;


typedef struct
{
    char type[NUM];
    char value[24];
}CMD;


/* ***********
 * 获取命令
 * ***********/
bool getCommand(char * data,char *cmd,int len ,char *num)
{
    // 表单：radio=led_on&num=1&button1=%E6%8F%90%E4%BA%A4
    char *p = data;
    char choose[15] = {0};
    if (0 == memcmp(p,REMOTE_HEAD,strlen(REMOTE_HEAD))) {
        // 加上等于号
        p += strlen(REMOTE_HEAD) + 1;
        int i = 0;
        while (*p != '&') {
            choose[i++] = *p++;
        }
    } else {
        return false;
    }

    if (0 == memcmp(p,"&num",strlen("&num"))) {
        // 加上等于号
        p += strlen("&num") + 1;
        int i = 0;
        while (*p != '&') {
            num[i++] = *p++;
        }

    } else {
        return false;
    }

    int cmd_1 = -1, cmd_2 = -1;
    if (choose[0] == 'l') {
        if (choose[5] == 'n') {
            cmd_1 = 1;
            cmd_2 = 1;
        }else if (choose[5] == 'f'){
            cmd_1 = 1;
            cmd_2 = 0;
        }
    } else if (choose[0] == 'm') {
        if (choose[7] == 'n') {
            cmd_1 = 0;
            cmd_2 = 1;
        }else if (choose[7] == 'f'){
            cmd_1 = 0;
            cmd_2 = 0;
        }
    }
    if (cmd_1 < 0 || cmd_2 < 0) {
        return false;
    }
    sprintf(cmd,";%s;%d;%d;",num , cmd_1, cmd_2);
    return true;
}

/* ***********
 * 任务处理
 * ***********/
int processTask(char *cmd, char *num)
{

    key_t key = ftok(MQ_KEY_PATH,MQ_KEY_CHAR);
    int msgid = msgget(key,O_RDWR);
    if (msgid <= 0){
        printf("服务器消息队列崩溃！\n");
        return 0;
    }
    MSG buf;
    bzero(&buf,sizeof(buf));
    buf.id = (long)atoi(num);
    strncpy(buf.msg,cmd,strlen(cmd)+1);
    msgsnd(msgid,&buf,MQ_MSGBUF_LEN,0);
//    printf("send msg type:%ld,msg:%s\n",buf.id,buf.msg);
    return 1;

}

int main()
{
    //web
    size_t i = 0,n = 0;
    char num[10] = {'\0'};
    char *method = NULL;
    char command[COMMAND_MAX] = {'\0'};
    //获取HTTP请求方法(POST/GET)
    if (NULL == (method = getenv("REQUEST_METHOD")))
    {
        return 0;
    }
    printf("content-type:text/html;charset=utf-8\n\n");
    printf("<TITLE>注册结果</TITLE>");
    //POST 方法解析,从 STDIN_FILENO 动态获取数据
    if (getenv("CONTENT_LENGTH") && strcmp(method,"POST") == 0){
        n = atoi(getenv("CONTENT_LENGTH"));
        size_t length = n * sizeof(char) + 1;
        //必须申请缓存，因为stdin是不带缓存的。
        char * inputdata = (char *)malloc(length);
        if(inputdata){
            memset((void*)inputdata,0,length);
            //从标准输入读取一定数据
            fread(inputdata, sizeof(char), length, stdin);
            //获取命令
            if(getCommand(inputdata,command,length,num)){
                processTask(command,num);
            }else{
                printf("任务解析失败\n");
            }
            free(inputdata);
        }
    }else if (getenv("QUERY_STRING") && strcmp(method,"GET") == 0){
        //环境变量的长度有限,导致GET方法传送数据被限制
        char * inputdata = getenv("QUERY_STRING");
        if (inputdata == NULL){
            printf("<p>错误：数据没有被输入或数据传输发生错误</p>");
        }else{
            int length = strlen(inputdata);
//           printf("+++html data len = %d++%s+++",strlen(inputdata),inputdata);
            //获取命令
            if(getCommand(inputdata,command,length,num)){
                printf("命令已发出！");
                processTask(command, num);
            }else{
                printf("任务解析失败\n");
            }

         //   sscanf(inputdata,"username=%[^&]&password=%s",name,pwd);
        }

    }else{
          printf("%s\n","bad request!");
          return 0;
    }

    printf("<meta http-equiv=\"Refresh\" content=\"3;URL=/control.html?num=%s\">",num);

}
