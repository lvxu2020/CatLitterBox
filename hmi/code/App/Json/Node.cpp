#include "Node.h"


Node::Node()
{
   m_root = cJSON_CreateObject();
   if (m_root) {
       m_bValid = true;
   }
}

Node::Node(const std::string& str)
{
    m_root = cJSON_Parse(str.c_str());
    if (m_root) {
        m_bValid = true;
    }
}

Node::Node(const char* str)
{
    m_root = cJSON_Parse(str);
    if (m_root) {
        m_bValid = true;
    }
}

Node::~Node()
{
    cJSON_Delete(m_root);
}

bool Node::isValid()
{
    return m_bValid;
}

bool Node::operator<<(const std::string str)
{
    if (m_root) {
        cJSON_Delete(m_root);
    }
    m_root = cJSON_Parse(str.c_str());
    if (m_root) {
        m_bValid = true;
    }else {
        m_bValid = false;
    }
    return m_bValid;
}

bool Node::operator<<(const char* str)
{
    if (m_root) {
        cJSON_Delete(m_root);
    }
    m_root = cJSON_Parse(str);
    if (m_root) {
        m_bValid = true;
    }else {
        m_bValid = false;
    }
    return m_bValid;
}

Node& Node::operator=(const Node& cpy)
{
    char *p = cJSON_Print(cpy.m_root);
    cJSON* dest = cJSON_Parse(p);
    free(p);
    if (dest) {
        cJSON_Delete(m_root);
        this->m_root = dest;

    }
    return *this;
}
