#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
 
#define FIFO_PATH	"/tmp/myfifo"
 
int main(){
	int pipe_fd = -1;
	unsigned char buff[]="hello world";
 
	if (access(FIFO_PATH, F_OK) == -1){
		printf("FIFO file not exist, creat it\n");
		if(mkfifo(FIFO_PATH, 0777) < 0){
			perror("mkfifo");
			_exit(-1);
		}
	}
	
 
	pipe_fd = open(FIFO_PATH, O_WRONLY);
	if(pipe_fd < 0){
		perror("open");
		printf("open file failed! %d\n", pipe_fd);
		return -1;
	}
	
	while(1){
		if(write(pipe_fd, buff, sizeof(buff)) < 0){
			printf("write error!\n");
			close(pipe_fd);
			return -1;
		}else{
			printf("send to fifo success!\n");
		}
		sleep(1);
	}
	close(pipe_fd);
 
	return 0;
}