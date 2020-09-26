#include "mqttSend.h"
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include "assert.h"

static pthread_once_t  ponce_;
static myQueue *ptr = NULL;

bool qmtt_send_init()
{

}
