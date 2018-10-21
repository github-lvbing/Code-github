#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define N 128
char buf[N] = {0};
pthread_cond_t cond;
pthread_mutex_t lock;
void *pthread_handler1(void *arg){
	while(1){
		fgets(buf, N, stdin);
		buf[strlen(buf) - 1] = '\0';
		pthread_cond_signal(&cond);//通过条件变量标识符，指定唤醒的线程
	}
	pthread_exit(0);
}
void *pthread_handler2(void *arg){
	while(1){
//		sleep(10);
		pthread_mutex_lock(&lock);//执行上锁，将睡眠线程锁住
		pthread_cond_wait(&cond, &lock);//执行线程睡眠操作，在睡眠之前需使用互斥锁将其锁住
		printf("buf = %s\n", buf);
		pthread_mutex_unlock(&lock);
	}
	pthread_exit(0);
}
int main(int argc, char *argv[])
{
	pthread_t thread1, thread2;
	
	pthread_mutex_init(&lock, NULL);//初始化互斥锁
	pthread_cond_init(&cond, NULL);//初始化条件变量

	if(pthread_create(&thread1, NULL, pthread_handler1, NULL) != 0){
		perror("fail to pthread_create");
		return -1;
	}
	if(pthread_create(&thread2, NULL, pthread_handler2, NULL) != 0){
		perror("fail to pthread_create");
		return -1;
	}
	
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&cond);
	return 0;
}
