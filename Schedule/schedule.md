# 相关参考文章
https://juejin.cn/post/7043252167687274533  
https://juejin.cn/post/7043252167687274533  
https://cloud.tencent.com/developer/article/1857521
https://zhuanlan.zhihu.com/p/336681382
讲的清晰：https://zhuanlan.zhihu.com/p/460460503
关于两个结构体:https://zhuanlan.zhihu.com/p/373958196
1.pt_regs和cpu_context都是处理器架构相关的结构。

2.pt_regs是发生异常时（当然包括中断）保存的处理器现场，用于异常处理完后来恢复现场，就好像没有发生异常一样，它保存在进程内核栈中。

3.cpu_context是发生进程切换时，保存当前进程的上下文，保存在当前进程的进程描述符中。

4.pt_regs表征发生异常时处理器现场，cpu_context发生调度时当前进程的处理器现场。


关于current:https://zhuanlan.zhihu.com/p/296750228
https://zhuanlan.zhihu.com/p/363791563

https://www.zhihu.com/search?type=content&q=%E8%BF%9B%E7%A8%8B%E5%86%85%E6%A0%B8%E6%A0%88%E5%88%87%E6%8D%A2%20%20%E4%BC%98%E9%9B%85

各种上下文：http://m.bubuko.com/infodetail-3594779.html