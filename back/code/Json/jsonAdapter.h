#ifndef JSONADAPTER_H
#define JSONADAPTER_H

#include "cJSON.h"
#include <string>
#include "Node.h"
#include <vector>
#include <iterator>
#include <regex>



class Node;

class jsonAdapter
{
public:
    //参数：输入节点，添加内容名字，添加内容；返回：成功or失败
    static bool addValueToNode(Node& node, const char* name, const bool& bl);
    static bool addValueToNode(Node& node, const char* name, const int& number);
    static bool addValueToNode(Node& node, const char* name, const std::string& str);
    static bool addValueToNode(Node& node, const char* name, const double& foatNum);
    static bool addValueToNode(Node& node, const char* name, const char* str);
    //参数：根节点，字节点
    static bool addNodeToNode(Node& root, const char* name, Node& node);
    //参数：节点，受理字符串
    static void getStrFromNode(const Node& node, std::string& str);
    static void getUnFormatStrFromNode(const Node& node, std::string& str);
    // 参数：解析节点，节点路径；返回：失败返回空字串。字符串格式 输入字串： china/LiaoNing/DaLian；
    static std::string parseNode(const Node& node, const std::string& in);
    static std::string parseNode(const Node& node, const char* in);
private:
    jsonAdapter();
    ~jsonAdapter();
};



#endif // JSONADAPTER_H
