#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>


int main(void)
{
    char buf[10];
    int fd,n;
    fd = open("/dev/tty",O_RDONLY | O_NONBLOCK);  //非阻塞方式
    if(fd < 0){
        perror("open /dev/tty");
        exit(1);
    }

tryagain:
    n = read(fd,buf,10);   //读不到  立即返回 ---非阻塞
    if(n < 0){
        if(errno != EAGAIN){       //读错误---并不是非阻塞导致的，直接返回
            perror("read /dev/tty");
            exit(1);
        }else{                    //非阻塞，循环读
            write(STDOUT_FILENO,"try again\n",strlen("try again\n"));
            sleep(2);
            goto tryagain;
        }
    }
    write(STDOUT_FILENO,buf,n);
    close(fd);
    return 0;
}