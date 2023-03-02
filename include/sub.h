#ifndef SUB_H
#define SUB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mosquitto.h"
#include "logger.h"
#include "config.h"
#include <pthread.h>

#define KEEP_ALIVE 60
#define MSG_MAX_SIZE 512

#ifdef DEBUG
#define dbg_print(format,args...) printf(format,##args)
#else
#define dbg_print(format,args...) do{} while(0)
#endif

void my_connect_callback(struct mosquitto *mosq,void *obj,int rc);
void my_disconnect_callback(struct mosquitto *mosq,void *obj,int rc);
void my_subscribe_callback(struct mosquitto *mosq,void *obj,int mid,int qos_count,const int *granted_qos);
void my_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);
int writfile(char *temp) ;
void *sub(void *arg);

extern void get_config();
extern void *delete_log_file(void *arg);
extern void create_file(const char *filename);

typedef struct data {
    char time[12][6];
} data;

#endif

