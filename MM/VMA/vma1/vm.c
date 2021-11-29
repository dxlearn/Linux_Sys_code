#include <linux/init.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/sched.h>

static int __init my_entry(void)
{
    unsigned long start, end, len;
    int j=0;
    struct task_struct *task;
    struct vm_area_struct *mmap,*tmp;
    task = current;
    struct mm_struct *mm;
    mm = task->mm;
    printk("%d\n",task->pid);
    if(mm == NULL)
    {
        printk("kernel thread  exit....!\n");
        return 0;
    }
    printk("No      vma       start         end       len\n");
    mmap = mm->mmap;
    tmp = mmap;
    down_read(&mm->mmap_sem);
    while(tmp)
    {
        j++;
        start = tmp->vm_start;
        end = tmp->vm_end;
        len = end - start;
        printk("%6d: %16p %12lx %12lx  %8ld\n",
                j,tmp,start,end,len);
        tmp = tmp->vm_next;

    }
    up_read(&mm->mmap_sem); 

    return 0;

}


static void __exit my_exit(void)
{
    printk("hello world!\n");

}




MODULE_LICENSE("GPL");
module_init(my_entry);
module_exit(my_exit);