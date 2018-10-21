#include <stdio.h>
#include <stdlib.h>

#define N 13

typedef int data_t;
//节点的类型
typedef struct node{
	data_t data; 			//数据段
	struct node *next; 		//地址段
}**HASH_t, linknode_t; 		//linknode_t  <==>  struct node
							//HASH_t 	  <==>  struct node **

//创建N个空链表
HASH_t linknode_create()
{
	int i;
	HASH_t h = (HASH_t)malloc(N * sizeof(linknode_t *));

	for(i=0; i<N; i++)
	{
		h[i] = (linknode_t *)malloc(sizeof(linknode_t));
		h[i]->next = NULL;
	}

	return h; 							//返回哈希表的首地址
}


//插入数据
int linknode_insert(HASH_t h, data_t value)
{
	int key;
	key = value % N;

	linknode_t *temp = h[key]; 			//temp：临时记录链表的头节点地址
	linknode_t *node = (linknode_t *)malloc(sizeof(linknode_t)); 	//node：来一个数据开辟一个空间
	node->data = value;

	while(temp->next != NULL && temp->next->data < value) 			//实现：按顺序插入数据
	{
		temp = temp->next;
	}

	node->next = temp->next;
	temp->next = node;

	return 0;
}

int linknode_show(HASH_t h)
{
	int i;
	linknode_t *temp = NULL;
	for(i=0; i<N; i++)
	{
		temp = h[i]; 					//遍历：所有的哈希表的头节点
		printf("h[%2d] ：",i);

		while(temp->next != NULL) 		//循环：遍历每一个链表的所有节点
		{
			printf("%d ",temp->next->data);
			temp = temp->next;
		}
		putchar(10);
	}

	return 0;
}
int linknode_serach(HASH_t h, data_t value)
{
	int key = value % N;
	linknode_t *temp = h[key]; 		//只查找某一特定链表

	while(temp->next != NULL)
	{
		if(temp->next->data == value)
		{
			return 1;
		}
		temp = temp->next;
	}

	return 0;
}

int main(int argc, const char *argv[])
{
	int i, a[11] = {33,15,12,67,89,34,22,20,31,10,11};
	HASH_t h = linknode_create();

	for(i=0; i<11; i++)
	{
		linknode_insert(h,a[i]);
	}

	linknode_show(h);

	if(linknode_serach(h,32))
	{
		puts("serach");
	}
	else
	{
		puts("no serach");
	}

	return 0;
}


