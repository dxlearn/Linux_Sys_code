#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define ERROR(mess) { perror(mess); exit(errno); }

int main(void){
    pid_t pid;
    int priority;

    pid = fork();
    if (pid < 0)
    ERROR("FORK");
    if(pid > 0)
    {
        priority = getpriority(PRIO_PROCESS,0);
          //nice 值越小 进程的优先级越高
        printf("Current process's priority(20-nice) :%d\n",priority);
        setpriority(PRIO_PROCESS,0,1);
        printf("After Modification:Current process's priority(20-nice):");
        priority = getpriority(PRIO_PROCESS,0);
        printf("%d\n",priority);


    }
  
    return 0;


}