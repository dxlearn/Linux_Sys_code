#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
    pid_t pid,wpid;
    int status;
    pid = fork();
    if(pid == 0){
        printf("---child,myparent = %d,going to sleep 10s\n",getppid());
        sleep(10);
        printf("----child die-----\n");
    }else if(pid > 0){

            wpid = wait(&status);
            if(wpid == -1){
                perror("wait error");
                exit(1);
            }
            printf("parent wait finish\n");
    }else{
            perror("fork");
            return 1;
        }
    return 0;
}