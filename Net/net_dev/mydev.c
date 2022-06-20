#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <net/ip6_checksum.h>
#include <linux/io.h>
#include <linux/prefetch.h>
#include <linux/bitops.h>
#include <linux/if_vlan.h>
#include <asm/uaccess.h>
#include <linux/netdevice.h>
#include "e1000.h"
 
MODULE_LICENSE("GPL");
 
 
static int hello_init(void)
{
printk(KERN_ALERT "hello,This is test\n");
//net_device的地址可以通过crash 到当前系统的内核中，net命令得到
struct net_device *netdev = 0xffff928bf2e16000; 
//下面验证0xffff90d577bf2000处结构体的net_device->name正是对应的网卡
printk(KERN_ALERT "%s\n",netdev->name); 
 //能过net_device得到e1000_adapter特有数据的结构体
 struct e1000_adapter *adapter = netdev_priv(netdev);
 printk(KERN_ALERT "%p\n",adapter);
//struct e1000_rx_ring *rx_ring = adapter->rx_ring;
struct e1000_tx_ring *tx_ring = adapter->tx_ring;
打印当前的rx_ring->count，及tx_ring->count的值
printk(KERN_ALERT "rx_ring count = %u\n",rx_ring->count);
printk(KERN_ALERT "rx_ring length of descriptor of ring in bytes = %u\n",rx_ring->size);
 printk(KERN_ALERT "tx_ring count = %u\n",tx_ring->count);
printk(KERN_ALERT "tx_ring length of descriptor of ring in bytes = %u\n",tx_ring->size);
 return 0;
}
 
static void hello_exit(void)
{
 printk(KERN_ALERT "goodbye,kernel\n");
}
 
module_init(hello_init);
module_exit(hello_exit);
 
 
MODULE_AUTHOR("Jia Lei/lei.jia2@h3c.com");
MODULE_DESCRIPTION("This is a simple example!\n");
MODULE_ALIAS("Simplest example");
