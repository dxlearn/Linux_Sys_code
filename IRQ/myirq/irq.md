# 中断相关总结
中断子系统讲解：
https://blog.csdn.net/weixin_41028621/category_8797155.html

http://wiki.100ask.org/index.php?title=%E7%AC%AC%E4%BA%94%E8%AF%BE._%E4%B8%AD%E6%96%AD%E7%B3%BB%E7%BB%9F%E4%B8%AD%E7%9A%84%E8%AE%BE%E5%A4%87%E6%A0%91&variant=zh

https://www.cnblogs.com/arnoldlu/category/1117988.html

三大结构体：https://www.cnblogs.com/lifexy/p/7506504.html
yingjian:https://blog.csdn.net/xiaojsj111/article/details/14129661

## 一些关键词
异步中断：中断是异步产生的，不是由CPU执行程序产生的，中断属于异常的一种，中断是唯一与CPU运行无关的异常

硬件中断号：hwirq
虚拟中断号：virq
当中断发生时，首先获取触发中断的HW ID，然后通过irq domin译码成IRQ number



中断的一个大体执行流程：
1、中断入口与现场保护
2、中断服务程序
3、中断返回与现场恢复

IRQ number:CPU需要为每一个外设中断编号，称之IRQ Number，这个IRQ number是一个虚拟的interrupt ID，和硬件无关，仅仅是被CPU用来标识一个外设中断.

中断描述符：
在Linux kernerl中，对于每一个外设的IRQ都用struct irq_desc来描述，称之为中断描述符(struct irq_desc)
中断控制器可以向CPU发出多种中断，每一种中断的处理方式不同，就通过irq_desc[NR_IRQS]来对应中断的：
![](../img/irq_desc%5BNR_IRQS%5D.png)


通用中断处理流程：
![](../img/interrpt_process.png)
```c
/**
 * struct irq_desc - interrupt descriptor
 * @irq_common_data:	per irq and chip data passed down to chip functions
 * @kstat_irqs:		irq stats per cpu
 * @handle_irq:		highlevel irq-events handler
 * @preflow_handler:	handler called before the flow handler (currently used by sparc)
 * @action:		the irq action chain
 * @status:		status information
 * @core_internal_state__do_not_mess_with_it: core internal status information
 * @depth:		disable-depth, for nested irq_disable() calls
 * @wake_depth:		enable depth, for multiple irq_set_irq_wake() callers
 * @irq_count:		stats field to detect stalled irqs
 * @last_unhandled:	aging timer for unhandled count
 * @irqs_unhandled:	stats field for spurious unhandled interrupts
 * @threads_handled:	stats field for deferred spurious detection of threaded handlers
 * @threads_handled_last: comparator field for deferred spurious detection of theraded handlers
 * @lock:		locking for SMP
 * @affinity_hint:	hint to user space for preferred irq affinity
 * @affinity_notify:	context for notification of affinity changes
 * @pending_mask:	pending rebalanced interrupts
 * @threads_oneshot:	bitfield to handle shared oneshot threads
 * @threads_active:	number of irqaction threads currently running
 * @wait_for_threads:	wait queue for sync_irq to wait for threaded handlers
 * @nr_actions:		number of installed actions on this descriptor
 * @no_suspend_depth:	number of irqactions on a irq descriptor with
 *			IRQF_NO_SUSPEND set
 * @force_resume_depth:	number of irqactions on a irq descriptor with
 *			IRQF_FORCE_RESUME set
 * @rcu:		rcu head for delayed free
 * @kobj:		kobject used to represent this struct in sysfs
 * @request_mutex:	mutex to protect request/free before locking desc->lock
 * @dir:		/proc/irq/ procfs entry
 * @debugfs_file:	dentry for the debugfs file
 * @name:		flow handler name for /proc/interrupts output
 */
struct irq_desc {
	struct irq_common_data	irq_common_data;
	struct irq_data		irq_data;
	unsigned int __percpu	*kstat_irqs;
	irq_flow_handler_t	handle_irq;
#ifdef CONFIG_IRQ_PREFLOW_FASTEOI
	irq_preflow_handler_t	preflow_handler;
#endif
	struct irqaction	*action;	/* IRQ action list */
	unsigned int		status_use_accessors;
	unsigned int		core_internal_state__do_not_mess_with_it;
	unsigned int		depth;		/* nested irq disables */
	unsigned int		wake_depth;	/* nested wake enables */
	unsigned int		irq_count;	/* For detecting broken IRQs */
	unsigned long		last_unhandled;	/* Aging timer for unhandled count */
	unsigned int		irqs_unhandled;
	atomic_t		threads_handled;
	int			threads_handled_last;
	raw_spinlock_t		lock;
	struct cpumask		*percpu_enabled;
	const struct cpumask	*percpu_affinity;
#ifdef CONFIG_SMP
	const struct cpumask	*affinity_hint;
	struct irq_affinity_notify *affinity_notify;
#ifdef CONFIG_GENERIC_PENDING_IRQ
	cpumask_var_t		pending_mask;
#endif
#endif
	unsigned long		threads_oneshot;
	atomic_t		threads_active;
	wait_queue_head_t       wait_for_threads;
#ifdef CONFIG_PM_SLEEP
	unsigned int		nr_actions;
	unsigned int		no_suspend_depth;
	unsigned int		cond_suspend_depth;
	unsigned int		force_resume_depth;
#endif
#ifdef CONFIG_PROC_FS
	struct proc_dir_entry	*dir;
#endif
#ifdef CONFIG_GENERIC_IRQ_DEBUGFS
	struct dentry		*debugfs_file;
	const char		*dev_name;
#endif
#ifdef CONFIG_SPARSE_IRQ
	struct rcu_head		rcu;
	struct kobject		kobj;
#endif
	struct mutex		request_mutex;
	int			parent_irq;
	struct module		*owner;
	const char		*name;
} ____cacheline_internodealigned_in_smp;
```
系统中每一个连接外设的中断线(irq request line)用一个中断描述符来描述，每一个外设的interrupt request line分配一个中断号(irq number),系统中有多少个中断线(中断源)就有多少个中断描述符(struct irq_desc)。NR_IRQS定义了硬件平台IRQ的最大数目。中断描述符的组织方式一般有两种：
1、radix-tree方式，这是以基数树的方法来组织irq_desc;
2、数组的方式，在系统初始化的时候定义一个全局数组
全局数组的方式容易导致内存空间的浪费

