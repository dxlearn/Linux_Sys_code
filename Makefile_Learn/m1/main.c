#include<stdio.h>

int add(int ,int);
int div(int ,int);
int main(void){
    int a = 3,b=3;
    printf("test Makefile\n");
    printf("a=%d+b=%d = %d\n",a,b,add(a,b));
}