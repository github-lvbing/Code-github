#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define N 128
int main(int argc, char *argv[])
{
	int fd;
	char buf[N] = {0};
	size_t n;
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
	else{
		fd = open("myfifo", O_RDWR);
		printf("fd = %d\n", fd);
	}
	printf("start write\n");

	while(1){
		fgets(buf, N, stdin);
		buf[strlen(buf) - 1] = '\0';
		n = strlen(buf);

		write(fd, buf, n);

		if(strncmp(buf, "quit", 4) == 0){
			break;
		}
	}
	return 0;
}
