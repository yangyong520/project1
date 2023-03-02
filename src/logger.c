#include "logger.h"

char *filename = config.log_path;

// 输出日志信息到控制台
void output_to_console(int level, const char *log_str) {
    printf(",config.logging_to_console:%d\n",config.logging_to_console);

    if (config.logging_to_console) {
        switch (level) {
            case LOG_LEVEL_DEBUG:
                printf("[DEBUG] %s\n", log_str);
                break;
            case LOG_LEVEL_INFO:
                printf("[INFO] %s\n", log_str);
                break;
            case LOG_LEVEL_WARN:
                printf("[WARN] %s\n", log_str);
                break;
            case LOG_LEVEL_ERROR:
                printf("[ERROR] %s\n", log_str);
                break;
            default:
                break;
        }
    }
}

// 输出日志信息到文件
void output_to_file(const char *filename, int level, const char *log_str) {

    creat_file(filename);

    FILE *file = fopen(filename, "a");
        if (file == NULL) {
        printf("Failed to open file: %s\n", filename);
       }
	printf("config.logging_to_file:%d\n",config.logging_to_file);
    if (config.logging_to_file) {
        fseek(file, 0, SEEK_END);
        if (file) {
            long file_size = ftell(file);
	    printf("file_size:%ld  MAX_LOG_SIZE :%d\n",file_size ,MAX_LOG_SIZE);
            if (file_size > MAX_LOG_SIZE) {
                // 检查日志文件大小是否超过指定大小
                fclose(file);
                roll_log(filename, MAX_LOG_SIZE);
                file = fopen(filename, "a");
            }
            if (file) {
                char time_str[24];
                time_t timestamp = time(NULL);
                strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&timestamp));
                fprintf(file, "[%s] ", time_str);
                switch (level) {
                    case LOG_LEVEL_DEBUG:
                        fprintf(file, "[DEBUG] %s\n", log_str);
                        break;
                    case LOG_LEVEL_INFO:
                        fprintf(file, "[INFO] %s\n", log_str);
                        break;
                    case LOG_LEVEL_WARN:
                        fprintf(file, "[WARN] %s\n", log_str);
                        break;
                    case LOG_LEVEL_ERROR:
                        fprintf(file, "[ERROR] %s\n", log_str);
                        break;
                    default:
                        break;
                }
                fclose(file);
            }
        }
    }
}

void roll_log(const char *filename, int max_size) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        if (file_size > max_size) {
            fclose(file);
            char new_filename[256];
            time_t timestamp = time(NULL);
            struct tm *time_info = localtime(&timestamp);
            strftime(new_filename, sizeof(new_filename), "%Y-%m-%d_%H-%M-%S", time_info);
            char bak_filename[512];
            sprintf(bak_filename, "%s_%s.log", filename, new_filename);
            rename(filename, bak_filename);
            file = fopen(filename, "w");
            if (file) {
                fclose(file);
            }
        }
        else {
            fclose(file);  // 调试代码
        }
    }
    else {
        printf("Failed to open log file: %s\n", filename);  
    }
}
void creat_file(const char *filename) {
    char *dir;
    struct stat st = {0};

    // get directory path
    dir = strrchr(filename, '/');
    if (dir != NULL) {
        *dir = '\0'; // terminate the string at the last '/'
        // create directory if it does not exist
        if (stat(filename, &st) == -1) {
            if (mkdir(filename, 0700) == -1) {
                perror("mkdir failed");
                return;
            }
        }
        *dir = '/'; // restore the original filename
    }

    // create file if it does not exist
    FILE *fp;
    if ((fp = fopen(filename, "r")) == NULL) {
        fp = fopen(filename, "w");
        if (fp == NULL) {
            printf("Failed to create file: %s\n", filename);
        }
        printf("File created: %s\n", filename);
    } else {
        printf("File exists: %s\n", filename);
        fclose(fp);
    }
}
void *delete_log_file(void *arg) {
    while (1) {
        // 获取需要删除的日志文件名
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        t->tm_mday--;
        mktime(t);
        char filename[40];
        strftime(filename, sizeof(filename), "sub.log_%Y-%m-%d_%H-%M-%S.log", t);

        // 判断需要删除的日志文件是否存在，如果存在则删除
        if (access(filename, F_OK) != -1) {
            if (remove(filename) == 0) {
                printf("Deleted log file: %s\n", filename);
            } else {
                printf("Failed to delete log file: %s\n", filename);
            }
        }

        // 等待一天
        sleep(24 * 60 * 60);
    }

    return NULL;
}
