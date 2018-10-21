#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define N 128
int main(int argc, char *argv[])
{
	int fd;
	char buf[N] = {0};
	ssize_t n;
	if((fd = open("test.txt", O_RDONLY)) < 0){
		perror("fail to open");
		return -1;
	}
	
	if((n = read(fd, buf, N)) > 0){
		printf("n = %d\n", n);
		printf("buf = %s\n", buf);
	}

	close(fd);
	return 0;
}

