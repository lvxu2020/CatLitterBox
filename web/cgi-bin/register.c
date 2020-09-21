#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <strings.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>

#define TABLE "USER_PASS"
#define USER_DB_PATH "/home/lvxu/catServer/sqlite/user.db"
#define SHM_MAX 128
#define SHM_CHECK_OK "yes"


bool found = false;

int semaphore_p(int semId);
int semaphore_v(int semId);

/* ***********
 * 查找特定元素
 * ***********/
int find(void *para,int ncolumn, char ** columnvalue,char *columnname[])
{
    int i;
    for(i = 0; i < ncolumn; i++){
        if(strcmp(columnvalue[i],(char*)para) == 0){
            found = true;
        }
    }
    return 0;
}

/* ***********
 * 用户名已被注册
 * ***********/
void usrRegistered()
{
    printf("<H3>用户名已被注册！</h3>");
    printf("<meta http-equiv=\"Refresh\" content=\"5;URL=/register.html\">");
}

/* ***********
 * 新用户添加
 * ***********/
bool newUsr(char *name, char *pwd, char *num)
{
    char file[100] = {0};
    char *shmBuf = NULL;
    sprintf(file,"reg:%s;%s;%s;",num,name,pwd);
    //申请操作共享内存信号量
    key_t key = ftok("/",'a');
    int sem_req_shm = semget(key,1,O_RDWR);
    if(sem_req_shm < 0){
        return false;
    }
    key = ftok("/bin",'b');
    int sem_task = semget(key,1,O_RDWR);
    if(sem_task < 0){
        return false;
    }
    key = ftok("/bin",'c');
    int sem_return = semget(key,1,O_RDWR);
    if(sem_return < 0){
        return false;
    }
    //P操作 防止其他cgi程序同时操作共享内存。
    semaphore_p(sem_req_shm);


    //打开共享内存
    key = ftok("/bin",'d');
    int shmid = shmget(key,0,0);
    if(shmid < 0)
    {
        return false;
    }
    shmBuf = shmat(shmid,NULL,0);
    if(shmBuf == NULL)
    {
        return false;
    }

    //写入命令释放信号量
    bzero(shmBuf,SHM_MAX);
    strcpy(shmBuf,file);
    semaphore_v(sem_task);
    //等待服务器端返回处理结果
    semaphore_p(sem_return);
    char recvBuf[10] = {0};
    strcpy(recvBuf,shmBuf);
    if ( strcmp(recvBuf,SHM_CHECK_OK) ) {
        return false;
    }
    //V操作 V操作释放掉信号量。程序消亡会自动释放。但我们是好孩子，自己动手
    semaphore_v(sem_req_shm);
}

/* ***********
 * 截取帐号密码设备号
 * ***********/
bool getNamePwdNum(const char *data, int len, char *name, char *pwd, char *num)
{
    int i;
    bool find_name = false,find_pwd = false,find_num = false;
    for (i=0; i < len; i++) {
        if (data[i] == '"' ) {
            if (!find_num && 0 == strncmp(data+(i+1),"number",6) ) {
                int j = 0;
                for (;;j++) {
                    if(data[i+12+j] == 13 || j > 9){
                        num[j]  = '\0';
                        find_num = true;
                        break;
                    }
                    num[j] = (char)data[i+12+j];

                }
                i += j;
            }

            if (!find_name && 0 == strncmp(data+(i+1),"username",8) ) {
                int j = 0;
                for (;;j++) {
                    if(data[i+14+j] == 13 || j > 49){
                        name[j]  = '\0';
                        find_name = true;
                        break;
                    }
                    name[j] = (char)data[i+14+j];

                }
                i += j;
            }

            if ( !find_pwd && 0 == strncmp(data+i+1,"password",8) ) {
                int k = 0;
                for (;;k++) {

                    if(data[i+14+k] == 13 || k > 19){
                        pwd[k] = '\0';
                        find_pwd = true;
                        break;
                    }
                    pwd[k] = data[i+14+k];
                }
                return true;
            }
        }
    }
    return false;

}

int main(int argc, char* argv[])
{

    //web
    size_t i = 0,n = 0;
    char *method = NULL;
    char name[50],pwd[20],num[10];


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
            getNamePwdNum(inputdata,length,name,pwd,num);
            //读出帐号密码
            free(inputdata);
        }
    }else if (getenv("QUERY_STRING") && strcmp(method,"GET") == 0){
        //环境变量的长度有限,导致GET方法传送数据被限制
        char * inputdata = getenv("QUERY_STRING");
        if (inputdata == NULL){
            printf("<p>错误：数据没有被输入或数据传输发生错误</p>");
        }else{
         //   sscanf(inputdata,"username=%[^&]&password=%s",name,pwd);
        }

    }else{
          printf("%s\n","bad request!");
          return 0;
    }




    //数据库
    sqlite3 *db;
    int res;
    char *errmsg = NULL;
    char **result;
    int row,col;
    const char *sqlcmd;
    char insert[128];

    //打开数据库
    res = sqlite3_open(USER_DB_PATH, &db);
    if(res != SQLITE_OK){
        printf( "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }

    //校验数据库中，是否重名
    sqlcmd = "SELECT * FROM USER_PASS";
    res = sqlite3_exec(db,sqlcmd,find,(void*)name,&errmsg);//查询数据
    if(res!=SQLITE_OK){
        printf("数据库操作失败：%d-%s\n",res,errmsg);
        sqlite3_close(db);
        return 0;
    }

    if(found){
        usrRegistered();
    }else{//添加进数据库
        if (newUsr(name,pwd,num)){
            memset((void*)insert,0,sizeof(insert));
            sprintf(insert,"INSERT INTO %s VALUES ('%s','%s','%s') ;",TABLE,num,name,pwd);
            sqlcmd = insert;
            res = sqlite3_exec(db,sqlcmd,NULL,0,&errmsg);//插入数据
            if(res!=SQLITE_OK){
                printf("数据库操作失败：%d-%s\n",res,errmsg);
                sqlite3_close(db);
                return 0;
            }else{
                printf("<H3>注册成功，稍候返回登录界面</h3>");
                printf("<meta http-equiv=\"Refresh\" content=\"5;URL=/main.html\">");
            }

        }else{
            printf("<H3>开创用户目录失败，稍候返回注册界面</h3>");
            printf("<meta http-equiv=\"Refresh\" content=\"5;URL=/register.html\">");
            sqlite3_close(db);
            return 0;

        }

    }

    sqlite3_close(db);
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
        fprintf(stderr, "semaphore_p failed\n");
        return 0;
    }
    return 1;

}

