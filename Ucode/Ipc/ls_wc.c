#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>

void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

int main(void)
{
    int fd[2];
    pid_t pid;
    int ret;
    int n = 2;

    ret = pipe(fd);  //创建管道

    if(ret == -1) sys_err("pipe error");

    //创建两个进程
    int i;
    for(i = 0; i< 2;i++){
        pid = fork();
        if(pid == -1) sys_err("fork error");
        else if(pid == 0) break;  //子进程不进入for循环
        else{
            printf("I'm parent\n");
        }
    }
    if(i == 0){  //子进程1
       close(fd[0]);//关闭读端，保留写端
       dup2(fd[1],STDOUT_FILENO); //将标准输出改到输出到fd[1]中，也就是管道的写端输入到管道
       execlp("ls","ls",NULL);
       sys_err("execlp is error");
    }
    if(i == 1){
        close(fd[1]); //关闭写端，保留读端
        dup2(fd[0],STDIN_FILENO);//将标准输入改到fd[0]中输入，也就是管道的读端输入
        execlp("wc","wc","-l",NULL);
        sys_err("execlp is error!");
    }
    if(n ==i){
        close(fd[0]);
        close(fd[1]);
        wait(NULL);
        wait(NULL);
    }

    return 0;
}