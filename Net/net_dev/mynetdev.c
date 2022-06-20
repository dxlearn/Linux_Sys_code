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

 static int test_entry(void)
 {
     struct net_device *netdev;
     struct e1000_adapter *adapter;
     struct e1000_rx_ring *rx_ring;
     struct e1000_tx_ring *tx_ring;
     printk(KERN_INFO"Test My_Net_Device\n");
    //通过crash获取当前系统内核中，网卡设备的地址0xffff928bf2e16000
     netdev = (struct net_device*)0xffff928bf2e16000;
     printk(KERN_INFO"this dev_name :%s\n",netdev->name);
     adapter = netdev_priv(netdev);
     printk(KERN_INFO"adapter address:%p\n",adapter);
     rx_ring = adapter->rx_ring;
     tx_ring = adapter->tx_ring;
     //测试当前网卡Ring Buffer的大小
     printk(KERN_INFO"RX Ring Buffer Count:%u\n",rx_ring->count);
     printk(KERN_INFO"TX Ring Buffer Count:%u\n",tx_ring->count);
     printk(KERN_INFO"RX Ring Buffer Size(Bytes):%u\n",rx_ring->size);
     printk(KERN_INFO"RX Ring Buffer Size(Bytes):%u\n",tx_ring->size);
     return 0;
 }

 
static void test_exit(void)
{
 printk(KERN_INFO "goodbye,kernel\n");
}
 
module_init(test_entry);
module_exit(test_exit);
 