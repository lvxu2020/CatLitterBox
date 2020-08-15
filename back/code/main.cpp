#include <iostream>
#include "Debug/Debug.h"
#include "Json/jsonAdapter.h"
#include "MyDbus/DbusAdapter.h"
#include <unistd.h>

int main()
{
    Node root;
    std::string a("test"),b("chenggong"),c;

    jsonAdapter::addValueToNode(root, a.c_str(), b);
    Node node1,node2,node3;
    jsonAdapter::addValueToNode(node1, "nb", "lvxu");
    jsonAdapter::addNodeToNode(root,"fenzu",node1);
    jsonAdapter::getStrFromNode(node1,c);
    printf("first json:\n%s\n", c.c_str());
    jsonAdapter::getStrFromNode(root,c);
    printf("first json:\n%s\n", c.c_str());

    DEBUG("hello world!");
    char * jsonStr  = "{\"semantic\":{\"slots\":{\"name\":\"张三\"}}, \"rc\":8, \"operation\":\"CALL\", \"service\":\"telephone\", \"text\":\"打电话给张三\"}";
    cJSON * rot = NULL;
    cJSON * item = NULL;//cjson对象
    rot = cJSON_Parse(jsonStr);
    Node node;
    node<<jsonStr;
    item = cJSON_GetObjectItem(rot, "operation");
    std::string in("/semantic/slots/name/");
    std::string testStr = jsonAdapter::parseNode(node,jsonStr);
    printf("test:::: %s*****\n",testStr.c_str());

    DbusAdapter bus;
    if (bus.sendASignal("/test/signal/Object","test.signal.Type","Test","lvxuNB")) {
        printf("send succeed\n");
    }

    return 0;
}
