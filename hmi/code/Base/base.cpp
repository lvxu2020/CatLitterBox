#include "base.h"
#include <QCoreApplication>
#include <QTime>
#include <iostream>
#include <vector>
#include <iterator>
#include <regex>

Base::Base()
{

}
//qt是针对于图形界面的程序，如果直接sleep，那界面势必无法响应。无法刷新。
void BaseSpace::mySleep(int ms = 1000){
    QTime time;
    time.start();
    while (time.elapsed() < ms)
    {
        QCoreApplication::processEvents();
    }
}

/*
   用delim指定的正则表达式将字符串in分割，返回分割后的字符串数组
   delim 分割字符串的正则表达式
 */
std::vector<std::string> BaseSpace::mySplit(const std::string& in, const std::string& delim) {
    std::regex re{ delim };
    // 调用 std::vector::vector (InputIterator first, InputIterator last,const allocator_type& alloc = allocator_type())
    // 构造函数,完成字符串分割
    return std::vector<std::string> {
        std::sregex_token_iterator(in.begin(), in.end(), re, -1),
        std::sregex_token_iterator()
    };
}

// 用于QString类型的字符串 调用shall命令
char* BaseSpace::qStringToC(QString& str)
{
    QByteArray c_str;
    c_str = str.toLatin1();
    return c_str.data();
}
//裁剪字符串左右侧留白
const std::string WHITESPACE = " \n\r\t\f\v";
std::string BaseSpace::trimStr(const std::string& str)
{
    size_t pos = str.find_first_not_of(WHITESPACE);
    if(pos == std::string::npos){
        return "";
    }else{
        pos = str.substr(pos).find_last_not_of(WHITESPACE);
        return (pos == std::string::npos) ? "" : str.substr(0, pos + 1);
    }
}

