#ifndef NODE_H
#define NODE_H

#include "JsonAdapter.h"


class Node
{
public:
    Node();
    Node(const std::string& str);
    Node(const char* str);
    ~Node();
    friend class JsonAdapter;
    bool isValid();
    bool operator<<(const std::string str);
    bool operator<<(const char* str);
    Node &operator=(const Node& cpy);
private:
    Node(const Node &);
    cJSON* m_root = nullptr;
    bool m_bValid = false;
};

#endif // NODE_H
