#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock1;
pthread_mutex_t lock2;

void *pthread_handler1(void *arg){
	while(1){
		pthread_mutex_lock(&lock1);
		puts("thread1 A");
		pthread_mutex_lock(&lock2);
		puts("thread1 B");
		pthread_mutex_unlock(&lock2);
		pthread_mutex_unlock(&lock1);
	}
	pthread_exit(0);
}
void *pthread_handler2(void *arg){
	while(1){
		pthread_mutex_lock(&lock2);
		puts("thread2 A");
		pthread_mutex_lock(&lock1);
		puts("thread2 B");
		pthread_mutex_unlock(&lock1);
		pthread_mutex_unlock(&lock2);
	}
	pthread_exit(0);
}
int main(int argc, char *argv[])
{
	pthread_t thread1, thread2;
//互斥锁初始化
	pthread_mutex_init(&lock1, NULL);
	pthread_mutex_init(&lock2, NULL);
	if((pthread_create(&thread1, NULL, pthread_handler1, NULL)) != 0){
		perror("fail to pthread_create");
		return -1;
	}
	if((pthread_create(&thread2, NULL, pthread_handler2, NULL)) != 0){
		perror("fail to pthread_create");
		return -1;
	}
	
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_mutex_destroy(&lock1);
	pthread_mutex_destroy(&lock2);
	return 0;
}
