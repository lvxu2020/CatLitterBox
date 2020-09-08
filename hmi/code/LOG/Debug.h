#ifndef DEBUG_H_
#define DEBUG_H_

//#include "platform.h"
#include <stdio.h>
#include "LogRecoder.h"

#define DEBUG_POS printf(" -->%s:%d: [%s]\n", __FILE__,__LINE__,__FUNCTION__);

//#define __WriteLogFile__     // 使能原始数据记录

#define ENABLE_DEBUG_I_RECOD // 使能DEBUG_I记录
#define ENABLE_DEBUG_I_PRINT // 使能DEBUG_I输出

#if defined ENABLE_DEBUG_I_RECOD && defined ENABLE_DEBUG_I_PRINT
#define DEBUG_I(format, ...) { std::string str; char dataStr[5000] = {}; char dataPos[1000] = {}; sprintf(dataStr, format, ##__VA_ARGS__); sprintf(dataPos, " -->%s:%d: [%s]\n", __FILE__,__LINE__,__FUNCTION__); str.append(dataStr).append(dataPos); LogRecoder::instance().insertLog("[DEBUG_I]", str); printf("DEBUG_I: %s %s", dataStr, dataPos); }
#elif defined ENABLE_DEBUG_I_RECOD
#define DEBUG_I(format, ...) { std::string str; char dataStr[5000] = {}; char dataPos[1000] = {}; sprintf(dataStr, format, ##__VA_ARGS__); sprintf(dataPos, " -->%s:%d: [%s]\n", __FILE__,__LINE__,__FUNCTION__); str.append(dataStr).append(dataPos); LogRecoder::instance().insertLog("[DEBUG_I]", str); }
#elif defined ENABLE_DEBUG_I_PRINT
#define DEBUG_I(format, ...) { printf("DEBUG_I: "#format, ##__VA_ARGS__); DEBUG_POS }
#else
#define DEBUG_I(format, ...)
#endif

#define ENABLE_DEBUG_W_RECOD // 使能DEBUG_W记录
#define ENABLE_DEBUG_W_PRINT // 使能DEBUG_W输出

#if defined ENABLE_DEBUG_W_RECOD && defined ENABLE_DEBUG_W_PRINT
#define DEBUG_W(format, ...) { std::string str; char dataStr[5000] = {}; char dataPos[1000] = {}; sprintf(dataStr, format, ##__VA_ARGS__); sprintf(dataPos, " -->%s:%d: [%s]\n", __FILE__,__LINE__,__FUNCTION__); str.append(dataStr).append(dataPos); LogRecoder::instance().insertLog("[DEBUG_W]", str); printf("DEBUG_W: %s %s", dataStr, dataPos); }
#elif defined ENABLE_DEBUG_W_RECOD
#define DEBUG_W(format, ...) { std::string str; char dataStr[5000] = {}; char dataPos[1000] = {}; sprintf(dataStr, format, ##__VA_ARGS__); sprintf(dataPos, " -->%s:%d: [%s]\n", __FILE__,__LINE__,__FUNCTION__); str.append(dataStr).append(dataPos); LogRecoder::instance().insertLog("[DEBUG_W]", str); }
#elif defined ENABLE_DEBUG_W_PRINT
#define DEBUG_W(format, ...) { printf("DEBUG_W: "#format, ##__VA_ARGS__); DEBUG_POS }
#else
#define DEBUG_W(format, ...)
#endif

#define ENABLE_DEBUG_E_RECOD // 使能DEBUG_E记录
#define ENABLE_DEBUG_E_PRINT // 使能DEBUG_E输出

