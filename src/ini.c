#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ini.h"
#include "config.h"

extern int config_handler(void* user, const char* section, const char* name, const char* value);


// 解析INI配置文件
int ini_parse(const char *filename, ini_handler handler, void *user) {
    FILE *file;
    int line_number = 0;
    char line[1024];
    char section[1024] = "";
    char *start;
    char *end;
    char *name;
    char *value;
    int len;

    // 打开文件
    file = fopen(filename, "r");
    if (!file) {
        return -1;
    }

    // 逐行读取文件内容并进行解析
    while (fgets(line, 1024, file) != NULL) {
        ++line_number;

        // 处理注释和空行
        start = line;
        while (isspace(*start)) {
            ++start;
        }

        if (*start == ';' || *start == '#' || *start == '\0' || *start == '\n') {
            continue;
        }

        // 处理节名
        if (*start == '[') {
            end = strchr(start, ']');
            if (!end) {
                fclose(file);
                return -1;
            }

            len = end - start - 1;
            strncpy(section, start + 1, len);
            section[len] = '\0';
            continue;
        }

        // 处理键值对
        end = strchr(start, '=');
        if (!end) {
            fclose(file);
            return -1;
        }

        *end = '\0';
        name = start;
        value = end + 1;

        // 去除键和值的前后空格
        while (isspace(*name)) {
            ++name;
        }

        end = name + strlen(name) - 1;
        while (isspace(*end)) {
            *end = '\0';
            --end;
        }

        while (isspace(*value)) {
            ++value;
        }

        end = value + strlen(value) - 1;
        while (isspace(*end)) {
            *end = '\0';
            --end;
        }

        // 调用回调函数处理键值对
        if (handler(user, section, name, value) < 0) {
            fclose(file);
            return -1;
        }
    }

    // 关闭文件
    fclose(file);

    return 0;
}

