#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/notifier.h>

//module_2:给module_1发通知

extern struct raw_notifier_head test_chain_head;

//Module2中的某事件
static int event(unsigned long val)
{
    int ret = raw_notifier_call_chain(&test_chain_head,val,NULL);
    return notifier_to_errno(ret);
}

static int __init my_entry(void)
{
    event(666);
    return 0;
}
static void __exit my_exit(void)
{
    printk("test end\n");
}
module_init(my_entry);
module_exit(my_exit);

MODULE_AUTHOR("Dong Xu");
MODULE_LICENSE("GPL");

