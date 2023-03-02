#ifndef INI_H
#define INI_H
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "config.h"


typedef int (*ini_handler)(void *user, const char *section, const char *name, const char *value);

int ini_parse(const char *filename, ini_handler handler, void *user);

extern int config_handler(void* user, const char* section, const char* name, const char* value);

#endif

