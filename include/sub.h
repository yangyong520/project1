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


int writfile(char *temp) ;
char *get_data_path(char *data_path,int s);

char    error[128];
char    info[128];
char    warn[128];
char    debug[128];


const char *filename = "sub.log";
const char *dataname = "data.txt";

typedef struct data {
    char time[12][6];
} data;
static int running = 1;

#endif

