#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int main(int agrc,char *argv[])
{
    int i;
    pid_t pid,tmppid,wpid;
    for(i = 0;i<5;i++){
        pid = fork();
        if(pid == 0) break;   //子进程直接跳出，只有父进程才fork
        if(i == 2){            //记录父进程fork出来的第二个子进程的pid
            tmppid = pid;
        }
    }
    if(i == 5){
        sleep(5);
        wpid = waitpid(tmppid,NULL,WNOHANG);//指定一个进程回收，不阻塞
        if(wpid == -1){
            perror("waitpid error");
            exit(1);
        }
        printf("I'm parent,wait 3th child finish %d\n",wpid);
    }else{
        sleep(i);
        printf("I'm %dth child,pid = %d\n",i+1,getpid());
    }
    return 0;
}