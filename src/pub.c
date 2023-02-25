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


#define HOST "23.224.131.118"
#define PORT 1883
#define KEEP_ALIVE 60
#define MSG_MAX_SIZE 512


static int running = 1;


void my_connect_callback(struct mosquitto *mosq,void *obj,int rc)
{
    printf("Call the function: my_connect_callback.\n");
}

void my_disconnect_callback(struct mosquitto *mosq,void *obj,int rc)
{
    printf("Call the function: my_disconnect_callback.\n");
    running = 0;

}


void my_publish_callback(struct mosquitto *mosq,void *obj,int mid)
{
    printf("Call the function: my_publish_callback.\n");

}
int get_temperature(char *tempbuf)
{
	int 		fd=-1;
	char		buf[128];
	char		*ptr =NULL;
	DIR		*dirp = NULL;
	struct dirent	*direntp = NULL;
	char		w1_path[64]="/home/yy/bus/w1/devices/";
	char		chip_sn[32];
	int		found =0;

	dirp=opendir(w1_path);
	if( !dirp)
	{
		printf("open folder %s failure: %s\n",w1_path,strerror(errno));
		return -1;
	}

	while(NULL!=(direntp =readdir(dirp)))
	{
		if(strstr(direntp->d_name,"28-"))
			strncpy(chip_sn,direntp->d_name,sizeof(chip_sn));
			found=1;
	}

	closedir(dirp);

	if(!found)
	{
		printf("Can not find ds18b20 chipset\n");
		return -2;
	}

	strncat(w1_path,chip_sn,sizeof(w1_path)-strlen(w1_path));
	strncat(w1_path,"/w1_slave",sizeof(w1_path)-strlen(w1_path));

	if( (fd=open(w1_path,O_RDONLY))<0)
	{
		printf("read data from fd=%d failure %s\n",fd ,strerror(errno));
		return -4;
	}

	memset(buf,0,sizeof(buf));

	if(read(fd,buf,sizeof(buf))<0)
	{
		printf("read data from fd=%d faiure %s\n",fd,strerror(errno));
		return -4;
	}

	ptr=strstr(buf,"t=");
	if(!ptr)
	{
		printf("Can not find t=string\n");
		return -5;
	}
	ptr +=2;

	strcpy(tempbuf,ptr);
	close(fd);

	return 0;

}


int main (int argc, char **argv)
{

    int rv;
    struct mosquitto *mosq;
    char buff[MSG_MAX_SIZE];

    mosquitto_lib_init();

    mosq = mosquitto_new("pub_test",true,NULL);
    if(mosq == NULL)
    {
        printf("New pub_test error: %s\n",strerror(errno));
        mosquitto_lib_cleanup();
        return -1;
    }

    mosquitto_connect_callback_set(mosq,my_connect_callback);
    mosquitto_disconnect_callback_set(mosq,my_disconnect_callback);
    mosquitto_publish_callback_set(mosq,my_publish_callback);

    rv = mosquitto_connect(mosq,HOST,PORT,KEEP_ALIVE);

    if(rv)
    {
        printf("Connect server error: %s\n",strerror(errno));
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return -1;
    }

    printf("Start!\n");

    int loop = mosquitto_loop_start(mosq);
    if(loop)
    {
        printf("mosquitto loop error: %s\n",strerror(errno));
        return 1;
    }
    while (1)
    {
    	rv=get_temperature(buff);
   	 if(rv<0)
    	{
		printf("get temperature failure,return value: %d",rv);
		return -1;
    	}

    	mosquitto_publish(mosq,NULL,"test",strlen(buff)+1,buff,0,0);
	sleep(2);
    }

    memset(buff,0,sizeof(buff));
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    printf("End!\n");

    return 0;
} 


