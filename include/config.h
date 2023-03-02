#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ini.h"
#include "logger.h"

#define MAX_IP_LEN 16
#define MAX_PATH_LEN 256


typedef struct {
    char ip[MAX_IP_LEN];
    int port;
    int logging_enable;
    int logging_to_console;
    int logging_to_file;
    char log_path[MAX_PATH_LEN];
    int log_level;
    char data_path[MAX_PATH_LEN];
} config_t;

int read_config(const char *filename, config_t *config);
int config_handler(void *user, const char *section, const char *name, const char *value);
void get_config();

extern config_t config;



#endif

