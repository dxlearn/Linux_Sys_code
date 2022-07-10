#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

void catch_child(int signo)
{
    pid_t wpid;

    //wpid = wait(NULL);  //回收子进程，NULL:无区别回收，是子进程就回收
    while((wpid = wait(NULL)) != -1){
      printf("---catch child id %d\n",wpid);
    }
    return;
}




int main(int argc,char *argv[])
{
    pid_t pid;
    sigset_t set;
    int i;
        //设置信号屏蔽阻塞
    sigemptyset(&set); //清空集合
    sigaddset(&set,SIGCHLD); //设置SIGCHILD位
    sigprocmask(SIG_BLOCK,&set,NULL);//使用set做阻塞

    for(i =0;i<5;i++){
        if((pid = fork()) == 0){ //子进程不参加for循环，直接退出
            break;
        }
    }
    if(i == 5){   //父进程 ：注册信号处理
        struct sigaction act;
        act.sa_handler = catch_child;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
        sigaction(SIGCHLD,&act,NULL);   //注册信号

       //解除阻塞
       sigprocmask(SIG_UNBLOCK,&set,NULL);


        while(1);
    }else{
        printf("child id:%d\n",getpid());
    }
    return 0;
}