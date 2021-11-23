#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/notifier.h>

//定义通知链
static RAW_NOTIFIER_HEAD(test_chain_head);

//通知块1的执行函数
static int test1_call(struct notifier_block *nb, unsigned long event, void *v)
{

    printk("event1 occur!\n");
    printk("priority:%d\n",nb->priority);
    return NOTIFY_DONE;
} 

static struct notifier_block test1 = {
    .notifier_call = test1_call,
    .priority = 1,
};

//通知块2的执行函数
static int test2_call(struct notifier_block *nb, unsigned long event, void *v)
{

    printk("event2 occur!\n");
    printk("priority:%d\n",nb->priority);
    return NOTIFY_STOP_MASK;
} 
static struct notifier_block test2 = {
    .notifier_call = test2_call,
    .priority = 9,
};


static int __init my_register(void)
{
    raw_notifier_chain_register(&test_chain_head,&test1);
    raw_notifier_chain_register(&test_chain_head,&test2);
    printk("----------call notice---------\n");
    raw_notifier_call_chain(&test_chain_head,0,NULL);

    return 0;
}
static void __exit my_unregister(void)
{
    raw_notifier_chain_unregister(&test_chain_head,&test1);
    raw_notifier_chain_unregister(&test_chain_head,&test2);

}

MODULE_LICENSE("GPL");
module_init(my_register);
module_exit(my_unregister);





