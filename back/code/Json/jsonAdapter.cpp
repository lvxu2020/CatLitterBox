#include "jsonAdapter.h"


void getValue(cJSON* p, std::string& str);
std::vector<std::string> mySplit(const std::string& in, const std::string& delim);

jsonAdapter::jsonAdapter()
{

}

jsonAdapter::~jsonAdapter()
{

}

bool jsonAdapter::addValueToNode(Node& node, const char* name, const bool& bl)
{
    if(cJSON_AddBoolToObject(node.m_root, name, bl)) {
        return true;
    }
    return false;
}

bool jsonAdapter::addValueToNode(Node& node, const char* name, const int& number)
{
    if(cJSON_AddNumberToObject(node.m_root, name, number)) {
        return true;
    }
    return false;
}

bool jsonAdapter::addValueToNode(Node& node, const char* name, const std::string& str)
{
    if(cJSON_AddStringToObject(node.m_root, name, str.c_str())) {
        return true;
    }
    return false;
}

bool jsonAdapter::addValueToNode(Node& node, const char* name, const char* str)
{
    if(cJSON_AddStringToObject(node.m_root, name, str)) {
        return true;
    }
    return false;
}

bool jsonAdapter::addValueToNode(Node& node, const char* name, const double& foatNum)
{
    if(cJSON_AddNumberToObject(node.m_root, name, foatNum)) {
        return true;
    }
    return false;
}

bool jsonAdapter::addNodeToNode(Node& root, const char* name, Node& node)
{
    char *p = cJSON_Print(node.m_root);
    cJSON* sun = cJSON_Parse(p);
    free(p);
    if (sun == nullptr) {
        return false;
    }
    cJSON_AddItemToObject(root.m_root, name, sun);
    if (root.m_root == nullptr) {
        return false;
    }
    return true;
}

void jsonAdapter::getStrFromNode(const Node& node, std::string& str)
{
    char *p = cJSON_Print(node.m_root);
    str = p;
    free(p);
}

void jsonAdapter::getUnFormatStrFromNode(const Node& node, std::string& str)
{
    char *p = cJSON_Print(node.m_root);
    str = p;
    free(p);
}

/*
 * 用delim指定的正则表达式将字符串in分割，返回分割后的字符串数组
 * delim 分割字符串的正则表达式
*/
std::vector<std::string> mySplit(const std::string& in, const std::string& delim) {
    std::regex re{ delim };
    // 调用 std::vector::vector (InputIterator first, InputIterator last,const allocator_type& alloc = allocator_type())
    // 构造函数,完成字符串分割
    return std::vector<std::string> {
        std::sregex_token_iterator(in.begin(), in.end(), re, -1),
        std::sregex_token_iterator()
    };
}

void getValue(cJSON* p, std::string& str)
{
    switch (p->type) {
    case cJSON_Number: {
        str = std::to_string(p->valuedouble);
    }
        break;
    case cJSON_String: {
        str = p->valuestring;
    }
        break;
    default : {
        str = "";
    }
        break;
    }

}

std::string jsonAdapter::parseNode(const Node& node, const std::string& in)
{
    if (node.m_root == nullptr) {
        return "";
    }
    std::string str(in);
    if (str[0] == '/') {
        str.erase(0,1);
    }
    std::vector<std::string> vec = mySplit(str,"/");
    if (vec.size() == 0 && str.size() > 0) {
        vec.push_back(str);
    }
    cJSON* p = node.m_root;
    for (int i = vec.size(); i > 0; i--) {
        if (i == 1){
            p = cJSON_GetObjectItem(p,vec.back().c_str());
            if (p == nullptr) {
                return "";
            }
            std::string out;
            getValue(p,out);
            return out;
        }else {
            p = cJSON_GetObjectItem(p,vec[vec.size() - i].c_str());
            if (p == nullptr) {
                return "";
            }
        }
    }
}

std::string jsonAdapter::parseNode(const Node& node, const char* in)
{
    if (node.m_root == nullptr) {
        return "";
    }
    std::string str(in);
    if (str[0] == '/') {
        str.erase(0,1);
    }
    std::vector<std::string> vec = mySplit(str,"/");
    if (vec.size() == 0 && str.size() > 0) {
        vec.push_back(str);
    }
    cJSON* p = node.m_root;
    for (int i = vec.size(); i > 0; i--) {
        if (i == 1){
            p = cJSON_GetObjectItem(p,vec.back().c_str());
            if (p == nullptr) {
                return "";
            }
            std::string out;
            getValue(p,out);
            return out;
        }else {
            p = cJSON_GetObjectItem(p,vec[vec.size() - i].c_str());
            if (p == nullptr) {
                return "";
            }
        }
    }
}


