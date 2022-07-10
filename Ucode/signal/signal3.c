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

void sig_catch(int signo)
{
    if(signo == SIGINT){
        printf("catch you %d\n",signo);
    }else{
        printf("--catch you %d--\n",signo);
    }
    // sleep(5);  //测试阻塞信号不主持排队，产生多次只记录一次，后32个实时信号支持
}

int main(int argc,char *argv[])
{
    struct sigaction act,oldact;
    act.sa_handler = sig_catch;  //设置回调函数
    sigemptyset(&(act.sa_mask)); //设置屏蔽字  清空，仅仅保证信号处理函数中，屏蔽其他信号
    act.sa_flags = 0;            //设置默认flag
    sigaction(SIGINT,&act,&oldact); //注册信号捕捉函数
    sigaction(SIGQUIT,&act,&oldact); //注册第二个信号捕捉函数
    while(1);
    return 0;
}