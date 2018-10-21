#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	pid = fork();
	int fd;
	if(pid < 0){
		perror("fail to fork child process");
		return -1;
	}

	else if(pid == 0){
		//child
		setsid();
		chdir("/");
		for(fd = 0; fd < getdtablesize(); fd++){
			close(fd);
		}
		printf("hello world\n");
		while(1)
			;
	}
	else{
		//parent
		exit(0);
	}
	return 0;
}
