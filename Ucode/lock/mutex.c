#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

//全局位置定义一个互斥锁
pthread_mutex_t mutex;

void *tfn(void *arg)
{
    srand(time(NULL));
    while(1){
        pthread_mutex_lock(&mutex);   //加锁
        printf("hello ");
        sleep(rand() % 3);   //模拟长时间操作共享资源，导致cpu调度切换
        printf("world\n");
        pthread_mutex_unlock(&mutex);  //解锁
        sleep(rand()%3);
    }
    return NULL;
}


int main(void)
{
    pthread_t tid;
    srand(time(NULL));
    pthread_mutex_init(&mutex,NULL);     //初始化一个互斥锁
    pthread_create(&tid,NULL,tfn,NULL);  //创建一个线程
    while(1){
        pthread_mutex_lock(&mutex);      //加锁
        printf("HELLO ");
        sleep(rand() %3);
        printf("WORD\n");
        pthread_mutex_unlock(&mutex);  //解锁
        sleep(rand() %3);
    }
    pthread_join(tid,NULL);
    pthread_mutex_destroy(&mutex);  //销毁一个互斥锁
    return 0;
}