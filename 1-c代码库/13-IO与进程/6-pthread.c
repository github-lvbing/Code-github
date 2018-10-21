#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock;
int count = 0;
int value1 = 0;
int value2 = 0;

void *pthread_handler1(void *arg){
	while(1){
		//执行上锁操作
		pthread_mutex_lock(&lock);
		count++;
		value1 = count;
		value2 = count;
		//执行解锁操作
		pthread_mutex_unlock(&lock);
	}
	pthread_exit(0);
}
void *pthread_handler2(void *arg){
	while(1){
//		pthread_mutex_lock(&lock);
		if(value1 != value2){
			sleep(1);
			printf("value1 = %d value2 = %d\n", value1, value2);
		}
//		pthread_mutex_unlock(&lock);
	}
	pthread_exit(0);
}
int main(int argc, char *argv[])
{
	pthread_t thread1, thread2;
//互斥锁初始化
	pthread_mutex_init(&lock, NULL);

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
	pthread_mutex_destroy(&lock);
	return 0;
}
