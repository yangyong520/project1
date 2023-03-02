#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include "config.h"

//#define DEBUG
#ifdef DEBUG
#define dbg_print(format,args...) printf(format,##args)
#else
#define dbg_print(format,args...) do{} while(0)
#endif
#define MAX_LOG_SIZE 1024*1024  // 1MB

#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_WARN 3
#define LOG_LEVEL_ERROR 4


#define MAX_IP_LEN 16
#define MAX_PATH_LEN 256

void output_to_console(int level, const char *log_str);
void output_to_file(const char *filename, int level, const char *log_str) ;
void roll_log(const char *filename, int max_size) ;
void creat_file(const char *filename);
void *delete_log_file(void *arg) ;
extern char    *filename;
#endif // LOGGER_H

