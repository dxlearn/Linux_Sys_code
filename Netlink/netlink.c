#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <net/sock.h>
#include <linux/netlink.h>


#define NETLINK_TEST     30
#define MSG_LEN            100
#define USER_PORT        66


struct sock *nlsk;

extern struct net init_net;

static void netlink_rcv_msg(struct sk_buff *skb)
{   
    struct nlmsghdr *nlh;
    char *umsg = NULL;
    //char *kmsg = "hello users";
    //从skb中获取data字段，并转换成nlh进行读取
    nlh = nlmsg_hdr(skb);
    //读取nlh后面的数据部分
    umsg = NLMSG_DATA(nlh);
    printk("kernel recv from user: %s\n", umsg);
    printk("port id :%d\n",NETLINK_CB(skb).portid);
}
struct netlink_kernel_cfg cfg = {
    .input = netlink_rcv_msg,
};



static int __init test_netlink_init(void)
{
       = (struct sock *)netlink_kernel_create(&init_net,NETLINK_TEST,&cfg);
    return 0;

} 

static void __exit test_netlink_exit(void)
{
    printk("exit......\n");
}
MODULE_LICENSE("GPL");
module_init(test_netlink_init);
module_exit(test_netlink_exit);