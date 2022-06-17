
## 参考
https://blog.csdn.net/scdxmoe/article/details/37901481?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-1-37901481-blog-117361412.pc_relevant_default&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-1-37901481-blog-117361412.pc_relevant_default&utm_relevant_index=1


https://blog.csdn.net/wangquan1992/article/details/117361412


https://zhuanlan.zhihu.com/p/157741043

网络收包流程：

网卡接收到网络帧通过DMA把网络帧放到内核的接受ring buffer(内存)中, CPU不参与。网卡硬中断CPU。CPU调用网卡驱动中断处理程序从ring buffer里取出来构造成sk_buff放入backlog队列(netdev_max_backlog)里。标志网络软中断。内核软中断线程遍历到当前网络软中断，从网络协议栈从下向上对数据包处理，然后把它放入到rqueue中。让应用层通过recvfrom调用。

接受缓冲区大小：限制创建sk_buff的数量。包括sk_buff结构体大小 + 网络首部 + 数据。

发送缓冲区大小：限制创建sk_buff的数量。

网络发包流程:

应用程序调用send把数据包从网络协议栈从上到下放入到网卡对应的在内存中的发送ring buffer中。CPU硬中断网卡。网卡通过DMA来发送数据包。

UDP socket 只有一个接受缓冲区，没有发送缓冲区

TCP保证包到达的先后顺序，但UDP不保证






一 收包流程
内核分配一个主内存地址段（DMA缓冲区 RingBuffer)，网卡设备可以在DMA缓冲区中读写数据。
网卡收到一个数据帧后， 检查MAC地址是否是自己，如果没开启混杂模式，且不是自己就丢了；
如果目的MAC是自己，则将数据帧以DMA方式放入到RingBuffer种，不用cpu参与。
当拷贝完成，网卡会触发网卡硬中断，cpu必须立刻响应硬中断， 根据中断类型在中断注册表中 查找对应的中断处理程序（网卡驱动在初始化的时候注册），调用注册的网卡驱动程序收包。
驱动程序收包会为网络帧分配内核数据结构sk_buff,并将它拷贝到sk_buff缓冲区。
驱动程序禁止网卡中断，因为已经在处理了，然后网卡驱动程序触发软中断，硬中断返回。
内核中的ksoftirqd进程专门处理软中断，收到软中断时候，会调用不同软中断处理，这里会调用
net_recv_action 收包，开始处理包，将sk_buff中的包格式转成上层协议栈能识别的包。
协议栈有一系列钩子函数，驱动程序调用现应的协议栈进行包的处理。
协议栈在链路层检查报文合法性，找出上层协议类型，去掉帧和帧尾，交给网络层。
网络层取出ip，判断网络包是转发，还是本机处理，如果本机处理则取上一层协议类型（TCP、UDP等）
去掉IP头部，交给传输层处理。
传输层取出TCP头和UDP头之后，根据四元组标识，找到对应socket，把数据拷贝到socket对应的
缓冲区中，唤醒相关进程收包。
至此应用程序通过socket接口，收到数据了。
数据经历： RingBuffer队列---> 拷贝到内核的缓冲区-->应用的内存

二 发包流程
应用程序调用socket接口， 会把数据放入到socket的缓冲区中。
网络协议栈从socket缓冲区中取数据包，按照tcp、udp协议为其增加头部，
再增加ip头，并按照数据包总大小和mtu比较，进行ip层的分片，再根据ip层路由
确定下一跳地址。
分片后，网络包送到网络接口层，进行物理地址寻址，寻找下一跳的mac地址。
然后增加帧头和帧尾，放入到发包队列中。
软中断通知驱动程序，发包队列中有数据要发送。
数据经历： 从应用内存---> 拷贝到内核的缓冲区-->RingBuffer队列



Ring-Buffer:https://xie.infoq.cn/article/06449d84c5f6b005775215024


初始化 Ring Buffer
当前绝大多数的 NIC 会直接将收到的数据包 DMA 到 RAM，然后操作系统网络子系统可以从 RAM 中取出数据，这段空间即是 Ring Buffer。因此，驱动必须向 OS 申请一段 Memory Region，并将这段地址告知 Hardware，之后网卡就会自动将数据包 DMA 到这段 Ring Buffer 中。

当网络数据包速率较高时，一个 CPU 不能够处理所有的数据包，具体的，这段 Ring Buffer 是固定大小的空间，数据包将会被丢掉。为了解决这个问题，设计了 Receive Side Scaling (RSS) 或者说多队列网卡技术。一些网卡设备可以将接收到的数据包分发到多个 Ring Buffer 中，每个 Ring Buffer 就是一个独立的 Queue。这样允许 OS 在硬件层面就使用多个 CPU 来并行处理收到的数据包。


数据包发送与接收，包含中断、NAPI、DMA等：https://houmin.cc/posts/d87f0ea6/