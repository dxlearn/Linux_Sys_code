#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>


void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

int main(int argc,char *argv[])
{
    pid_t pid;
    int fd;
    pid = fork();
    if(pid > 0){
        exit(1);
    }
    setsid();
    chdir("/home/dx/Linux_Sys_code/Ucode/daemon");
    umask(0022);
    close(STDIN_FILENO);
    fd = open("/dev/null",O_RDWR);

    dup2(fd,STDOUT_FILENO);
    dup2(fd,STDERR_FILENO);
    while(1);
    return 0;
}