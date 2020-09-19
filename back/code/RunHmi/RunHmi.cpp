#include "RunHmi.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>


#define VERSION_PATH_1 "/home/debian/Cat/ver1/ver.conf"
#define VERSION_PATH_2 "/home/debian/Cat/ver2/ver.conf"

RunHmi::RunHmi()
{

}

void* RunHmi::run(void *arg)
{
    std::ifstream conf1(VERSION_PATH_1), conf2(VERSION_PATH_2);
    char buf[10] = {0};
    conf1.getline(buf,10);
    int v1 = atoi(buf);
    conf1.close();

    memset(buf, '\0', 10);
    conf2.getline(buf,10);
    int v2 = atoi(buf);
    conf2.close();

    if (v2 > v1) {
        DEBUG("start ver2");
        system("echo 2 > /home/debian/Back/running");
        system("/home/debian/Cat/run2.sh");
    } else {
        DEBUG("start ver1");
        system("echo 1 > /home/debian/Back/running");
        system("/home/debian/Cat/run1.sh");
    }
}
