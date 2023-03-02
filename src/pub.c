#include"pub.h"

static int running = 1;

char    *dataname= "data.txt";
char    error[356];
char    info[356];
char    warn[356];
char    debug[356];

int main (int argc, char **argv)
{
 pthread_t tid1, tid2;

    // 创建线程，分别执行删除日志文件和其他任务
    pthread_create(&tid1, NULL, delete_log_file, NULL);
    pthread_create(&tid2, NULL, pub, NULL);

    // 等待线程结束
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}

void my_connect_callback(struct mosquitto *mosq,void *obj,int rc)
{
    output_to_console(LOG_LEVEL_INFO, "Call the function: my_connect_callback.");
    output_to_file(filename, LOG_LEVEL_INFO,"Call the function: my_connect_callback." );
}

void my_disconnect_callback(struct mosquitto *mosq,void *obj,int rc)
{
    output_to_console(LOG_LEVEL_INFO, "Call the function: my_disconnect_callback.");
    output_to_file(filename, LOG_LEVEL_INFO,"Call the function: my_disconnect_callback." );

    running = 0;

}

void my_publish_callback(struct mosquitto *mosq,void *obj,int mid)
{
    output_to_console(LOG_LEVEL_INFO, "Call the function: my_publish_callback.");
    output_to_file(filename, LOG_LEVEL_INFO,"Call the function: my_publish_callback." );
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
	    sprintf(error,"open folder %s failure: %s",w1_path,strerror(errno));
	    output_to_console(LOG_LEVEL_ERROR,error);
            output_to_file(filename, LOG_LEVEL_ERROR,error);

	    return -1;
	}

	while(NULL!=(direntp =readdir(dirp)))
	{
		if(strstr(direntp->d_name,"28-"))
		{
			strncpy(chip_sn,direntp->d_name,sizeof(chip_sn));
			found=1;
		}
	}

	closedir(dirp);

	if(!found)
	{
		output_to_console(LOG_LEVEL_ERROR, "Can not find ds18b20 chipset.");
   		output_to_file(filename, LOG_LEVEL_ERROR,"Can not find ds18b20 chipset." );

		return -2;
	}

	strncat(w1_path,chip_sn,sizeof(w1_path)-strlen(w1_path));
	strncat(w1_path,"/w1_slave",sizeof(w1_path)-strlen(w1_path));

	if( (fd=open(w1_path,O_RDONLY))<0)
	{
		sprintf(error,"read data from fd=%d failure %s",fd ,strerror(errno));
                output_to_console(LOG_LEVEL_ERROR, error);
                output_to_file(filename, LOG_LEVEL_ERROR,error );

		return -4;
	}

	memset(buf,0,sizeof(buf));

	if(read(fd,buf,sizeof(buf))<0)
	{
		sprintf(error,"read data from fd=%d faiure %s",fd,strerror(errno));
                output_to_console(LOG_LEVEL_ERROR, error);
                output_to_file(filename, LOG_LEVEL_ERROR,error );

		return -4;
	}

	ptr=strstr(buf,"t=");
	if(!ptr)
	{
		sprintf(error,"Can not find t=string");
                output_to_console(LOG_LEVEL_ERROR, error);
                output_to_file(filename, LOG_LEVEL_ERROR,error );

		return -5;
	}
	ptr +=2;

	strcpy(tempbuf,ptr);
	close(fd);

	return 0;

}


void *pub(void *arg)
{

    int rv;
    struct mosquitto *mosq;
    char buff[MSG_MAX_SIZE];

    get_config();
    strcat(config.log_path,"pub.log");


    mosquitto_lib_init();

    mosq = mosquitto_new("pub_test",true,NULL);
    if(mosq == NULL)
    {
        sprintf(error,"New pub_test error: %s.",strerror(errno));
	output_to_console(LOG_LEVEL_ERROR, error);
        output_to_file(filename, LOG_LEVEL_ERROR,error );

        mosquitto_lib_cleanup();
        return NULL;
    }

    mosquitto_connect_callback_set(mosq,my_connect_callback);
    mosquitto_disconnect_callback_set(mosq,my_disconnect_callback);
    mosquitto_publish_callback_set(mosq,my_publish_callback);

    rv = mosquitto_connect(mosq,config.ip,config.port,KEEP_ALIVE);

    if(rv)
    {
        sprintf(error,"Connect server error: %s.",strerror(errno));
	output_to_console(LOG_LEVEL_ERROR, error);
        output_to_file(filename, LOG_LEVEL_ERROR,error );

        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return NULL;
    }

    sprintf(info,"Start!");
    output_to_console(LOG_LEVEL_INFO, info);
    output_to_file(filename, LOG_LEVEL_INFO,info );


    int loop = mosquitto_loop_start(mosq);
    if(loop)
    {
        sprintf(error,"mosquitto loop error: %s.",strerror(errno));
        output_to_console(LOG_LEVEL_ERROR, error);
        output_to_file(filename, LOG_LEVEL_ERROR,error );

        return NULL;
    }
    while (1)
    {
    	rv=get_temperature(buff);
   	 if(rv<0)
    	{
		sprintf(error,"get temperature failure,return value: %d",rv);
        	output_to_console(LOG_LEVEL_ERROR, error);
        	output_to_file(filename, LOG_LEVEL_ERROR,error );

		return NULL;
    	}

    	mosquitto_publish(mosq,NULL,"test",strlen(buff)+1,buff,0,0);
	sleep(2);
    }

    memset(buff,0,sizeof(buff));
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    sprintf(info,"End!");
    output_to_console(LOG_LEVEL_INFO, info);
    output_to_file(filename, LOG_LEVEL_INFO,info );


    return NULL;
} 


