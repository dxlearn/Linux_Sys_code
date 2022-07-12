#include <linux/init.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/mm_types.h>

static int __init my_entry(void)
{
    struct page test_page;
    long size = sizeof(test_page);
    printk("page size:%lu\n",size);
    return 0;

}
static void __exit my_exit(void)
{
    printk("test size exit\n");

}
MODULE_LICENSE("GPL");
module_init(my_entry);
module_exit(my_exit);
