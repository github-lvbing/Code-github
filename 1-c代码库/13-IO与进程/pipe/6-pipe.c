#include <stdio.h>
#include <unistd.h>

#define N 1024
int main(int argc, char *argv[])
{
	int fd[2];
	char buf[N];
	char buf1[N] = {0};
	int i = 0;
	ssize_t n;
	for(i = 0; i < N; i++){
		buf[i] = 'a';
	}
	if(pipe(fd) < 0){
		perror("fail to pipe");
		return -1;
	}
	write(fd[1], buf, N);
	close(fd[1]);
	while(1){
		n = read(fd[0], buf1, sizeof(buf1));
		printf("buf1 = %s\n", buf1);
	}
	return 0;
}
