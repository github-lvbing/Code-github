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
	int count = 0;
	for(i = 0; i < N; i++){
		buf[i] = 'a';
	}
	if(pipe(fd) < 0){
		perror("fail to pipe");
		return -1;
	}

	while(1){
		n = write(fd[1], buf, sizeof(buf));
		count ++;
		printf("count = %d\n", count);

		if(count == 64){
			read(fd[0], buf1, sizeof(buf1));
			read(fd[0], buf1, sizeof(buf1));
			read(fd[0], buf1, sizeof(buf1));
			read(fd[0], buf1, sizeof(buf1));
			read(fd[0], buf1, sizeof(buf1));
			read(fd[0], buf1, sizeof(buf1));
			read(fd[0], buf1, sizeof(buf1));
			read(fd[0], buf1, sizeof(buf1));
		}
	}
	return 0;
}
