#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

void *tfn(void *arg)
{
    int i = (int)arg;
    sleep(i);
    printf("%d th thread pid = %d,tid = %lu\n",i,getpid(),pthread_self());
    return NULL;
}


int main(int argc,char *argv[])
{
    int i,ret;
    pthread_t tid;

    for(i = 0;i < 5;i++){
        ret = pthread_create(&tid,NULL,tfn,(void *)i); //64位中，int 4个字节，指针8个字节，强转成void不会有影响
        if(ret != 0){
            sys_err("pthread_create error");
        }
    }
    sleep(i);
    printf("main:pid = %d ,tid = %lu\n",getpid(),pthread_self());
    return 0;
}
