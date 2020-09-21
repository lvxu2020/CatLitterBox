#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "control.h"
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
#include "../Tool/myQueue.h"

#define MQ_KEY_PATH "/lib"
#define MQ_KEY_CHAR 'A'
#define MQ_MSGBUF_LEN 50
#define GROUP_LINE 10

void * remoteCmd(void *p);
typedef struct mqbuf
{
        long type;
        char msg[MQ_MSGBUF_LEN];
}MSG;

int getMqId(char *path,char ch);
int processTask(MSG *p);


#endif  //_CONTROL_H_
