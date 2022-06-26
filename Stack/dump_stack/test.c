#include <linux/module.h>   
#include <linux/kernel.h>
#include <linux/init.h>

 
void aaa(int a);
void bbb(int b);
void ccc(int c);
 
void aaa(int a)
{
	int b = a + 10;
	bbb(b);
}
 
void bbb(int b)
{
	int c = b + 10;
	ccc(c);
}
 
void ccc(int c)
{
	dump_stack();
	printk("c is %d\n",c);
}
 
static int __init my_init( void )
{
	int a = 10;
	aaa(a);
	printk("my_init \n");  
    return 0;
}
 
static void __exit my_exit(void )
{
    	printk("my_exit \n");	
}
 
module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL"); 