#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/delay.h>


/*
内核线程：独立运行在内核空间的线程，和普通用户进程的区别在于内核线程没有
独立的进程地址空间即task_struct数据结构中mm指针设置为NULL，只能运行
在内核空间，和普通进程一样参与到系统的调度中。


进程地址空间又分两部分, 进程的用户地址空间, 和内核地址空间. 
其中用户地址空间是每个进程私有的, 但内核地址空间则是所有进程共享的, 
也就是说所有进程的虚拟地址空间中, 内核空间的那一部分, 都是一样的.
但内核态线程不是这样, 它只有大家都共享的内核态地址空间这一部分, 
没有用户态地址空间, 它的整个生命周期中都是完全运行于内核态的地址空间中的.
*/

int count=0;
static struct task_struct *tsk[NR_CPUS];


static int thread_run(void *data)
{
  int pid;
  struct task_struct *task;
  task=current;
  pid=task->pid;
  pr_info("this is %dth kernel_thread,pid=%d",++count,pid);
  if(count==1)
  {
      msleep(100000);
  }

   return 0;
   
}


static int __init kernel_thread_init(void)
{

    int i;
    //遍历所有当前启用并可供计划程序使用的cpu
    for_each_online_cpu(i)
        {
             pr_info("<1> onlie cpu :%d\n", i);
                //创建内核线程
            tsk[i]=kthread_create(thread_run,NULL,"kthread-demo");
               if(!tsk[i]){
              return 0;
               }
        //绑定cpu
        kthread_bind(tsk[i],i);
        //调用wake_up_process内核线程开始运行
        wake_up_process(tsk[i]);
        }

}

static void __exit kernel_thread_exit(void)
{
    printk("exit module!\n");

}



module_init(kernel_thread_init);
module_exit(kernel_thread_exit);
MODULE_LICENSE("GPL");