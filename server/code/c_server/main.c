#include <stdio.h>
#include <pthread.h>
#include <getopt.h>
#include "Debug/Debug.h"
#include "Config/config.h"

/* ***********************************
 * 解析命令行参数
 * *********************************** */
int parseComLine(int argc, char * argv[])
{
    const struct option long_options[] = {
        {"debug", 0, NULL, 'd'},
        {"info", 0, NULL, 'i'},
        {"warn", 0, NULL, 'w'},
        {"error", 0, NULL, 'e'}
    };

    int opt = 0;
    while ((opt = getopt_long(argc, argv, "diwe", long_options, NULL)) != -1) {
        switch(opt) {
        case 'd':
            config = 4;
            printf("debug level: %d\n", config);
            break;
        case 'i':
            config = 3;
            printf("debug level: %d\n", config);
            break;
        case 'w':
            config = 2;
            printf("debug level: %d\n", config);
            break;
        case 'e':
            config = 1;
            printf("debug level: %d\n", config);
            break;
        default:
            //非法参数处理，也可以使用case来处理，？表示无效的选项，：表示选项缺少参数
            printf("Argument error! \n");
            break;
        };
    }
    return 0;
}


int main(int argc, char * argv[])
{
    // 解析命令行参数
    if (parseComLine(argc, argv) < 0) {
        exit(1);
    }
    printf("server start  !!!\n");
    if (!config_init())
    {
        printf("初始化失败，请检查配置文件\n");
    }
//    pthread_t reg_id,ctl_id,mqtt_rec_id,mqtt_send_id;
//    //注册用户
//    pthread_create(&reg_id,NULL,registerUsr,NULL);
//    //远程控制命令
//    pthread_create(&ctl_id,NULL,remoteCmd,NULL);
//    //mqtt操作
//    pthread_create(&mqtt_rec_id,NULL,mqttMsgRec,NULL);
//    pthread_create(&mqtt_send_id,NULL,mqttMsgSend,NULL);

//    pthread_join(reg_id,NULL);
//    pthread_join(ctl_id,NULL);
//    pthread_join(mqtt_send_id,NULL);
//    pthread_join(mqtt_rec_id,NULL);
    return 0;

}
