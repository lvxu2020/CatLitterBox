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
#define MQ_MSGBUF_LEN 3
#define NUM 10

#define COMMAND_MAX 1

typedef struct mqbuf
{
        long type;
        char msg[MQ_MSGBUF_LEN];
}MSG;

int main()
{

    //web
    size_t i = 0,n = 0;
    char *method = NULL;
    char command[COMMAND_MAX][50] = {0};
    char num[10] = {0};
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

            free(inputdata);
        }
    }else if (getenv("QUERY_STRING") && strcmp(method,"GET") == 0){
        //环境变量的长度有限,导致GET方法传送数据被限制
        char * inputdata = getenv("QUERY_STRING");
        if (inputdata == NULL){
            printf("<p>错误：数据没有被输入或数据传输发生错误</p>");
        }else{
            int length = strlen(inputdata);
            sscanf(inputdata,"num=%s",num);
            key_t key = ftok(MQ_KEY_PATH,MQ_KEY_CHAR);
            int msgid = msgget(key,O_RDWR);
            if (msgid <= 0){
                printf("消息队列系统故障\n");
            }else{
                MSG buf;
                char sendBuf[3] = {"1;"};
                bzero(&buf,sizeof(buf));
                buf.type = atol(num);
                strcpy(buf.msg,sendBuf);
                msgsnd(msgid,&buf,MQ_MSGBUF_LEN,0);
                printf("+status.cgi  len = %d+buf+%s+++",strlen(inputdata),inputdata);

            }

        }

    }else{
          printf("%s\n","bad request!");
          return 0;
    }
    printf("<meta http-equiv=\"Refresh\" content=\"6;URL=/status.html?num=%s\">",num);
}
