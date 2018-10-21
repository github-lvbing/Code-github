#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define N 128
int main(int argc, char *argv[])
{
	int fdr, fdw;
	char buf[N] = {0};
	ssize_t n;

	if(argc != 2){
		fprintf(stderr, "Usage: %s + filename\n", argv[0]);
		return -1;
	}
	if((fdr = open("test.txt", O_RDONLY)) < 0){
		perror("fail to open");
		return -1;
	}
	if((fdw = open(argv[1], O_WRONLY|O_CREAT|O_TRUNC, 0664)) < 0){
		perror("fail to open");
		return -1;
	}

	while((n = read(fdr, buf, N)) > 0){
		write(fdw, buf, n);
	}
	return 0;
}
