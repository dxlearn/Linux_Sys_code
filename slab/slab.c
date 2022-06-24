#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/mm.h>

static char *kbuf;
static struct kmem_cache *my_cache;



static int __init create(void)
{

    //创建slab描述符
    my_cache=kmem_cache_create("test_obj",20,8,(SLAB_RECLAIM_ACCOUNT),NULL);
    //通过创建的slab，分配一个缓存对象kbuf
    kbuf=kmem_cache_alloc(my_cache,GFP_ATOMIC);
    //test dump_stack()
    //dump_stack();
    if(!kbuf)
    {
        pr_err("failed to create a cache object\n");
        //释放slab描述符
        kmem_cache_destroy(my_cache);
    }
    pr_info("successfully create a object,kbuf_addr=0x%p\n",kbuf);
    return 0;

}

static void __exit my_exit(void)
{
    //释放分配的缓存对象kbuf
    kmem_cache_free(my_cache,kbuf);
    //释放创建的slab
    kmem_cache_destroy(my_cache);
    pr_info("destory mycache\n");
}

module_init(create);
module_exit(my_exit);
MODULE_LICENSE("GPL"); 