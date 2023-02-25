
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mosquitto.h"


#define HOST "23.224.131.118"
#define PORT 1883
#define KEEP_ALIVE 60
#define MSG_MAX_SIZE 512


int writfile(char *temp) ;

typedef struct data {
    char time[12][6];
} data;
static int running = 1;

void my_connect_callback(struct mosquitto *mosq,void *obj,int rc)
{
    printf("Call the function:on_connect\n");

    if(rc)
    {
        printf("on_connect error!\n");
        exit(1);
    }
    else
    {
        if(mosquitto_subscribe(mosq,NULL,"test",2))
        {
            printf("Set the topic error!\n");
            exit(1);
        }
        
    }
}


void my_disconnect_callback(struct mosquitto *mosq,void *obj,int rc)
{
    printf("Call the function: my_disconnect_callback\n");
    running = 0;
}

void my_subscribe_callback(struct mosquitto *mosq,void *obj,int mid,int qos_count,const int *granted_qos)
{
    printf("Call the function: on_subscribe\n");
}

void my_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    writfile((char *)msg->payload);

    printf("Call the function: on_message\n");
    printf("Recieve a message of %s: %s\n.",(char *)msg->topic,(char *)msg->payload);
    if(0 == strcmp(msg->payload,"quit")){
    
        mosquitto_disconnect(mosq);
    }
}


int writfile(char *temp) {
    static int i = 0;
    static data dt;
    FILE *fp;
    size_t len = strlen(temp);
    if (len > 0 && temp[len-1] == '\n') {
        temp[len-1] = '\0';
    }
    strcpy(dt.time[i], temp);
    i = (i + 1) % 12;
    if (i==0)
    {
        fp=fopen("temp.txt","a");
        fprintf(fp,"%s %s %s %s %s %s %s %s %s %s %s %s\n", dt.time[0], dt.time[1], dt.time[2], dt.time[3], dt.time[4], dt.time[5], dt.time[6], dt.time[7], dt.time[8], dt.time[9], dt.time[10], dt.time[11]);
    fclose(fp);
    }
    
    return 0;
}
int main (int argc, char **argv)
{
    int ret;
    struct mosquitto *mosq;

    ret = mosquitto_lib_init();

    mosq = mosquitto_new("sub_test",true,NULL);
    if(mosq == NULL)
    {
        printf("New sub_test error!\n");
        mosquitto_lib_cleanup();
        return -1;
    }

    
    mosquitto_connect_callback_set(mosq,my_connect_callback);
    mosquitto_disconnect_callback_set(mosq,my_disconnect_callback);
    mosquitto_subscribe_callback_set(mosq,my_subscribe_callback);
    mosquitto_message_callback_set(mosq,my_message_callback);

    ret = mosquitto_connect(mosq,HOST,PORT,KEEP_ALIVE);

    if(ret)
    {
        printf("Connect server error!\n");
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return -1;
    }

    printf("Start!\n");

    while(running)
    {
        mosquitto_loop(mosq,-1,1);
    }


    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();


    printf("End!\n");

    return 0;
} 

