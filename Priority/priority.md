unix提供了getpriority系统调用用于返回进程，进程组，用户进程的进程优先级。

long getpriority(int which,int who)

getpriority系统调用实际返回的是进程的nice值，只是对nice值做了转化，返回的是20-nice值。进程的nice值越小，进程的优先级越高。

参数which是PRIO_PROCESS，PRIO_PGRP，PRIO_USER中的一个。
```
#define PRIO_PROCESS    0   //进程
#define PRIO_PGRP   1   //进程组
#define PRIO_USER   2   //用户进程
```
当参数which为PRIO_PROCESS时，参数who为0，则返回当前进程的进程优先级；参数who不为0，则返回进程号为who的进程的优先级。

当参数which为PRIO_PGRP时，参数who为0，则返回当前进程组的优先级；参数who不为0，则返回进程组号为who的进程组的优先级。进程组的优先级为进程组中优先级最高的进程的优先级。

当参数which为PRIO_USER时，参数who为0，则返回当前用户进程的优先级；参数who不为0，则返回用户ID为who的进程的优先级。用户进程的优先级为进程中优先级最高的进程的优先级。

当系统调用执行成功，则返回优先级。若执行失败，则返回-1，错误号放入errno中。
