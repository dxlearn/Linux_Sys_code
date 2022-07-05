#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

int main(int argc,char *argv[])
{
    pid_t pid = fork();
    if(pid == -1){
        perror("fork error");
        exit(1);
    }else if(pid == 0){
        execlp("ls","ls","-l","-d","-h",NULL);  //函数没有返回值，除非是出错
        perror("exec error");
        exit(1);
    }else if(pid >0){
        sleep(1);
        printf("parent:%d\n",getpid());
    }
    return 0;
}