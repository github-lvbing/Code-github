#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	pid_t ret;
	pid = fork();

	if(pid < 0){
		perror("fail to fork child process");
		return -1;
	}

	else if(pid == 0){
		//child
		sleep(5);
		printf("In the child process, child process id = %d parent process id = %d\n", getpid(), getppid());
	}
	else{
		//parent
#if 0
		ret = wait(NULL);
#endif
		ret = waitpid(-1, NULL, 0);
		printf("ret = %d\n", ret);
		printf("In the parent process, child process id = %d parent process id = %d\n", pid, getpid());
		while(1)
			;
	}
	return 0;
}
