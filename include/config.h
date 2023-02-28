#ifndef CONFIG_H
#define CONFIG_H

#define MAX_IP_LEN 16
#define MAX_PATH_LEN 256

typedef struct {
    char ip[MAX_IP_LEN];
    int port;
    int logging_enable;
    int logging_to_console;
    int logging_to_file;
    char log_path[MAX_PATH_LEN];
    char data_path[MAX_PATH_LEN];
} config_t;

int read_config(const char *filename, config_t *config);
int config_handler(void *user, const char *section, const char *name, const char *value);

#endif

