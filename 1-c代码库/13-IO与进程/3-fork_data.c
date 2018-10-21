#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	int a = 3;
	pid = fork();

	if(pid < 0){
		perror("fail to fork child process");
		return -1;
	}
	else if(pid == 0){
		//child
		printf("In the child process a = %d &a = %p\n", a, &a);
		while(1)
			;
	}
	else{
		//parent
		printf("In the parent process a = %d &a = %p\n", a, &a);
		while(1)
			;
	}
	return 0;
}
