#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

int resouece = 100;

void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

void *tfn(void *arg)
{
    resouece = 1000;
    printf("thread:pid = %d,tid = %lu\n",getpid(),pthread_self());
    return NULL;
}

int main(int argc,char *argv[])
{
    pthread_t tid;
    printf("main:pid = %d,tid = %lu\n",getpid(),pthread_self());
    printf("Before create thread resource:%d\n",resouece);
    int ret = pthread_create(&tid,NULL,tfn,NULL);
    if(ret != 0){
        perror("pthread_create error");
    }
    sleep(1);
    printf("After main thread resource:%d\n",resouece);
    return 0;

}