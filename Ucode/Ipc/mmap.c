#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>

void sys_err(const char *str)
{
    perror(str);
    exit(1);
}
int main(int argc,char *argv[])
{
    char *p = NULL;
    int fd;

    fd = open("testmap",O_RDWR|O_CREAT|O_TRUNC,0644);
    if(fd == -1){
        sys_err("open error");
    }
    lseek(fd,20,SEEK_END);//扩大空间
    write(fd,"\0",1);

    int len = lseek(fd,0,SEEK_END);  //把位置初始化到开始，并返回大小

    p = mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);  //建立文件映射
    if(p == MAP_FAILED){
        sys_err("mmap error");
    }
    strcpy(p,"hello mmap");
    printf("-----%s\n",p);
    return 0;
}

