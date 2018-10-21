#include <stdio.h>
#include <pthread.h>

void *pthread_handler(void *arg){
//	int i = 5;
	int i = *((int *)arg);
	while(i > 0){
		sleep(1);
		puts("hello world");
		i--;
	}
	pthread_exit("exit");
}
int main(int argc, char *argv[])
{
	pthread_t thread;
	int a = 3;
	void *retval;
	if(pthread_create(&thread, NULL, pthread_handler, (void *)&a) != 0){
		perror("fail to pthread_create");
		return -1;
	}

	pthread_join(thread, &retval);
	printf("%s\n", (char *)retval);

	printf("pthread_join after\n");
	return 0;
}
