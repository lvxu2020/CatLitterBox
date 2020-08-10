#ifndef NODE_H
#define NODE_H

#include "jsonAdapter.h"


class Node
{
public:
    Node();
    Node(const std::string&);
    Node(const char*);
    ~Node();
    friend class jsonAdapter;
    bool isValid();
    bool operator<<(const std::string);
    bool operator<<(const char*);
    Node &operator=(const Node&);
private:
    Node(const Node &);
    cJSON* m_root = nullptr;
    bool m_bValid = false;
};

#endif // NODE_H
