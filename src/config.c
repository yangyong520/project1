#include "config.h"

config_t config;
char buff[256];

static int handler(void* user, const char* section, const char* name, const char* value) {
    config_t* pconfig = (config_t*)user;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("Network", "ip")) {
        strncpy(pconfig->ip, value, MAX_IP_LEN);
    } else if (MATCH("Network", "port")) {
        pconfig->port = atoi(value);
    } else if (MATCH("Logging", "enable")) {
        pconfig->logging_enable = atoi(value);
    } else if (MATCH("Logging", "to_console")) {
        pconfig->logging_to_console = atoi(value);
    } else if (MATCH("Logging", "to_file")) {
        pconfig->logging_to_file = atoi(value);
    } else if (MATCH("Logging", "output_file")) {
        strncpy(pconfig->log_path, value, MAX_PATH_LEN);
    } else if(MATCH("Logging", "log_level")) {
	pconfig->log_level = atoi(value);
    } else if (MATCH("Data", "save_path")) {
        strncpy(pconfig->data_path, value, MAX_PATH_LEN);
    } else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}

int read_config(const char *filename, config_t *config) {
    if (ini_parse(filename, handler, config) < 0) {
        return -1;
    }
    return 0;
}

void get_config() {
    if(read_config("../config.ini", &config) < 0)
    {
       sprintf(buff,"cannot open config file config.ini");
       output_to_console(LOG_LEVEL_ERROR, buff);
       output_to_file(filename, LOG_LEVEL_ERROR, buff);

    }

}

