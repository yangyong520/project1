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

#ifdef DEBUG
#define dbg_print(format,args...) printf(format,##args)
#else
#define dbg_print(format,args...) do{} while(0)
#endif

int writfile(char *temp) ;
config_t *get_config(); 
void my_connect_callback(struct mosquitto *mosq,void *obj,int rc);
void my_disconnect_callback(struct mosquitto *mosq,void *obj,int rc);
void my_subscribe_callback(struct mosquitto *mosq,void *obj,int mid,int qos_count,const int *granted_qos);

void my_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);

static config_t *global_config_ptr = NULL;


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

