/*
 * @Author: Dong Xu 
 * @Date: 2021-11-27 16:30:01 
 * @Last Modified by: Dong.xu
 * @Last Modified time: 2021-11-27 17:27:17
 */
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/vmalloc.h>


static int __init my_alloc(void)
{   
    size_t size;
    int order;
    char *kbuf,*gbuf;
    unsigned long addr;

    /* kmalloc的底层也是依赖_get_free_page()实现
       kmalloc第一个参数是分配块的大小(以字节为单位)，第二个参数为分配标志，用于控制kmalloc的行为
       kmalloc申请的是较小的连续的物理内存，内存物理地址上连续，虚拟地址上也是连续的，使用的是slab的一片
       申请的内存位于物理内存的映射区域，其真正的物理地址只相差一个固定的偏移，kmalloc使用kfree函数释放内存
    */
    printk("************Kmalloc**************\n");
    for(size = PAGE_SIZE, order = 0; order < MAX_ORDER; size *= 2,order++){
       kbuf = kmalloc(size,GFP_KERNEL);
    printk("--------------order=%d,size=%ld-------------------\n",order,size);
    if(!kbuf)
    {
        printk("alloc error\n");
    }
    printk("kbuf addrss :0x%lx\n",(unsigned long)kbuf);
    addr = virt_to_phys(kbuf);
    printk("kbuf actual address :0x%lx\n",addr);
    printk("test : kbuf address:0x%lx\n",phys_to_virt(addr));
    kfree(kbuf);
   }
   /*
   get_free_page()申请的内存是一整页
   */
   printk("**********get_free_page************\n");
   for(size = PAGE_SIZE,order = 0; order < MAX_ORDER; size *= 2,order++){
       gbuf = (char *)__get_free_pages(GFP_K ERNEL,order);
       printk("--------------order=%d,size=%ld-------------------\n",order,size);
    if(!gbuf)
    {
        printk("alloc error\n");
    }
    printk("gbuf addrss :0x%lx\n",(unsigned long)gbuf);
    addr = virt_to_phys(gbuf);
    printk("gbuf actual address :0x%lx\n",addr);
    printk("test : gbuf address:0x%lx\n",phys_to_virt(addr));
    free_pages((unsigned long)gbuf,order);
   }
   
  return 0;

}
static void __exit my_exit(void)
{
    printk("--exit--\n");

}

MODULE_LICENSE("GPL");
module_init(my_alloc);
module_exit(my_exit);


