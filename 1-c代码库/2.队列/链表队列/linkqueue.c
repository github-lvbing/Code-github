#include "./3_linkqueue.h"

//创建一个空队
linkqueue_t *linkqueue_create()
{
	linklist_t *head = (linklist_t *)malloc(sizeof(linklist_t)); 		//创建队头
	head->next = NULL;

	linkqueue_t * queue = (linkqueue_t*)malloc(sizeof(linkqueue_t)); 	//创建一个队列
	queue->front = head; 		//指针最开始，都指向头节点
	queue->rear = head;

	return queue;
}

//入队(尾插法)
int linkqueue_input(linkqueue_t *queue, data_t value)
{
	linklist_t *node = (linklist_t *)malloc(sizeof(linklist_t)); 	//开辟新节点的空间
	node->data = value;
	node->next = NULL;

	queue->rear->next = node; 			//完成链表的链接
	queue->rear = node; 				//使rear指向最后一个节点的地址

	return 0;
}

linkqueue_is_empty(linkqueue_t *queue)
{
	return queue->front->next == NULL ? 1 : 0;
}

//出队（头删法）
int linkqueue_output(linkqueue_t *queue)
{
	if(linkqueue_is_empty(queue))
	{
		return -1;
	}

	linklist_t *temp = queue->front->next; 	//temp：临时记录第一个节点的地址
	data_t value = temp->data; 				//value：记录要删除的数据
	queue->front->next = temp->next; 		//删除第一个节点


	if(queue->front->next == NULL) 			//当删除掉最后一个节点时，rear需重新赋值，指向头节点
	{
		queue->rear = queue->front;
	}

	free(temp); 							//释放
	temp = NULL;

	return value;
}