https://blog.csdn.net/weixin_41028621/article/details/101753159

中断的函数执行流程：
https://www.cnblogs.com/arnoldlu/p/8659981.html

## request_irq
实现动态地申请注册一个中断，根据传入的irq号获得数组irq_desc中以irq未下标的元素，然后动态地创建一个irqaction的描述符，根据传入的参数初始化新生成的irqaction描述符，最后调用函数_setup_irq()把该描述符假如到IRQ链表中，完成中断的动态申请及注册。
```c
//中断注册API
//irq是中断编号、myhandler是中断处理函数、flages是中断类型，name是设备名称、dev指设备
request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,const char *name, void *dev)
{
	return request_threaded_irq(irq, handler, NULL, flags, name, dev);
}
```
关于抢占：
1、硬中断可以被另一个优先级比自己高的硬中断“中断”，不能被同级(同一种硬中断)或低级硬中断“中断”，更不能被软中断“中断”。
2、软中断可以被硬中断“中断”，但是不会被另一个软中断“中断”，在一个CPU上，软中断总是串行执行，所以单处理器上，对软中断的数据结构进行访问不需要加任何同步原语。

硬中断和软中断(只要是中断上下文)，执行的时候都不允许内核抢占，换句话说，中断上下文中永远不允许进程切换。（个人理解，由于中断处理程序都需要较快地完成，而且中断处理程序可以嵌套，因此中断处理程序必须不能阻塞，否则性能就非常不能保证了。


 中断处理的流程：(https://blog.csdn.net/LuckyDog0623/article/details/120912442)
①发生中断时，cpu响应执行异常向量表__vectors_start  
②接着到--》vector_irq中断代码（在这里会计算返回值，保存一些寄存器，进入管理模式）
③vector_irq最后会调用中断处理总入口asm_do_IRQ
④asm_do_IRQ根据中断号调用irq_desc[??]数组项的处理函数handle_irq
⑤handle_irq接着会调用irq_desc[??]中chip成员设置硬件，比如清除中断，禁止中断，重新使能中断
⑥handle_irq会逐个调用action成员链表中注册的处理函数

x86 process:
https://blog.csdn.net/yin262/article/details/53994699

!!!!!!!!!!!net_irq:https://jishuin.proginn.com/p/763bfbd69efd
yi chang xiang liang biao :https://blog.csdn.net/qq_17270067/article/details/104974601
IDT:https://blog.csdn.net/qq_41208289/article/details/106012230?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-1-106012230-blog-47138979.pc_relevant_aa&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-1-106012230-blog-47138979.pc_relevant_aa&utm_relevant_index=2

https://ty-chen.github.io/linux-kernel-interrupt/



from 0:https://blog.51cto.com/u_15169172/2710701


good process:http://t.zoukankan.com/lifexy-p-7506504.html

best:https://zhuanlan.zhihu.com/p/511846183

讲到Generic interrupt layer:https://cloud.tencent.com/developer/article/1517862

好文：关于do_irq还是irq_handler
https://blog.csdn.net/weixin_42135087/article/details/107932658

http://t.zoukankan.com/x2i0e19linux-p-11718602.html
https://zhuanlan.zhihu.com/p/90074320

https://blog.csdn.net/weixin_42135087/article/details/123193379


NAPI:https://baijiahao.baidu.com/s?id=1712167031976582525&wfr=spider&for=pc
NAPI:https://www.jianshu.com/p/7d4e36c0abe8
Best NAPI:https://blog.csdn.net/Rong_Toa/article/details/109401935
NAPI:https://sniffer.site/2020/05/12/%E4%BB%8Enapi%E8%AF%B4%E4%B8%80%E8%AF%B4linux%E5%86%85%E6%A0%B8%E6%95%B0%E6%8D%AE%E7%9A%84%E6%8E%A5%E6%94%B6%E6%B5%81%E7%A8%8B/

NAPI和非NAPI:https://chengqian90.com/Linux%E5%86%85%E6%A0%B8/NAPI-%E9%9D%9ENAPI%E6%94%B6%E5%8C%85%E5%88%86%E6%9E%90.html
linux网卡ring buffer学习笔记

讲到门描述符：https://blog.51cto.com/u_15293891/3000865



      仔细揣摩了一段时间.

       系统调用过程,用户进程进入内核态，进程栈进入内核态栈, cpu进入内核态，cpu用户态各寄存器的值保存到内核态栈,执行内核态代码. 执行完从内核态返回到用户态，包括进程栈返回到用户态栈，cpu返回到用户态，cpu各寄存器的值用之前保存在内核态栈的值还原.  内核在执行系统调用时处于进程上下文中，current指针指向当前进程，即引发系统调用的进程。

   1 系统调用的过程中 可以发生进程切换(1 来自时钟中断，时间片用完,schedule()。2内核态代码执行过程中阻塞，主动schedule() )。 
   2 系统调用的过程中 可以发生中断，中断任意时刻可以发生,中断不属于任何一个进程上下文. 
    a 当前进程是用户态  cpu进入内核态，使用栈进入到中断栈,并且在中断栈保存用户态各寄存器的值,执行中断代码，中断代码执行过程中 不能被阻塞， 不能被切换。执行完中断代码后，从中断栈恢复用户态寄存器值，cpu进入用户态。(用户态进入到内核栈时，该栈时空的，中断代码可以直接使用进程的内核态栈)
    b 当前进程是内核态,使用栈进入到中断栈,并且在中断栈保存内核态各寄存器的值，执行中断代码,执行完中断代码后，从中断栈恢复内核态寄存器值.
    c 中断代码执行时，处在中断上下文.不属于任何一个进程上下文.

    d 每CPU变量中会有两个栈单独用于中断过程 分别用于软中断和硬中断 (2.6.x版本后)

  3 从内核态返回到用户态的过程中,有很多事情都是这个环节里面做的.

      a  .检查 need_resched，当前进程是否需要发起schedule() ,这个过程也会发生进程切换

      b  会检查所有进程是否有信号到达(深入理解linux内核 第三版 422页 ),  信号的处理是在这个过程来触发的。之前看了很多资料，都没发现一个可以进入信号处理的入口. 几乎所有的文章都是讲怎么发信号，怎么处理信号.就是没有一个说道有信号达到的进程是如何感知到，并且被调度.