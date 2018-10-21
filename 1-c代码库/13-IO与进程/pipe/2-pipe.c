#include <stdio.h>
#include <unistd.h>

#define N 1024
int main(int argc, char *argv[])
{
	int fd[2];
	char buf[N];
	int i = 0;
	ssize_t n;
	int count = 0;
	for(i = 0; i < N; i++){
		buf[i] = 'a';
	}
	if(pipe(fd) < 0){
		perror("fail to pipe");
		return -1;
	}

	while(1){
		n = write(fd[1], buf, 1023);
		count += n;
		printf("count = %d\n", count);
	}
	return 0;
}
