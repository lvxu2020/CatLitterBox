#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define TABLE "USER_PASS"
#define USER_DB_PATH "/home/lvxu/catServer/sqlite/user.db"


bool checked = false;

/* ***********
 * 查找特定元素
 * ***********/
int check(void *para,int ncolumn, char ** columnvalue,char *columnname[])
{
    int i;
    char (*p)[50] = (char(*)[50])para;
    if( 0 == strcmp(columnvalue[0],p[0]) && 0 == strcmp(columnvalue[1],p[1]) && 0 == strcmp(columnvalue[2],p[2]) ){
        checked = true;
    }
    return 0;
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

int main() 
{   
    //web
    size_t i = 0,n = 0;
    char *method = NULL;
    char num[10],name[50],pwd[20];
    printf("content-type:text/html;charset=utf-8\n\n");  
    printf("<TITLE>登陆结果</TITLE>");  
    printf("<script src=\"js/jquery-3.0.0.min.js\"></script>");
    printf("<script src=\"js/jquery.params.js\"></script>");
//    printf( "<script>function show(){var date =new Date(); var now = \"\"; now = date.getFullYear()+\"年\"; now = now + (date.getMonth()+1)+\"月\"; \ now = now + date.getDate()+\"日\"; now = now + date.getHours()+\"时\"; now = now + date.getMinutes()+\"分\";now = now + date.getSeconds()+\"秒\"; document.getElementById(\"nowDiv\").innerHTML = now; setTimeout(\"show()\",1000);} </script> \n ");
//    printf( "<h2><font face=\"Broadway\"><font color=\"#00FAF0\">Home1 Real-time Environment Info:</font></font></H2>\n ");
//    printf( "<h2 align=center><font color=\"#cc0033\"><body onload=\"show()\"> <div id=\"nowDiv\"></div></font></h2> \n ");

    if (NULL == (method = getenv("REQUEST_METHOD"))){
        return 0;
    }

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
            //读出帐号密码
            getNamePwdNum(inputdata,length,name,pwd,num);
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
    char  account[3][50] = {0};
    strcpy(account[0],num);
    strcpy(account[1],name);
    strcpy(account[2],pwd);
    //打开数据库
    res = sqlite3_open(USER_DB_PATH, &db);
    if(res != SQLITE_OK){
        printf( "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }

    //校验数据帐号密码
    sqlcmd = "SELECT * FROM USER_PASS";
    res = sqlite3_exec(db,sqlcmd,check,(void*)account,&errmsg);//查询数据
    if(res!=SQLITE_OK){
        printf("数据库操作失败：%d-%s\n",res,errmsg);
        sqlite3_close(db);
        return 0;
    }
    if(checked){
        printf("<H3>登录成功</h3>");
        printf("<meta http-equiv=\"Refresh\" content=\"1;URL=/control.html?num=%s\">",num);
    }else{
        printf("<H3>帐号密码错误</h3>");
        printf("<meta http-equiv=\"Refresh\" content=\"2;URL=/main.html\">");
    }

    return 0; 
}
