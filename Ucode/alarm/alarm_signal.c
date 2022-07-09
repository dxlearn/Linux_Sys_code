#include <stdio.h>
#include <sys/time.h>
#include <signal.h>


void myfunc(int signo)
{
    printf("test\n");
}

int main(void)
{
    struct itimerval it,oldit;
    signal(SIGALRM,myfunc);  //注册SIGALRM信号的捕捉处理函数

    it.it_value.tv_sec = 2;   //设置定时器时间
    it.it_value.tv_usec = 0;

    it.it_interval.tv_sec = 5; //设置时间间隔
    it.it_interval.tv_usec = 0;

    if(setitimer(ITIMER_REAL,&it,&oldit) == -1){
        perror("settimer error");
        return -1;
    }
    while(1);
    return 0;

}