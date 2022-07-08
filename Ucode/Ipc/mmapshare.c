#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>


void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

int var = 10;

int main(void)
{
    int *p;
    pid_t pid;

    int fd;
    fd = open("temp",O_RDWR|O_CREAT|O_TRUNC,0644);
    if(fd < 0){
        sys_err("open error");
    }
    ftruncate(fd,4);

    p = (int *)mmap(NULL,4,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);  //创建共享映射区
    if(p == MAP_FAILED){
        sys_err("mmap error");
    }
    close(fd);
    pid = fork();
    if(pid == 0){
        *p = 2000;   //传递消息：修改数值
        var = 1000;
        printf("child=*p = %d,var =%d\n",*p,var);
    }else{
        sleep(1);               //父进程等待子进程 传递消息
        printf("parent,*p = %d,var = %d\n",*p,var);
        wait(NULL);

        int ret = munmap(p,4);
        if(ret == -1){
            sys_err("munmap error");
        }
    }
    return 0;

}
