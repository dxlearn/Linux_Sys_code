#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
void sys_err(const char *str)
{
    perror(str);
    exit(1);
}
/*
父子进程之间的通信与父子进程之间通信的区别是：
1、父子进程间通信只要关闭读端，子进程关闭写端就行
2、兄弟进程间通信，不仅要关闭兄进程的读端，弟进程关闭写端，还要父进程关闭读端和写段，保证管道的单向通信
*/
int main(int argc,char *argv[])
{
    int ret;
    int fd[2];
    pid_t pid;

    char *str = "hello pipe\n";
    char buf[1024];

    ret = pipe(fd);  //父进程创建一个读端一个写端

    if(ret == -1){
        sys_err("pipe errot");
    }
    pid = fork();
    if(pid > 0){
        close(fd[0]);
        write(fd[1],str,strlen(str));
        sleep(1);
        close(fd[1]);
    }else if(pid == 0 ){
        close(fd[1]);  //关闭写端，保证了读写端
        ret = read(fd[0],buf,sizeof(buf));
        write(STDOUT_FILENO,buf,ret);  //把子进程读到的内容写到屏幕
        close(fd[0]);  //关闭读端
    }

    return 0;
}