#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

//https://www.pengrl.com/p/20032/
int main()
{
    void *curr_brk,*tmp_brk = NULL;
    printf("test brk example:%d\n",getpid());

    tmp_brk = curr_brk = sbrk(0);

    printf("Program Break Location1:%p\n",curr_brk);
    getchar();

    brk(curr_brk+4096);

    curr_brk = sbrk(0);
    printf("Pogram break Location2:%p\n",curr_brk);
    getchar();
    //使用brk减少program break位置
    brk(tmp_brk);

    curr_brk = sbrk(0);
    printf("Program Break Location3:%p\n",curr_brk);
    getchar();

    return 0;
}