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
    int num = 100;
    fd = open("test",O_RDWR|O_CREAT|O_TRUNC,0644);
    if(fd < 0){
        sys_err("open error");
    }
    ftruncate(fd,sizeof(num));

    p = (int *)mmap(NULL,sizeof(num),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);  //创建共享映射区
    if(p == MAP_FAILED){
        sys_err("mmap error");
    }
    close(fd);

    while(1){
        memcpy(p,&num,sizeof(num));
        num++;
        sleep(5);
    }
        int ret = munmap(p,sizeof(num));
        if(ret == -1){
            sys_err("munmap error");
        }
    return 0;

}
