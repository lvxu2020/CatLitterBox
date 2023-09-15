#ifndef REGISTER_H_
#define REGISTER_H_

#include "Debug/Debug.h"
#include "Config/config.h"

#define SEM_REQ_SHM_PATH "/"
#define SEM_REQ_SHM_CHAR 'a'
#define SEM_CGIWRITE_PATH "/bin"
#define SEM_CGIWRITE_CHAR 'b'
#define SEM_RETURNCGI_PATH "/bin"
#define SEM_RETURNCGI_CHAR 'c'
#define SHM_PATH "/bin"
#define SHM_CHAR 'd'
#define SHM_MAX 128
#define SHM_TASK_TRTURN_OK 6
#define SHM_CHECK_OK "yes"
#define STATUS "status"
#define CONF "usr.conf"


union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};

void * register_usr(void *p);

#endif
