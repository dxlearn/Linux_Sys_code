#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
    int pid;

    pid=fork();
    if(pid<0){
        printf("error\n");
        return 0;
    }
    //返回值为0是子进程
    if(pid==0)
    {
        printf("子进程\n");
    }
    //返回值大于0是父进程
    if (pid>0)
    {
        printf("父进程\n");
    }
    
}
