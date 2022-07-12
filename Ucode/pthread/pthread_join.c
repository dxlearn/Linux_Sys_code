#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


struct test{
    int id;
    char str[256];
};

void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

void *tfn(void *arg)
{
    struct test *t;          //如果采用strcut test t，然后初始化成员的方式，t是分配在函数栈上的，会随着tfn函数的执行而消失
    t = malloc(sizeof(t));   //malloc分配在堆上，不会随着函数的结束而释放(栈随着函数结束而释放)
    t->id = 1;
    strcpy(t->str,"hello");
    printf("thread:pid = %d,tid = %lu\n",getpid(),pthread_self());
    return (void *)t;
}


int main(int argc,char *argv[])
{
    pthread_t tid;
    struct test *t;
    printf("main:pid = %d,tid = %lu\n",getpid(),pthread_self());
    int ret = pthread_create(&tid,NULL,tfn,NULL);
    if(ret != 0){
        perror("pthread_create error");
    }
    ret = pthread_join(tid,(void **)&t);
    printf("thread result:id:%d,str:%s\n",t->id,t->str);
    sleep(1);
    return 0;
}
