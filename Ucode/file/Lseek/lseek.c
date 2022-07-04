#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>

int main(void)
{
    int fd,n;
    char msg[] = "test lseek\n";
    char ch;
    fd = open("lseek.txt",O_RDWR | O_CREAT,0644);
    if(fd < 0){
        perror("open lseek.txt error");
        exit(1);
    }
    write(fd,msg,strlen(msg));
    lseek(fd,0,SEEK_SET); //读和写会同时更新下一个位置，所以要定位到文件的开始再读数据
    while ((n = read(fd,&ch,1)))
    {
        if(n<0){
            perror("read error");
            exit(1);
        }
     write(STDOUT_FILENO,&ch,n);   
    }
    close(fd);
    return 0;
    
}