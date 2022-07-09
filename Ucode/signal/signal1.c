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

void print_pending(sigset_t *set){
    int i;
    for(int i = 1;i<32;i++){
        if(sigismember(set,i))   //检查未决信号集：在的话返回1 否则返回 0
         putchar('1');
        else
        putchar('0');
    }
    printf("\n");
}


int main(char argc,char *argv[])
{
    sigset_t set,oldset,pedset;  //定义信号集

    sigemptyset(&set);  //set清空
    sigaddset(&set,SIGINT);//设置自定义信号集

    sigprocmask(SIG_BLOCK,&set,&oldset); //用自定义的set设置屏蔽信号集

    
    while(1){
        sigpending(&pedset);  //获取未决信号集
        print_pending(&pedset);
        sleep(5);
        
    }
    return 0;
}
