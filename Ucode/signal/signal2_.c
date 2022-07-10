#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <signal.h>


void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

void sig_cath(int signo)
{
    printf("catch signal!%d\n",signo);
    return;
}

int main(int argc,char *argv[])
{
    signal(SIGINT,sig_cath);  //注册一个信号
    while(1);
    return 0;
}