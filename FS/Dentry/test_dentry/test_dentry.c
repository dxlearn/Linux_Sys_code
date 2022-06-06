#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/fs_struct.h>
#include <linux/path.h>
#include <linux/sched.h>
#include <linux/dcache.h>
#include <linux/list.h>

MODULE_LICENSE("GPL");

static int test_dentry_init(void)
{
    //获取当前文件节点的inode节点
    //fs:struct fs_struct :每个进程都有自己的根目录和当前工作目录，
    //内核使用struct fs_struct来记录这些信息，进行描述符的fs字段便是指向该进程的fs_struct结构
    
    struct dentry *dentry_pwd;
    struct dentry *root_dentry;
    struct qstr name;
    struct dentry *dentry_parent;
    struct dentry *dentry;;



    dentry_pwd = current->fs->pwd.dentry;
    printk("current_task_workplace's dentry:%s\n",dentry_pwd->d_name.name); //当前进程的工作目录
    root_dentry = current->fs->root.dentry;
    printk("current_task_root_workplace'dentry:%s\n",root_dentry->d_name.name);//当前进程的根工作目录

    //dentry的名字结构是qstr
    name = current->fs->pwd.dentry->d_name;


    //获取当前进程工作目录的上一级
    dentry_parent = current->fs->pwd.dentry->d_parent;
    printk("upper's dentry:%s\n",dentry_parent->d_name.name);

    //指定dentry名字：使用d_alloc分配一个dentry
    dentry = d_alloc(dentry_parent,&name);
    printk("After d_alloc the dentry is %s\n",dentry->d_name.name);
    return 0;

}

static void test_exit(void)
{
    printk("---exit----\n");
}

module_init(test_dentry_init);
module_exit(test_exit);
