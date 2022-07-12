 ```c
 #include <pthread.h>

       int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);  
```
其中第三个参数:void *(*start_routine) (void *)是一个返回类型为void *，参数为void *的函数

```c
SYNOPSIS
       #include <pthread.h>

       int pthread_join(pthread_t thread, void **retval);
```  
第2个参数是二级指针，是传入传出参数