#if defined ENABLE_DEBUG_E_RECOD && defined ENABLE_DEBUG_E_PRINT
#define DEBUG_E(format, ...) { std::string str; char dataStr[5000] = {}; char dataPos[1000] = {}; sprintf(dataStr, format, ##__VA_ARGS__); sprintf(dataPos, " -->%s:%d: [%s]\n", __FILE__,__LINE__,__FUNCTION__); str.append(dataStr).append(dataPos); LogRecoder::instance().insertLog("[DEBUG_E]", str); printf("DEBUG_E: %s %s", dataStr, dataPos);}
#elif defined ENABLE_DEBUG_E_RECOD
#define DEBUG_E(format, ...) { std::string str; char dataStr[5000] = {}; char dataPos[1000] = {}; sprintf(dataStr, format, ##__VA_ARGS__); sprintf(dataPos, " -->%s:%d: [%s]\n", __FILE__,__LINE__,__FUNCTION__); str.append(dataStr).append(dataPos); LogRecoder::instance().insertLog("[DEBUG_E]", str); }
#elif defined ENABLE_DEBUG_E_PRINT
#define DEBUG_E(format, ...) { printf("DEBUG_E: "#format, ##__VA_ARGS__); DEBUG_POS }
#else
#define DEBUG_E(format, ...)
#endif

//#define ENABLE_DEBUG_D_RECOD // 使能DEBUG_D记录
#define ENABLE_DEBUG_D_PRINT // 使能DEBUG_D输出

#if defined ENABLE_DEBUG_D_RECOD && defined ENABLE_DEBUG_D_PRINT
#define DEBUG_D(format, ...) { std::string str; char dataStr[5000] = {}; char dataPos[1000] = {}; sprintf(dataStr, format, ##__VA_ARGS__); sprintf(dataPos, " -->%s:%d: [%s]\n", __FILE__,__LINE__,__FUNCTION__); str.append(dataStr).append(dataPos); LogRecoder::instance().insertLog("[DEBUG_D]", str); printf("DEBUG_D: %s %s", dataStr, dataPos); }
#elif defined ENABLE_DEBUG_D_RECOD
#define DEBUG_D(format, ...) { std::string str; char dataStr[5000] = {}; char dataPos[1000] = {}; sprintf(dataStr, format, ##__VA_ARGS__); sprintf(dataPos, " -->%s:%d: [%s]\n",basename(__FILE__),__LINE__,__FUNCTION__); str.append(dataStr).append(dataPos); LogRecoder::instance().insertLog("[DEBUG_D]", str); }
#elif defined ENABLE_DEBUG_D_PRINT
#define DEBUG_D(format, ...) { printf("DEBUG_D: "#format, ##__VA_ARGS__); DEBUG_POS }
#else
#define DEBUG_D(format, ...)
#endif

#endif
/* ******************讲解***********************
一、#用来把参数转换成字符串
#define SQUARE(x) printf("The square of "#x" is %d.\n", ((x)*(x)));
SQUARE(8)
输出的是：The square of 8 is 64

二、##运算符可以用于宏函数的替换部分。这个运算符把两个语言符号组合成单个语言符号，
为宏扩展提供了一种连接实际变元的手段

#define XNAME(n) x ## n
如果这样使用宏：XNAME(8)

则会被展开成这样：x8
##就是个粘合剂，将前后两部分粘合起来，也就是有“字符化”的意思。但是“##”不能随意
粘合任意字符，必须是合法的C语言标示符。在单一的宏定义中，最多可以出现一次“#”或
“##”预处理操作符。如果没有指定与“#”或“##”预处理操作符相关的计算次序，则会产生
问题。为避免该问题，在单一的宏定义中只能使用其中一种操作符(即，一份“#”或一个
“##”，或都不用)。除非非常有必要，否则尽量不要使用“#”和“##”

三、__VA_ARGS__ 是一个可变参数的宏，很少人知道这个宏，这个可变参数的宏是新的C99规范中新增的，目前似乎只有gcc支持（VC6.0的编译器不支持）。
实现思想就是宏定义中参数列表的最后一个参数为省略号（也就是三个点）。

四、##__VA_ARGS__ 宏前面加上##的作用在于，当可变参数的个数为0时，这里的##起到把前面多余的","去掉的作用,否则会编译出错

#define my_print1(...)  printf(__VA_ARGS__)   my_print1("i=%d,j=%d\n",i,j)  正确打印

#define my_print2(fmt,...)  printf(fmt,__VA_ARGS__)

my_print1("i=%d,j=%d\n",i,j) 正确打印

my_print2("iiiiiii\n")       编译失败打印，因为扩展出来只有一个参数，至少要两个及以上参数

如果是#define my_print2(fmt,...)  printf(fmt,##__VA_ARGS__)

那么

my_print1里面不管是几个参数都能正确打印

******************讲解*********************** */
