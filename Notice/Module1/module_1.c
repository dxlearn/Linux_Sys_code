#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/notifier.h>

//module_1:  被通知者，设置通知函数
//定义通知链
static RAW_NOTIFIER_HEAD(test_chain_head);
EXPORT_SYMBOL_GPL(test_chain_head);

//通知块1的执行函数
static int A_call(struct notifier_block *nb, unsigned long event, void *v)
{

    printk("AAAAAAAAAA---------event_A occur!---------AAAAAAAAAA\n");
    printk("my priority:%d\n",nb->priority);
    return NOTIFY_DONE;
} 

static struct notifier_block testA = {
    .notifier_call = A_call,
    .priority = 7,
};

//通知块2的执行函数
static int B_call(struct notifier_block *nb, unsigned long event, void *v)
{

    printk("BBBBBBBBBB---------event_B occur!---------BBBBBBBBB\n");
    printk("my priority:%d\n",nb->priority);
    return NOTIFY_DONE;
} 
static struct notifier_block testB = {
    .notifier_call = B_call,
    .priority = 9,
};

//通知块1的执行函数
static int C_call(struct notifier_block *nb, unsigned long event, void *v)
{
    printk("CCCCCCCCCC---------event_c occur!---------CCCCCCCCCC\n");
    printk("my priority:%d\n",nb->priority);
    return NOTIFY_DONE;

}
static struct notifier_block testC = {
    .notifier_call = C_call,
    .priority = 6,
};


static int __init my_register(void)
{
    printk("----------register notice chain---------\n");
    raw_notifier_chain_register(&test_chain_head,&testA);
    raw_notifier_chain_register(&test_chain_head,&testB);
    raw_notifier_chain_register(&test_chain_head,&testC);
    printk("----------register notice chain done---------\n");
    //遍历已经注册的调用链
	struct notifier_block *nb, *next_nb;
    struct raw_notifier_head *tmp = &test_chain_head;
    struct notifier_block *head = tmp->head;
	nb = rcu_dereference_raw(head);
    printk("----Test registed notice call----\n");
	while (nb) {
        int ret = NOTIFY_DONE;
        int index=0;
		next_nb = rcu_dereference_raw(nb->next);
        printk("notice%d fun:%p,priority:%d",++index,nb->notifier_call,nb->priority);
		ret = nb->notifier_call(nb, 1, NULL);          //调用注册的回调函数
		if ((ret & NOTIFY_STOP_MASK) == NOTIFY_STOP_MASK)  //有停止的mask就返回，否则继续
			break;
		nb = next_nb;
	}
   printk("--------------Module_1 test end-------------\n");
    return 0;
}
static void __exit my_unregister(void)
{
    raw_notifier_chain_unregister(&test_chain_head,&testA);
    raw_notifier_chain_unregister(&test_chain_head,&testB);
    raw_notifier_chain_unregister(&test_chain_head,&testC);
}


module_init(my_register);
module_exit(my_unregister);

MODULE_AUTHOR("Dong Xu");
MODULE_LICENSE("GPL");

