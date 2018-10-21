#include <stdio.h>
#include <pthread.h>

void *pthread_handler(void *arg){
//	int i = 5;
	int i = *((int *)arg);
	while(i > 0){
		sleep(1);
		puts("thread");
		i--;
	}
	pthread_exit(0);
}
void *pthread_handler1(void *arg){
	int i = 5;
	while(i > 0){
		sleep(1);
		puts("thread1");
		i--;
	}
	pthread_exit(0);
}
int main(int argc, char *argv[])
{
	pthread_t thread, thread1;
	int a = 3;
	void *retval;
	if(pthread_create(&thread, NULL, pthread_handler, (void *)&a) != 0){
		perror("fail to pthread_create");
		return -1;
	}

	if(pthread_create(&thread1, NULL, pthread_handler1, NULL) != 0){
		perror("fail to pthread_create");
		return -1;
	}

	pthread_join(thread, NULL);
	pthread_join(thread1, NULL);
	return 0;
}
