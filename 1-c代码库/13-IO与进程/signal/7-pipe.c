#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>

#define N 128
int main(int argc, char *argv[])
{
	int fd[2];
	char buf[N] = {0};
	char buf1[N] = {0};
	size_t n;
	if(pipe(fd) < 0){
		perror("fail to pipe");
		return -1;
	}

	pid_t pid;

	pid = fork();

	if(pid < 0){
		perror("fail to fork");
		return -1;
	}
	else if(pid == 0){
		while(1){
			fgets(buf, N, stdin);
			buf[strlen(buf) - 1] = '\0';
			n = strlen(buf);

			write(fd[1], buf, n);

			if(strncmp(buf, "quit", 4) == 0){
				kill(getppid(), SIGKILL);
				raise(SIGKILL);
			}
		}
	}
	else{
		while(1){
			read(fd[0], buf1, N);
			
			printf("buf1 = %s\n", buf1);
		}
	}
	return 0;
}
