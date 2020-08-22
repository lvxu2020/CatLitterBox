#include <iostream>
#include "Debug/Debug.h"
#include "Json/jsonAdapter.h"
#include "MyDbus/DbusAdapter.h"
#include "MyDbus/DbusReceive.h"
#include <unistd.h>

void testFun()
{
    DbusAdapter bus;
    bus.setDbusWellKnownName("lvxu.zhende.nb");
    if (bus.sendASignal("/","test.signal.Type","Test","lvxuNB")) {
        printf("send succeed\n");
    }
}

int main()
{
    Node root;
    std::string a("test"),b("chenggong"),c;

    jsonAdapter::addValueToNode(root, a.c_str(), b);
    Node node1,node2,node3;
    jsonAdapter::addValueToNode(node1, "nb", "lvxu");
    jsonAdapter::addNodeToNode(root,"fenzu",node1);
    jsonAdapter::getStrFromNode(node1,c);
    printf("node json:\n%s\n", c.c_str());
    jsonAdapter::getStrFromNode(root,c);
    printf("root json:\n%s\n", c.c_str());

    DEBUG("hello world!");
    char * jsonStr  = "{\"semantic\":{\"slots\":{\"name\":\"张三\"}}, \"rc\":8, \"operation\":\"CALL\", \"service\":\"telephone\", \"text\":\"打电话给张三\"}";
    cJSON * rot = NULL;
    cJSON * item = NULL;//cjson对象
    rot = cJSON_Parse(jsonStr);
    Node node(jsonStr);
    item = cJSON_GetObjectItem(rot, "operation");
    std::string in;
    jsonAdapter::getStrFromNode(node,c);
    printf("test root: %s\n",c.c_str());
    std::string testStr = jsonAdapter::parseNode(node,"semantic/slots/name");
    printf("test1:::: %s*****\n",testStr.c_str());
    testStr = jsonAdapter::parseNode(node,"semantic/rc");
    printf("test2:::: %s*****\n",testStr.c_str());
    testFun();

    DbusReceive::instance()->addListenSig("lvxu.cheshi.nb","xiaoXinHao");
    DbusReceive::instance()->start();

    DbusReceive::instance()->join();
    return 0;
}
