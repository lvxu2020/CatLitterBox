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




int main()
{
    //web
    size_t i = 0,n = 0;
    char num[10] = {'\0'};
    char *method = NULL;
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
            char down[length];
            memcpy(down,inputdata,length);
            free(inputdata);
            if (creat("/home/lvxu/catServer/log/only_one.log",0777) < 0) {
                printf("wenjian shibai\n");
            }
            int fd = open("/home/lvxu/catServer/log/only_one.log",O_WRONLY);
            if (fd < 0) {
                return 0;
            }
            char len[10];
            sprintf(len,"%d\n",length);
            write(fd,len,10);
            write(fd,down,length);
            close(fd);

        }
    }    else{
          printf("%s\n","bad request!");
          return 0;
    }

}
