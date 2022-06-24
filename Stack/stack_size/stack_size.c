#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>




static int __init test_stack(void)
{
    union thread_union *test;
    int size;
    test = current->stack;
    size = sizeof(*test);
    pr_info("%d\n",size);
    return 0;

}

static void __exit test_exit(void)
{

    pr_info("end\n");
}

module_init(test_stack);
module_exit(test_exit);
MODULE_LICENSE("GPL"); 