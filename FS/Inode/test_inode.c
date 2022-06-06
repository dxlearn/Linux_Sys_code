#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/fs_struct.h>
#include <linux/path.h>
#include <linux/sched.h>
#include <linux/dcache.h>
#include <linux/list.h>

MODULE_LICENSE("GPL");

static int test_inode_init(void)
{
    //获取当前文件节点的inode节点
    //fs:struct fs_struct :每个进程都有自己的根目录和当前工作目录，
    //内核使用struct fs_struct来记录这些信息，进行描述符的fs字段便是指向该进程的fs_struct结构
    
    struct dentry *dentry_pwd;
    struct dentry *root_dentry;


    dentry_pwd = current->fs->pwd.dentry;
    printk("current_task_workplace's dentry:%s   size:%lld\n",dentry_pwd->d_name.name,dentry_pwd->d_inode->i_size); //当前进程的工作目录、以及大小
    root_dentry = current->fs->root.dentry;
    printk("current_task_root_workplace'dentry:%s  size:%lld\n",root_dentry->d_name.name,root_dentry->d_inode->i_size);//当前进程的根工作目录、以及大小

    return 0;

}

static void test_exit(void)
{
    printk("---exit----\n");
}

module_init(test_inode_init);
module_exit(test_exit);
