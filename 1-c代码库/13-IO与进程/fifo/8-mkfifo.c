#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	int fd;
	if(mkfifo("myfifo", 0664) < 0){
		if(errno == EEXIST){
			fd = open("myfifo", O_RDWR);
			printf("fd = %d\n", fd);
		}
		else{
			perror("fail to mkfifo");
			return -1;
		}
	}
#if 0
	if((fd = open("myfifo", O_RDWR)) > 0){
		printf("fd = %d\n", fd);
	}
#endif
	return 0;
}
