#include <iostream>
#include "Debug/Debug.h"
#include "Json/JsonAdapter.h"
#include "MyDbus/DbusAdapter.h"
#include "MyDbus/DbusReceive.h"
#include "Beat/Beat.h"
#include "RunHmi/RunHmi.h"
#include <unistd.h>

void downTest()
{
    sleep(3);
    DbusAdapter bus;
    Node test,root;
    JsonAdapter::addValueToNode(root, "upLoadPath", "http://192.168.1.118/cgi-bin/test.cgi");
    JsonAdapter::addValueToNode(root, "id", "3");
    std::string str;

    JsonAdapter::getUnFormatStrFromNode(root,str);
    if (DbusAdapter::sendASignal("/","code.hmi","signal",str.c_str())) {
        printf("send succeed:%s\n",str.c_str());
    }else {
        printf("send fail:%s\n",str.c_str());
    }
    printf("testFun out\n");
}


void demo_fun()
{
    /******** Dbus监听信号 start ********/
    // 选择监听的信号可监听多个信号，一般来说跟同一个进程交互用一个信号就够了
    DbusReceive::instance()->addListenSig("code.hmi","signal");
    // 开启线程，循环监听
    DbusReceive::instance()->start();
    /******** Dbus监听信号 end ********/
}

int main()
{
    RunHmi hmi;
    DbusReceive::instance()->addListenSig("code.hmi","signal");
    DbusReceive::instance()->start();
    Beat::instance()->start();
    HardMaster::instance()->start();
    hmi.start();
    hmi.detach();
    Beat::instance()->join();
    DbusReceive::instance()->join();
    return 0;
}


//int main()
//{
//    Node root;
//    std::string a("test"),b("chenggong"),c;

//    JsonAdapter::addValueToNode(root, a.c_str(), b);
//    Node node1,node2,node3;
//    JsonAdapter::addValueToNode(node1, "nb", "lvxu");
//    JsonAdapter::addValueToNode(node1, "nb", "lvxu");
//    JsonAdapter::addNodeToNode(root,"fenzu",node1);
//    JsonAdapter::getStrFromNode(node1,c);
//    printf("node json:\n%s\n", c.c_str());
//    JsonAdapter::getStrFromNode(root,c);
//    printf("root json:\n%s\n", c.c_str());

//    DEBUG("hello world!");
//    char * jsonStr  = "{\"semantic\":{\"slots\":{\"name\":\"张三\"}}, \"rc\":8, \"operation\":\"CALL\", \"service\":\"telephone\", \"text\":\"打电话给张三\"}";
//    cJSON * rot = NULL;
//    cJSON * item = NULL;//cjson对象
//    rot = cJSON_Parse(jsonStr);
//    Node node(jsonStr);
//    item = cJSON_GetObjectItem(rot, "operation");
//    std::string in;
//    JsonAdapter::getStrFromNode(node,c);
//    printf("test root: %s\n",c.c_str());
//    std::string testStr = JsonAdapter::parseNode(node,"semantic/slots/name");
//    printf("test1:::: %s*****\n",testStr.c_str());
//    testStr = JsonAdapter::parseNode(node,"semantic/rc");
//    printf("test2:::: %s*****\n",testStr.c_str());


//    DbusReceive::instance()->addListenSig("code.hmi","signal");
//    DbusReceive::instance()->start();
//    Beat::instance()->start();
//    downTest();
//    Beat::instance()->join();
//    DbusReceive::instance()->join();
//    return 0;
//}
