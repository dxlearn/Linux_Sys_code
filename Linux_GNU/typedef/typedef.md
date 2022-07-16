Demo1:
#include <signal.h>
typedef void (*sighandler_t)(int);  //返回值为空，回调函数名为sighandler_t 参数 int  通过typedef 定义了一个新的变量
sighandler_t signal(int signum, sighandler_t handler);

如上所示：signal函数中，第二个参数 sighandler_t handler就是声名sighandler_t类型


Demo2:
kernel:<include/linux/types.h>  
```c
typedef strcut {
    int counter;
}atomic_t;
```
