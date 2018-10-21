#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd;
	while(1){
		if((fd = open("test.txt", O_RDONLY)) < 0){
			perror("fail to open");
			return -1;
		}
		sleep(1);
		printf("fd = %d\n", fd);
	}
	return 0;
}
