#define _GNU_SOURCE //注意，这个宏必须在最前面，否则编译会报错
#include <sched.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/select.h>
#include <stdlib.h>

int i=0;


int print_child(void *arg)
{
    while(1){
        printf("i=%d\n",i);
        i++;
        if(i==20)
        break;
    }
    return 0;
}
int main(void)
{
    int mask,tid,ret;
    char *stack;
    stack=malloc(4096);
    /*
     CLONE_VM:父子进程共享进程地址空间
     CLONE_VFORK：父进程创建子进程时，父进程进入睡眠等待，直到子进程调用execve()或exit()释放内存资源
     CHILD_SETTID:设置子进程的TID
    */
    mask = CLONE_VM|CLONE_VFORK|CLONE_CHILD_SETTID;
    //stack+4096:栈地址向下增长，所以stack+4096是首地址
    ret=clone(print_child,stack+4096,mask,NULL,NULL,NULL,&tid);
    if(ret<0)
    {
        printf("error\n");
        return 0;
    }
    printf("Create Children Success pid:%d  i=%d\n",tid,i);
    exit(0);
    sleep(10);
    printf("10s After,i:%d\n",i);

}


