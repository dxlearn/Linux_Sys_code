#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/freezer.h>
#include <linux/mutex.h>
#include <linux/delay.h>


static void my_timefunc(struct timer_list *unused);
//定义一个定时器
static DEFINE_TIMER(my_timer,my_timefunc);
static int count = 0;
static atomic_t flags;
wait_queue_head_t wait_head;

static void my_timefunc(struct timer_list *unused)
{
    atomic_set(&flags,1);
    wake_up_interruptible(&wait_head);
    mod_timer(&my_timer,jiffies+msecs_to_jiffies(2000));
}

static void my_try_to_sleep(void)
{
    //创建队列成员-thread
    DEFINE_WAIT(wait);
    if(freezing(current) || kthread_should_stop())
       return;
    //wait 加入到等待队列
    prepare_to_wait(&wait_head,&wait,TASK_INTERRUPTIBLE);

    if(!atomic_read(&flags))
      schedule();

    finish_wait(&wait_head,&wait);;
}

static void show_reg(void)
{
    struct task_struct *task = current;
    printk("%s: %s,count:%d,pid :%d\n",__func__,task->comm,count,task->pid);
}



static int my_thread(void *nothing){
    set_freezable();
    set_user_nice(current,0);

    while(!kthread_should_stop()){
        my_try_to_sleep();
        atomic_set(&flags,0);
        count++;
        show_reg();
    }
    return 0;
}


static struct task_struct *thread;

static int __init my_init(void)
{
    printk("ben: my lockdep module init\n");

    //创建一个线程
    thread  = kthread_run(my_thread,NULL,"ktest");

    //设置定时器
    my_timer.expires = jiffies + msecs_to_jiffies(500);
    add_timer(&my_timer);

    init_waitqueue_head(&wait_head);
    return 0;

}

static void __exit my_exit(void)
{
    printk("end\n");
    kthread_stop(thread);
}
MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);