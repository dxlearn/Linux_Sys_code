/*
 * @Author: Dong Xu 
 * @Date: 2021-11-27 16:13:36 
 * @Last Modified by: Dong.xu
 * @Last Modified time: 2021-11-27 16:15:09
 */
#include<linux/gfp.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/gfp.h>
#include <linux/mm_types.h>
#include <linux/mm.h>

struct page *page = NULL;
static int __init my_alloc(void)
{    
    //alloc_page返回的是分配的物理页对于的page描述符的逻辑地址
    page = alloc_pages(GFP_KERNEL,0);
    printk("page  address:0x%lx\n",(unsigned long)page);
    //page_address返回Page对应的物理页的逻辑地址
    printk("page_address(pages) = 0x%lx\n", (unsigned long)page_address(page));

    return 0;
}

static void __exit my_exit(void)
{
    printk("exit......\n");
    if(page)
    {
         __free_pages(page,1); 
    }
}


MODULE_LICENSE("GPL");
module_init(my_alloc);
module_exit(my_exit);