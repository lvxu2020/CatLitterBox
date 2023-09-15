#include <stdio.h>
#include <pthread.h>
#include <getopt.h>
#include "Debug/Debug.h"
#include "Config/config.h"
#include "Register/register.h"
#include "Control/control.h"
#include "Mqtt/mqttSend.h"
#include "Mqtt/mqttReceive.h"
#include "Task/pool.h"

threadpool_t *pool = NULL;

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
        exit(1);
    }

    // 初始化mqtt发送
    INIT_SEND_PRAM pram;
    bzero(&pram,sizeof (INIT_SEND_PRAM));
    pram.qos = 0;
    memcpy(pram.pub_id, "qmtt_send", sizeof("qmtt_send"));
    pram.timeout = 1000L;
    pram.retained = 0;
    sprintf(pram.address, "tcp://%s:%s", config_arr[mqtt_ip], config_arr[mqtt_port]);
    pram.cleansession = 1;
    pram.keepAliveInterval = 60;
    if (!qmtt_send_init(&pram)) {
        DEBUG_E("mqtt init fail");
        exit(1);
    }

    // 初始化mqtt接收
    INIT_RECV_PRAM pram1;
    bzero(&pram1, sizeof(INIT_RECV_PRAM));
    pram1.qos = 0;
    pram1.keepAliveInterval = 20;
    pram1.cleansession = 1;
    pram1.timeout = 5000L;
    memcpy(pram.pub_id, "qmtt_recv", sizeof("qmtt_send"));
    sprintf(pram1.address, "tcp://%s:%s", config_arr[mqtt_ip], config_arr[mqtt_port]);
    strcpy(pram1.topic,"cToS");
    if (!qmtt_recv_init(&pram1)) {
        DEBUG_E("mqtt init fail");
        exit(1);
    }

    pool = threadpool_create(POOL_PTH_MIN,POOL_PTH_MAX,POOL_QUEUE_MAX);
    sleep(1);
    if (pool == NULL) {
        DEBUG_E("pool creat fail");
        exit(1);
    }

    pthread_t reg_id, ctl_id;
    //注册用户
    pthread_create(&reg_id,NULL,register_usr,NULL);
    //远程控制命令
    pthread_create(&ctl_id,NULL,remote_control,NULL);

    pthread_join(reg_id,NULL);
    pthread_join(ctl_id,NULL);
    return 0;

}
