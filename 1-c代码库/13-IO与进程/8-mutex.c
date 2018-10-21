#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock;
void *pthread_handler(void *arg){
	while(1){
		pthread_mutex_lock(&lock);
		puts("A");

		pthread_mutex_lock(&lock);

		puts("B");
		pthread_mutex_unlock(&lock);
		pthread_mutex_unlock(&lock);
	}
	pthread_exit("exit");
}
int main(int argc, char *argv[])
{
	pthread_t thread;
	int a = 3;
	void *retval;

	pthread_mutex_init(&lock, NULL);
	if(pthread_create(&thread, NULL, pthread_handler, (void *)&a) != 0){
		perror("fail to pthread_create");
		return -1;
	}

	pthread_join(thread, &retval);
	printf("%s\n", (char *)retval);

	printf("pthread_join after\n");
	return 0;
}
