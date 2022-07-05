#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int fd;
    fd = open("ps.txt",O_WRONLY|O_CREAT|O_TRUNC,0644);
    if(fd < 0){
        perror("open ps.out error");
        exit(1);
    }
    dup2(fd,STDOUT_FILENO); //将输出保存到标准文件中
    execlp("ps","ps","ax",NULL);
    perror("exevlp error");
    return 0;
}