#include "logger.h"

// 输出日志信息到控制台
void output_to_console(int level, const char *log_str) {
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

// 输出日志信息到文件
void output_to_file(const char *filename, int level, const char *log_str) {
    FILE *file = fopen(filename, "a");
    fseek(file, 0, SEEK_END);
    if (file) {
        long file_size = ftell(file);
        if (file_size > MAX_LOG_SIZE) {  // 检查日志文件大小是否超过指定大小
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
        } else {
            fclose(file);  // 调试代码
        }
    } else {
        printf("Failed to open log file: %s\n", filename);  // 调试代码
    }
}

