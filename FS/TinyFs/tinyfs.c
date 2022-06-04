#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#include "tinyfs.h"


struct file_blk block[MAX_FILES+1];

//文件计数
int curr_count = 0;

//获取一个尚未使用的文件块，保存新创建的文件或目录
static int get_block(void)
{
    int i;
    for(int i = 2;i < MAX_FILES; i++){
        if(!block[i].busy){
            block[i].busy = 1;
            return i;
        }
    }
    return i;
}
//创建文件
static int tiny_do_create(struct inode *dir, struct dentry *dentry, umode_t mode,
		       bool excl){
    
    struct inode *inode;
    struct super_block *sb;
    struct dir_entry *entry;
    struct file_blk *blk,*pblk;
    int idx;
    sb = dir->isb;

}

static struct inode_operations tinyfs_inode_ops = {
    .create = 
    .lookup =
    .mkdir = 
    .rmdir =
    .unlink =
}