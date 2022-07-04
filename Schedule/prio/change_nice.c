#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>

#define JMAX (4000*100000)


#define GET_ELAPSED_TIME(tv1,tv2) (\
  (double) ((tv2.tv_sec - tv1.tv_sec) \
  + .000001 * (tv2.tv_usec - tv1.tv_usec)))


double do_something (void)
{
    int j;
    double x = 0.0;
    struct timeval tv1, tv2;
    gettimeofday (&tv1, NULL);//获取时区
    for (j = 0; j < JMAX; j++)
        x += 1.0 / (exp ((1 + x * x) / (2 + x * x)));
    gettimeofday (&tv2, NULL);
    return GET_ELAPSED_TIME (tv1, tv2);//求差值
}



int main(int argc,char *argv[])
{
    int niceval = 0,nsched;
    // printf("long long %ld\n",sizeof(long long));
    // printf("long %ld\n",sizeof(long));
    // printf("void * %ld\n",sizeof(void *));
    long scale = 1000;
    
    long ticks_cpu,ticks_sleep;
    pid_t pid;

    FILE *fp;
    char fname[256];
    double elapsed_time,timeslice,t_cpu,t_sleep;

    if(argc > 1){
        niceval = atoi(argv[1]);
    }
    pid = getpid();
    if(argc > 2){
        scale = atoi(argv[2]);
    }
    sleep(30);

    sprintf(fname,"/proc/%d/schedstat",pid); //读取进程的调度状态
    if(!(fp = fopen(fname,"r"))){
        printf("Failed to open stat file\n");
        exit(1);
    }
    if(nice(niceval) == -1 && niceval != -1){
        printf("Failed to set nice to %d\n",niceval);
        exit(-1);
    }
    elapsed_time = do_something(); //执行时间

    fscanf(fp,"%ld %ld %d",&ticks_cpu,&ticks_sleep,&nsched);
    t_cpu = (float)ticks_cpu / scale;
    t_sleep = (float)ticks_sleep / scale;
    
    timeslice = t_cpu / (double)nsched; //除以调度的次数：每次调度的时间
    printf ("\nnice=%3d time=%8g secs pid=%5d"
            "  t_cpu=%8g  t_sleep=%8g  nsched=%5d"
            "  avg timeslice = %8g\n",
            niceval, elapsed_time, pid, t_cpu, t_sleep, nsched, timeslice);
    fclose(fp);

    exit(0);
}

//gcc change_nice.c -o change_nice -lm 