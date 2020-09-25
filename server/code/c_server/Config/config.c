#include "config.h"
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>

char config_arr[LINEMAX][100] = {0};

bool config_init()
{
    DEBUG_D("config_init in %s","hello");

    FILE *fp = fopen(SERVER_CONFIG_PATH,"r");
    if (fp == NULL) {
        DEBUG_E("open %s fail", SERVER_CONFIG_PATH);
        return false;
    }

    int i = 0;
    while (fgets(config_arr[i++], 100, fp)) {
        DEBUG_I(" %s", config_arr[i-1]);
        if (LINEMAX == i) break;
    }
    if (i != LINEMAX ) {
        fclose(fp);
        return false;
    }
    fclose(fp);
    return true;
}
