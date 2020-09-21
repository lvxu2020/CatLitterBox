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

/********命令种类**********
 * 字符串格式：（分号“；”是格式内一部分）
 * ；设备号；命令id；命令值；
 * ******命令种类 end ****/
#define CMD_CHANSHI_ID 1

typedef struct mqbuf
{
        long type;
        char msg[MQ_MSGBUF_LEN];
}MSG;

enum remoteCmd
{
    status = 0,
    chanshi,
    remoteCmdMax
};

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
    char *p = data;
    char cmdNum[NUM] = {0};
    bool getCmd = false;
    unsigned char rmtCmd[remoteCmdMax];
    int i = 0;
    for(;i < remoteCmdMax; i++){
        rmtCmd[i] = 0x0;
    }
    if(len > 0){
        while (*p != '\0') {
            if(0 == rmtCmd[chanshi] && *p == 'c'){//铲屎
                if (0 == memcmp(p,"chanshi=start",strlen("chanshi=start") )) {
                    p += strlen("chanshi=start");
                }
                rmtCmd[chanshi] = 0x01;
                continue;
            }

            if (!getCmd && *p == 'n') {
                if (0 == memcmp(p,"num=",strlen("num="))) {
                    p += strlen("num=");
                    int i = 0;
                    while(*p != '&' && *p != '\0' && i < NUM){
                        cmdNum[i] = *p;
                        i++;
                        p++;
                    }
                    strcpy(num,cmdNum);
                    getCmd = true;

                    continue;
                }
            }
            p++;
        }

    }
    if (getCmd) {
        //停留在此页面状态肯定需要上报
        rmtCmd[status] = 1;
        sprintf(cmd,"%s;",cmdNum);
        for (i = 0; i < remoteCmdMax; i++){
            //保证一位代表枚举的一个命令
            sprintf(cmd + (strlen(cmd)),"%d%c",rmtCmd[i] % 9,'\0');
        }
        sprintf(cmd + (strlen(cmd)),";%c",'\0');
    }
    return getCmd;
}

bool getSendMsg(CMD *cmd,char *data)
{
    if(strlen(data) > 50){
        return false;
    }

    char *p = data;
    int i = 0;
    while(*p != ';' && i < sizeof(cmd->type)){
        cmd->type[i] = *(p);
        p++;
        i++;
    }
    cmd->type[i + 1] = '\0';
    i = 0;
    p++;
    while(*p != ';' && i < sizeof(cmd->value)){
        cmd->value[i] = *(p);
        p++;
        i++;
    }
    cmd->value[i] = ';';
    cmd->value[i+1] = '\0';
    return true;
}


/* ***********
 * 任务处理
 * ***********/
int processTask(char *cmd)
{

    key_t key = ftok(MQ_KEY_PATH,MQ_KEY_CHAR);
    int msgid = msgget(key,O_RDWR);
    if (msgid <= 0){
        printf("服务器消息队列崩溃！\n");
        return 0;
    }
    MSG buf;
    CMD command;
    if(getSendMsg(&command,cmd)){
        bzero(&buf,sizeof(buf));
        buf.type = atol(command.type);
        strcpy(buf.msg,command.value);
        msgsnd(msgid,&buf,MQ_MSGBUF_LEN,0);
        printf("send msg type:%ld,msg:%s\n",buf.type,buf.msg);
    }

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
                processTask(command);
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
            printf("+++html data len = %d++%s+++",strlen(inputdata),inputdata);
            //获取命令
            if(getCommand(inputdata,command,length,num)){
                printf("命令已发出！");
                processTask(command);
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
