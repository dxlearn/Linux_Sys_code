#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
struct msg{
    int num;
    struct msg *next;
};
struct msg *head;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  //定义并初始化一个互斥量
pthread_cond_t has_data = PTHREAD_COND_INITIALIZER;//定义并初始化一个条件变量


void err_thread(int ret,char *str)
{
    if(ret != 0){
        fprintf(stderr,"%s:%s\n",str,strerror(ret));
        pthread_exit(NULL);
    }
}


void *produser(void *arg)
{
    while(1){
    struct msg *mp = malloc(sizeof(struct msg));
    mp->num = rand()%1000+1;  //模拟生产一个数据
    printf("produce %d\n",mp->num);
    pthread_mutex_lock(&mutex);  //加锁 互斥量
    mp->next = head;             //写操作
    head = mp;                   //写操作
    pthread_mutex_unlock(&mutex);  //解锁 互斥量
    
    pthread_cond_signal(&has_data);//唤醒阻塞在条件变量has_data上的线程
    sleep(rand() %3);
    }
    return NULL;
}


void *consumer(void *arg)
{
    while(1){
    struct msg *mp;
    pthread_mutex_lock(&mutex);  //加锁
    if(head == NULL){
        pthread_cond_wait(&has_data,&mutex);  //阻塞并等待,解锁，pthread_cond_wait返回时，重写加锁mutex
    }
    mp = head;
    head = mp->next;
    pthread_mutex_unlock(&mutex);
    printf("------consumer:%d\n",mp->num);
    free(mp);
    sleep(rand()%3);
    }
    return NULL;
}



int main(int argc,char *argv[])
{
    srand(time(NULL));
    pthread_t pid,cid;
    int ret;
    ret = pthread_create(&pid,NULL,produser,NULL);
    if(ret != 0){
        err_thread(ret,"ptrhread_create error");
    }
    ret = pthread_create(&cid,NULL,consumer,NULL);
    if(ret != 0){
        err_thread(ret,"ptrhread_create error");
    }
    pthread_join(pid,NULL);
    pthread_join(cid,NULL);
    return 0;

}

