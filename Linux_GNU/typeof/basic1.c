#include<stdio.h>
#include <stdlib.h>
#include <string.h>
//使用方法1：宏定义内动态获取结构体成员的类型
#define min(x,y) ({ \
    typeof(x) _min1 = (x); \
    typeof(y) _min2 = (y); \
    (void) (&_min1 == &_min2);\
    _min1 < _min2 ? _min1 : _min2; })

#define A int

//使用方法2 ：typeof()定义一个用于接收函数返回值的变量
struct apple{
    int weight;
    int color;
};

struct apple* get_app_info()
{
    struct apple *a1;
    a1 = malloc(sizeof(struct apple));
    a1->weight = 2;
    a1->color = 1;
    A test = 1;
    print("%d",test)
    return a1;

}


int main(void){

    int a = 1;
    int b = 2;
    int c = min(a++,b);
    printf("%d\n",c);



   
    typeof(get_app_info()) r1;//定义一个变量r1，用于接收函数get_apple_info的返回值
    r1 = get_app_info();
    printf("apple weight:%d\n",r1->weight);
    printf("apple color :%d\n",r1->color);
    return 0;
}