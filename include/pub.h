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
#include "config.h"
#include <pthread.h>


#ifdef DEBUG
#define dbg_print(format,args...) printf(format,##args)
#else
#define dbg_print(format,args...) do{} while(0)
#endif

#define KEEP_ALIVE 60
#define MSG_MAX_SIZE 512

void my_connect_callback(struct mosquitto *mosq,void *obj,int rc);
void my_disconnect_callback(struct mosquitto *mosq,void *obj,int rc);
void my_publish_callback(struct mosquitto *mosq,void *obj,int mid);
int get_temperature(char *tempbuf);
void *pub(void *arg);
extern void get_config();
extern void *delete_log_file(void *arg);

#endif

