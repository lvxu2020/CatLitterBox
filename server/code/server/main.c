#include "stdio.h"
#include <pthread.h>
#include "./Register/register.h"
#include "./MqttMsg/mqttMsg.h"
#include "./Control/control.h"


int main()
{
    printf("server start !!!\n");
    pthread_t reg_id,ctl_id,mqtt_rec_id,mqtt_send_id;
    //注册用户
    pthread_create(&reg_id,NULL,registerUsr,NULL);
    //远程控制命令
    pthread_create(&ctl_id,NULL,remoteCmd,NULL);
    //mqtt操作
    pthread_create(&mqtt_rec_id,NULL,mqttMsgRec,NULL);
    pthread_create(&mqtt_send_id,NULL,mqttMsgSend,NULL);

    pthread_join(reg_id,NULL);
    pthread_join(ctl_id,NULL);
    pthread_join(mqtt_send_id,NULL);
    pthread_join(mqtt_rec_id,NULL);

}
