#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>


#define BUFSIZE 512
#define SERV_PORT 8888



/*
关于select:
从用户空间拷贝fd_set到内核空间，每次调用select，都需要把fd集合从用户态拷贝到内核态，这个开销在fd很多时很大
每次调用select，都需要把fd集合从用户态拷贝到内核态，这个开销在fd很多时很大

select函数在每次调用之前，都要对参数进行重新设定，这样做比较麻烦，而且会降低性能。

进程被唤醒后，程序并不知道哪些socket收到数据，还需要遍历一次。
*/

int main(int argc,char *argv[])
{
    int i,j,n,nready;
    int maxfd = 0;
    int listenfd,connfd;
    char buf[BUFSIZ];
    char str[100];
    struct sockaddr_in clie_addr,serv_addr;
    socklen_t clie_addr_len;

    listenfd = socket(AF_INET,SOCK_STREAM,0);

    int opt = 1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);

    bind(listenfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));

    listen(listenfd,128);

    fd_set rset,allset;  //rset 读事件文件描述符集合，allset用来暂存
    maxfd = listenfd;

    FD_ZERO(&allset);  //allset清0
    FD_SET(listenfd,&allset);  //把Listen fd放到allset集合中，置1
    while(1)
    {
        rset = allset;   //每次循环都重新设置select 监控信号集----allset
        nready = select(maxfd+1,&rset,NULL,NULL,NULL);   //nready是有事件发生的文件描述符个数
        if(nready < 0)   
        {
            perror("select");
            return -1;
        }
        if(FD_ISSET(listenfd,&rset)){      //说明有新的客户端连接请求
            clie_addr_len = sizeof(clie_addr);
            connfd = accept(listenfd,(struct sockaddr *)&clie_addr,&clie_addr_len);
            printf("received from %s at port %d\n",inet_ntop(AF_INET,&clie_addr.sin_addr,str,sizeof(str)),ntohs(clie_addr.sin_port));
            FD_SET(connfd,&allset);     //将新创建出来的客户端文件描述符加入到监听集合

            if(maxfd < connfd)
               maxfd = connfd;
            if(0 == --nready)
            {
                continue;
            }
        }
        for(i = listenfd+1;i <= maxfd;i++){
            if(FD_ISSET(i,&rset)){
                if((n = read(i,buf,sizeof(buf))) == 0){  //当client关闭情况下：1、服务端关闭连接 2、解除select对该文件描述符的监控
                    close(i);                            //关闭套接字
                    FD_CLR(i,&allset);                  //解除select对该文件描述符的监控
                }else if(n > 0){
                    for(j = 0;j<n;j++)
                    buf[j] = toupper(buf[j]);
                    write(connfd,buf,n);
                }
            }
        }
    }
    close(listenfd);

    return 0;
}

