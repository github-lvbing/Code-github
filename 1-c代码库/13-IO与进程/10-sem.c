#include <stdio.h>
#include <semaphore.h>

int main(int argc, char *argv[])
{
	sem_t sem;
	int val = 0;
	sem_init(&sem, 0, 1);

	sem_wait(&sem);
	sem_getvalue(&sem, &val);
	printf("val = %d\n", val);

	sem_post(&sem);
	sem_getvalue(&sem, &val);
	printf("val = %d\n", val);
	return 0;
}
