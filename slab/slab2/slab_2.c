#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/gfp.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("viz.xu, xujiwei@163.com");

struct slab_test{
    int val;
    int val2;
    int val3;
};

static int num;
struct kmem_cache *test_cachep = NULL;
struct slab_test *object1 = NULL, *object2 = NULL;

void slab_ctor(void *cachep){
    printk("slab_ctor is called! object %d has been inited!\n", num);
    num++;
}


static int __init kmem_cache_create_init(void){
    printk("slab test module init\n");
    num = 0;
    printk("%ld\n",sizeof(struct slab_test));
    //创建slab分配器:test_cachep
    test_cachep = kmem_cache_create("slab_test_cachep", sizeof(struct slab_test), 0, SLAB_HWCACHE_ALIGN, slab_ctor);
    if(!test_cachep)
        return -ENOMEM;
    //从slab分配器中分配一个结构体test_cachep
    object1 = kmem_cache_alloc(test_cachep, GFP_KERNEL);
    if(!object1)
        return -ENOMEM;
    else
        printk("object one has been created!\n");
    //再次分配
    object2 = kmem_cache_alloc(test_cachep, GFP_KERNEL);
    if(!object2)
        return -ENOMEM;
    else
        printk("object two has been created!\n");
    return 0;
}

static void __exit kmem_cache_create_exit(void){
    printk("slab test module exit\n");
    //释放slab分配器
    kmem_cache_free(test_cachep, object1);
    kmem_cache_free(test_cachep, object2);
    if(test_cachep)
        kmem_cache_destroy(test_cachep);
}

module_init(kmem_cache_create_init);
module_exit(kmem_cache_create_exit);