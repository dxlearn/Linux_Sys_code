#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/klog.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>


#define FALLBACK_KLOG_BUF_SHIFT 17  /* CONFIG_LOG_BUF_SHIFT in kernel */
#define FALLBACK_KLOG_BUF_LEN   (1 << FALLBACK_KLOG_BUF_SHIFT)

/* These correspond to the kernel's SYSLOG_ACTION_whatever constants. */
#define KLOG_CLOSE         0
#define KLOG_OPEN          1
#define KLOG_READ          2
#define KLOG_READ_ALL      3
#define KLOG_READ_CLEAR    4
#define KLOG_CLEAR         5
#define KLOG_CONSOLE_OFF   6
#define KLOG_CONSOLE_ON    7
#define KLOG_CONSOLE_LEVEL 8
#define KLOG_SIZE_UNREAD   9
#define KLOG_SIZE_BUFFER   10

/* we use 'Linux version' string instead of Oops in this lab */
//#define OOPS_LOG  "Oops"
#define OOPS_LOG  "Linux version"


int save_log(char *buffer)
{
    int fd;
    time_t t;
    char path[128];
    struct tm *tm;
    /*
   struct tm {
   int tm_sec;         //秒，范围从 0 到 59              
   int tm_min;         // 分，范围从 0 到 59               
   int tm_hour;        // 小时，范围从 0 到 23                
   int tm_mday;        // 一月中的第几天，范围从 1 到 31                  
   int tm_mon;         // 月份，范围从 0 到 11                
   int tm_year;        // 自 1900 起的年数                
   int tm_wday;        // 一周中的第几天，范围从 0 到 6                
   int tm_yday;        // 一年中的第几天，范围从 0 到 365                   
   int tm_isdst;       // 夏令时                         
};
    */
    t=time(0);
    tm=localtime(&t);
    snprintf(path,128,"/mnt/%d.%d.%d.%d.%d.%d.log",tm->tm_year+1900,
    tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
    printf("%s\n",path);
    fd=open(path,O_WRONLY|O_CREAT, 0644);
    if(fd<0)
    {
        printf("open error\n");
        return -1;
    }
    write(fd,buffer,strlen(buffer));
    close(fd);
    return 0;


}

int create_daemon()
{
    
    int pid;
    pid=fork();

    if(pid<0)
    {
       printf("error\n");
       exit(1);
    }

    else if(pid>0)
    {
        exit(0);
    }

    if(setsid()<0)
    {
        printf("setsid:error\n");
        exit(1);
    }

    pid=fork();
    if(pid<0){
        printf("error\n");
        exit(1);
    }
    else if(pid>0)
    {
        exit(1);
    }
    printf("pid：%d\n",getpid());
    chdir("/home/dx/");
    for(int i=0;i<3;++i)
    {
        close(i);
    }
    umask(0);
    return 0;
    
}
int check()
{
    char *buffer;
    char *p;
    int ret=-1;

    ssize_t  klog_size;
    int size;
    /*
     int klogctl(int type, char *bufp, int len);
    */

   klog_size=klogctl(KLOG_SIZE_BUFFER,0,0);
   if(klog_size<=0){
       klog_size=FALLBACK_KLOG_BUF_LEN;
   }

   buffer=malloc(klog_size);
   if(!buffer){
       return -1;
   }
    size = klogctl(KLOG_READ_ALL, buffer, klog_size);
    if(size<0)
    {
        printf("klogctl read error\n");
        goto done;
    }
    buffer[size]='\0';
    p=strstr(buffer,OOPS_LOG);
    if(p)
    {
        printf("we found %s on kernel log\n",OOPS_LOG);
        save_log(buffer);
        ret=0;

    }
    

done:
free(buffer);
return ret;

}



int main(void)
{

    //创建守护进程
    create_daemon();
    //每5s检查内核日志，并保存
    while(1){
        check();
        sleep(5);
    }
    return 0;


}