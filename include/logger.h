#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>
#include <unistd.h>  // 包含 sleep 函数的头文件

#define DEBUG
#ifdef DEBUG
#define dbg_print(format,args...) printf(format,##args)
#else
#define dbg_print(format,args...) do{} while(0)
#endif
#define MAX_LOG_SIZE 1024 *1024*10  // 1MB

#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_ERROR 3

void output_to_console(int level, const char *log_str);
void output_to_file(const char *filename, int level, const char *log_str) ;
void roll_log(const char *filename, int max_size) ;

#endif // LOGGER_H

