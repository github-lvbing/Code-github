#include <stdio.h>
#include <unistd.h>

#define N 1024
int main(int argc, char *argv[])
{
	int fd[2];
	char buf[N] = {0};
	int i = 0;
	ssize_t n;
	if(pipe(fd) < 0){
		perror("fail to pipe");
		return -1;
	}

	while(1){
		n = read(fd[0], buf, sizeof(buf));
		printf("n = %d\n", n);
	}
	return 0;
}
