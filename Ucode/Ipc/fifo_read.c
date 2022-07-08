#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
 
#define FIFO_PATH	"/tmp/myfifo"
 
int main(void){
	int fd;
	unsigned char buff[50];
 
	if (access(FIFO_PATH, F_OK) == -1){
		printf("FIFO_PATH file not exist, creat it\n");
		if(mkfifo(FIFO_PATH, 0777) < 0){
			perror("mkfifo");
			_exit(-1);
		}
	}
	
	if((fd = open(FIFO_PATH, O_RDONLY|O_NONBLOCK, 0)) < 0){
		perror("open");
		return -1;
	}
	
	while(1){
		memset(buff, 0x00, sizeof(buff));
		if(read(fd, buff, sizeof(buff)) < 0){
			printf("read from fifo failed!\n");
			continue;
		}
		printf("receive:%s\n",  buff);
		sleep(1);
	}
	close(fd);
	return 0;
}