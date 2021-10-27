#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include<sys/wait.h>

/*
wait():父进程一旦调用了wait就立即阻塞了自己，由wait自动分析是否当前进程的某个子进程已经退出
如果找到了一个已经变成僵尸进程的子进程，wait就回收集这个子进程的信息，并把它彻底销毁后返回
如果没有找到这样一个子进程，wait就会一直阻塞在这里，直到有一个出现为止
*/

/*
在fork()的调用处，整个父进程空间会原模原样地复制到子进程中，
包括指令，变量值，程序调用栈，环境变量，缓冲区，等等
*/
int main(void)
{
    int i;
    for(i=0;i<2;i++)
    {
        fork();
        printf("_ : ppid=%d  pid=%d\n",getppid(),getpid());
    }
    wait(NULL);
    wait(NULL);
    return 0;
}