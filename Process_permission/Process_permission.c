#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//定义一个出错提示宏
#define DEATH(mess) { perror(mess); exit(errno); }



//打印资源限制
void do_limit(int limit, const char *limit_string, struct rlimit *rlim)
{
    if(getrlimit(limit,rlim))
       fprintf(stderr,"Faild in getrlimit\n");
    printf("%15s = %2d: cur = %20lu,   max = %20lu\n",limit_string, 
           limit, rlim->rlim_cur, rlim->rlim_max);

}

//设置要打印的几种资源
void print_limits(void)
{
    struct rlimit rlimt;
    do_limit(RLIMIT_CPU, "RLIMIT_CPU", &rlimt);
    do_limit(RLIMIT_FSIZE, "RLIMIT_FSIZE", &rlimt);
    do_limit(RLIMIT_DATA, "RLIMIT_DATA", &rlimt);

}

//打印当前已经使用的资源
void print_rusage(int who)
{
    struct rusage usage;
    if(getrusage(who,&usage))
       DEATH("getrusage faild");
    
    if(who == RUSAGE_SELF)
       printf("For RUSAGE_SELF\n");
    if(who == RUSAGE_CHILDREN)
       printf("\nFor RUSAGE_CHILEDREN\n");

    printf("ru_utime.tv_sec, ru_utime.tv_usec = %4d %4d (user time used)\n",
           (int)usage.ru_utime.tv_sec,(int)usage.ru_utime.tv_usec);
    printf("ru_stime.tv_sec, ru_stime.tv_usec = %4d %4d (system time used)\n",
           (int)usage.ru_stime.tv_sec, (int)(usage.ru_stime.tv_usec));
}



int main(void)
{
    struct rlimit rlim;
    pid_t pid = 0;
    int status = 0, nchildren = 3, i;

    //打印所有的资源
    printf("Printing out all limits for pid=%d:\n",getpid());
    print_limits();
    print_rusage(RUSAGE_SELF);

}















