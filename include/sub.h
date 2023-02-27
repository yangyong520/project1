#ifndef SUB_H
#define SUB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mosquitto.h"
#include "logger.h"

#define HOST "23.224.131.118"
#define PORT 1883
#define KEEP_ALIVE 60
#define MSG_MAX_SIZE 512

char    error[128];
char    info[128];
char    warn[128];
char    debug[128];


const char *filename = "sub.log";
int writfile(char *temp) ;

typedef struct data {
    char time[12][6];
} data;
static int running = 1;

#endif

