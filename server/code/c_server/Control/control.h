#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <sys/ipc.h>
#include <strings.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <unistd.h>
#include <fcntl.h>
#include "../Queue/myQueue.h"
#include "Debug/Debug.h"

#define MQ_KEY_PATH "/lib"
#define MQ_KEY_CHAR 'A'
#define MQ_MSGBUF_LEN 50
#define GROUP_LINE 10


typedef struct mqbuf
{
        long type;
        char msg[MQ_MSGBUF_LEN];
}MSG;

void * remote_control(void *p);
int get_mq_id(char *path, char ch);
void process_task(MSG *p);


#endif  //_CONTROL_H_
