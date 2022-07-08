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


int main(void)
{
    int *p;
    int fd;
    int num;
    fd = open("test",O_RDONLY);
    if(fd < 0){
        sys_err("open error");
    }

    p = (int *)mmap(NULL,sizeof(num),PROT_READ,MAP_SHARED,fd,0);  //创建共享映射区
    if(p == MAP_FAILED){
        sys_err("mmap error");
    }
    close(fd);

    while(1){
        sleep(10);
        printf("read :%d\n",*p);
    }
        int ret = munmap(p,4);
        if(ret == -1){
            sys_err("munmap error");
        }
    return 0;

}
