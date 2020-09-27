#ifndef CONFIG_H_
#define CONFIG_H_

#include "Debug/Debug.h"
#include <stdio.h>
#include <stdbool.h>

#define SERVER_CONFIG_PATH "/home/lvxu/catServer/config/config.conf"
#define CONF_ROW 10

enum config_line {
    http_ip,
    http_port,
    http_upload_path,
    http_down_path,
    mqtt_ip,
    mqtt_port,
    app_verson,
    server_usr_path,

    LINEMAX
};


extern char config_arr[LINEMAX][100];

bool config_init();


#endif
