#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int main(int agrc,char *argv[])
{
    int i;
    pid_t pid,wpid;
    for(i = 0;i<5;i++){
        pid = fork();
        if(pid == 0) break;   //子进程直接跳出，只有父进程才fork
    }
    if(i == 5){
        while ((wpid = waitpid(-1,NULL,0)) != -1)
        {
            printf("wait child %d\n",wpid);
        }
        
    }else{
        sleep(i);
        printf("I'm %dth child,pid = %d\n",i+1,getpid());
    }
    return 0;
}