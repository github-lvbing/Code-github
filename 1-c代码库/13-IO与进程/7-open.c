#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd1, fd2, fd3, fd4;

	if((fd1 = open("test.txt", O_RDONLY)) > 0){
		printf("fd1 = %d\n", fd1);
	}
	if((fd2 = open("test.txt", O_RDONLY)) > 0){
		printf("fd2 = %d\n", fd2);
	}
	close(fd1);
	if((fd3 = open("test.txt", O_RDONLY)) > 0){
		printf("fd3 = %d\n", fd3);
	}
	if((fd4 = open("test.txt", O_RDONLY)) > 0){
		printf("fd4 = %d\n", fd4);
	}

	return 0;
}
