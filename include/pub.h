#ifndef PUB_H
#define PUB_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "mosquitto.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include "logger.h"
char    error[128];
char    info[128];
char    warn[128];
char    debug[128];

const char *filename = "pub.log";
static int running = 1;

#define HOST "23.224.131.118"
#define PORT 1883
#define KEEP_ALIVE 60
#define MSG_MAX_SIZE 512
#endif

