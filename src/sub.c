#include "sub.h"
#include "config.h"

void my_connect_callback(struct mosquitto *mosq,void *obj,int rc)
{
    sprintf(info,"Call the function:on_connect.");
    output_to_console(LOG_LEVEL_INFO, info);
    output_to_file(filename, LOG_LEVEL_INFO, info);

    if(rc)
    {
        sprintf(error,"on_connect error!");
	output_to_console(LOG_LEVEL_ERROR, error);
        output_to_file(filename, LOG_LEVEL_ERROR, error);
        exit(1);
    }
    else
    {
        if(mosquitto_subscribe(mosq,NULL,"test",2))
        {
            sprintf(error,"Set the topic error!");
	    output_to_console(LOG_LEVEL_ERROR, error);
            output_to_file(filename, LOG_LEVEL_ERROR, error);

            exit(1);
        }
        
    }
}


void my_disconnect_callback(struct mosquitto *mosq,void *obj,int rc)
{
    sprintf(info,"Call the function: my_disconnect_callback");
    output_to_console(LOG_LEVEL_INFO, info);
    output_to_file(filename, LOG_LEVEL_INFO, info);

    running = 0;
}

void my_subscribe_callback(struct mosquitto *mosq,void *obj,int mid,int qos_count,const int *granted_qos)
{
    sprintf(info,"Call the function: on_subscribe");
    output_to_console(LOG_LEVEL_INFO, info);
    output_to_file(filename, LOG_LEVEL_INFO, info);

}

void my_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{

    writfile((char *)msg->payload);

    sprintf(info,"Call the function: on_message");
    output_to_console(LOG_LEVEL_INFO, info);
    output_to_file(filename, LOG_LEVEL_INFO, info);

    sprintf(info,"Recieve a message of %s: %s.",(char *)msg->topic,(char *)msg->payload);
    output_to_console(LOG_LEVEL_INFO, info);
    output_to_file(filename, LOG_LEVEL_INFO, info);

    if(0 == strcmp(msg->payload,"quit")){
    
        mosquitto_disconnect(mosq);
    }
}


int writfile(char *temp) {
    static int i = 0;
    static data dt;
    FILE *fp;
    size_t len = strlen(temp);
    char *data_path=get_data_path("",0);
    if (len > 0 && temp[len-1] == '\n') {
        temp[len-1] = '\0';
    }
    strcpy(dt.time[i], temp);
    i = (i + 1) % 12;
    if (i==0)
    {
        fp=fopen(data_path,"a");
        fprintf(fp,"%s %s %s %s %s %s %s %s %s %s %s %s\n", dt.time[0], dt.time[1], dt.time[2], dt.time[3], dt.time[4], dt.time[5], dt.time[6], dt.time[7], dt.time[8], dt.time[9], dt.time[10], dt.time[11]);
    fclose(fp);
    }
    
    return 0;
}
char *get_data_path(char *data_path,int s)
{
    static char path[MAX_PATH_LEN];
    if (s==1)
    {
	    strcpy(path,data_path);
	    strcat(path,dataname);
    }
    return path;
}
int main (int argc, char **argv)
{
    int ret;
    struct mosquitto *mosq;

    config_t config;
    if(read_config("../config.ini", &config) < 0)
    {
        sprintf(error,"cannot open config file config.ini");
        output_to_console(LOG_LEVEL_ERROR, error);
        output_to_file(filename, LOG_LEVEL_ERROR, error);

    }
   get_data_path(config.data_path,1);

    ret = mosquitto_lib_init();

    mosq = mosquitto_new("sub_test",true,NULL);
    if(mosq == NULL)
    {
        sprintf(error,"New sub_test error!");
	output_to_console(LOG_LEVEL_ERROR, error);
        output_to_file(filename, LOG_LEVEL_ERROR, error);
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
        sprintf(info,"Connect server error!");
	output_to_console(LOG_LEVEL_INFO, info);
        output_to_file(filename, LOG_LEVEL_INFO, info);
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return -1;
    }

    sprintf(info,"Start!");
    output_to_console(LOG_LEVEL_INFO, info);
    output_to_file(filename, LOG_LEVEL_INFO, info);


    while(running)
    {
        mosquitto_loop(mosq,-1,1);
    }


    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();


    sprintf(info,"End!");
    output_to_console(LOG_LEVEL_INFO, info);
    output_to_file(filename, LOG_LEVEL_INFO, info);


    return 0;
} 

