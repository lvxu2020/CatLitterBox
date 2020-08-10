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
    static bool addValueToNode(Node&, const char*, const bool&);
    static bool addValueToNode(Node&, const char*, const int&);
    static bool addValueToNode(Node&, const char*, const std::string&);
    static bool addValueToNode(Node&, const char*, const double&);
    static bool addValueToNode(Node&, const char*, const char* );
    //参数：根节点，字节点
    static bool addNodeToNode(Node&, const char*, Node&);
    //参数：节点，受理字符串
    static void getStrFromNode(const Node&, std::string&);
    static void getUnFormatStrFromNode(const Node&, std::string&);
    // 参数：解析节点，节点路径；返回：失败返回空字串。字符串格式 输入字串： china/LiaoNing/DaLian；
    static std::string parseNode(const Node&, const std::string&);
    static std::string parseNode(const Node&, const char*);
private:
    jsonAdapter();
    ~jsonAdapter();
};



#endif // JSONADAPTER_H
