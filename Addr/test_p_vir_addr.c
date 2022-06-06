#include<stdio.h>    
#include<unistd.h>    
#include<stdlib.h>    

int gval = 10;    

int main()    
{    
    pid_t pid = fork();  
    if(pid == 0)  
    {    
        gval = 100;    
        printf("child --gval:%d--%p\n",gval,&gval);                                                                                                                                          
    }               
    else if(pid > 0)
    {    
        sleep(3);  
        printf("parent --gval:%d--%p\n",gval,&gval);  
    }               
    return 0;    
}                                                    
