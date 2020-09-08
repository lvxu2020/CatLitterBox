#include "netMonitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


NetMonitor::NetMonitor()
{
    //ping 5秒一次监控联网状态，连接的wifi名称
    monitorPtr = new QTimer(this);
    monitorPtr->setInterval(5000);
//    monitorPtr->setSingleShot(true);
    connect(monitorPtr,SIGNAL(timeout()),this,SLOT(slot_monitor()));
    connect(this,SIGNAL(monitorTimerStart()),this,SLOT(slot_monitorTimerStart()));
    connect(this,SIGNAL(monitorTimerStop()),this,SLOT(slot_monitorTimerStop()));
}

bool NetMonitor::getNetStatus()
{
    return netIsOK;
}

int NetMonitor::ping(std::string str_ip)
{
    const char *ip = str_ip.c_str();
    int i, status;
        pid_t pid;

        // 不同则循环检测多次
        for (i = 0; i < 1; ++i)
        {
            // 新建一个进程来执行ping命令
            if ((pid = vfork()) < 0)
            {
                printf("vfork error");
                continue;
            }

            if (pid == 0)
            {
/* **********************************************************************************************
 *  执行ping命令，第一个参数是命令的路径，嵌入式程序有可能更改环境变量
    /bin/ping  这个是ping的绝对路径。
    execlp()会从PATH 环境变量所指的目录中查找符合参数file的文件名，找到后便执行该文件，
    然后将第二个以后的参数（第三个参数）当做该文件的argv[0]、argv[1]……，最后一个参数必须用空指针(NULL)作结束。
    如果用常数0来表示一个空指针，则必须将它强制转换为一个字符指针，否则将它解释为整形参数，如果一个整形数的长度与
    char * 的长度不同，那么exec函数的实际参数就将出错。如果函数调用成功,进程自己的执行代码就会变成加载程序的代
    码,execlp()后边的代码也就不会执行了.
    返回值：如果执行成功则函数不会返回，执行失败则直接返回-1，失败原因存于errno 中
    参数2
    是该条命令在进程中的名字。比如，ping一个不存在的网址。这条命令会存活一段时间，这是  ps -aus | grep 参数二
    就能查看到他的进程id等信息。

********************************************************************************************** */

                if (execlp("ping", "ping", "-c 1", ip, (char*)0) < 0)
                {
                    printf("execlp error\n");
                    exit(1);
                }
            }

            waitpid(pid, &status, 0);

            // 相等说明正常
            if (status == 0)
                return 0;

        //    sleep(1);
        }

        return -1;
}

void NetMonitor::slot_monitor()
{
    bool res;
    if (ping(BAIDU_IP) < 0) {
        res = false;
    }else {
        res = true;
        // 测试mqtt用的
//        MqttSend test;
//        printf("wo neng da yin\n");
//        test.setInit("192.168.1.151","1883",60,1);
//        if (0 != test.connectMqtt()) {
//            printf("lian jie  shi bai tie zi\n");
//        }else{
//            printf("connectMqtt chenggong\n");
//
//            test.sendMess("cToS","cTos:127;0;luxuNB;",0,0,100L);
//        }

    }
    netIsOK = res;
    sig_netSatusChange(netIsOK);
}

void NetMonitor::slot_monitorTimerStart()
{
    monitorPtr->start();
}

void NetMonitor::slot_monitorTimerStop()
{
    monitorPtr->stop();
}

bool NetMonitor::getNetConnection()
{
    return netIsOK;
}
