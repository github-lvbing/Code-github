#include <stdio.h>
#include <stdlib.h>

#define N 8

typedef int data_t;
typedef struct{
	data_t a[N];
	int front; 				//front ：队头的下标
	int rear; 				//rear  ：队尾下一个位置的下标
}sqqueue_t;


//创建一个空的队列
sqqueue_t * sqqueue_create()
{
	sqqueue_t *queue = (sqqueue_t *)malloc(sizeof(sqqueue_t));
	queue->front = 0;
	queue->rear = 0;

	return queue;
}

//为满
sqqueue_is_full(sqqueue_t *queue)
{
	return (queue->rear + 1) % N == queue->front ? 1 : 0;
}

//入队
int sqqueue_input(sqqueue_t *queue, data_t value)
{
	if(sqqueue_is_full(queue))
	{
		puts("full");
		return -1; 				//异常退出
	}

	queue->a[queue->rear] = value;
	queue->rear  = (queue->rear + 1) % N;

	return 0;
}

//是否为空
int sqqueue_is_empty(sqqueue_t *queue)
{
	return queue->rear == queue->front ? 1 : 0;
}

//出队
int sqqueue_output(sqqueue_t * queue)
{
	data_t value;
	if(sqqueue_is_empty(queue))
	{
		puts("empty");
		return -1;
	}

	value = queue->a[queue->front];
	queue->front = (queue->front + 1) % N;

	return value;
}

int main(int argc, const char *argv[])
{
	sqqueue_t *q = sqqueue_create();
	sqqueue_input(q,10);
	sqqueue_input(q,20);
	sqqueue_input(q,30);
	sqqueue_input(q,40);
	printf("%d  ",sqqueue_output(q));
	printf("%d  ",sqqueue_output(q));
	printf("%d  ",sqqueue_output(q));
	printf("%d  ",sqqueue_output(q));
	putchar(10);

	
	return 0;
}


