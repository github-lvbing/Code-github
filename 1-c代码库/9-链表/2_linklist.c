//线性表的链式存储（链表）

#include <stdio.h>
#include <stdlib.h>

typedef int data_t;

//数据类型
typedef struct linknode
{
	data_t data;
	struct linknode *next; 				//下一个节点的类型：struct linknode
}linklist_t;

//创建第一个节点（头节点）
//特点：数据段为空，地址段为NULL 		//nulll链表结束条件
linklist_t * linklist_create()
{
	linklist_t *head = (linklist_t *)malloc(sizeof(linklist_t)); 	//开辟头节点的空间
	head->next = NULL;

	return head; 													//返回动态区的首地址
}

//数据的插入,头插法
int linklist_insert(linklist_t *head, data_t value)
{
	linklist_t *node = (linklist_t *)malloc(sizeof(linklist_t));   //node：新节点的地址

	node->data = value; 				//数据段：存储数值
	node->next = head->next; 			//地址段：记录头节点的下一个数据
	head->next = node; 					//头节点的地址段：指向新的节点

	return 0;
}

//尾插法
int linklist_insert_tail(linklist_t *head, data_t value)
{
	linklist_t *node = (linklist_t *)malloc(sizeof(linklist_t));
	node->data = value;
	node->next = NULL;

	while(head->next != NULL)
	{
		head = head->next;
	}

	head->next = node;

	return 0;
}

//判断是否为空
//空：return 1    非空：return 0
int linklist_is_empty(linklist_t *head)
{
	return head->next == NULL ? 1 : 0;
}

//头删除法
int linklist_delete_head(linklist_t *head)
{
	if(linklist_is_empty(head))
	{
		puts("empty");
		return -1;
	}

	linklist_t *temp = head->next; 		//临时记录第一个节点
	head->next = head->next->next; 		//头节点的地址段：指向第二个节点地址

	free(temp);
	temp = NULL;

	return 0;
}

//数据修改
int linklist_change(linklist_t *head, data_t old, data_t new)
{
	while(head->next != NULL)
	{
		head = head->next;
		if(head->data == old)
		{
			head->data = new;
			return 0;
		}
	}

	puts("no search");
	return -1;
}

//根据数据，查找位置
int linklist_data_search_pos(linklist_t *head, data_t value)
{
	int i;
	for(i=0; head->next != NULL; i++)
	{
		head = head->next;
		if(head->data == value)
		{
			return i;
		}
	}

	puts("no search");
	return -1;
}

//按照位置插入数据
int linklist_insert_pos(linklist_t *head, int pos, data_t value)
{
	int i;
	linklist_t *node = (linklist_t *)malloc(sizeof(linklist_t));
	node->data = value;

	for(i=0; i<pos && head->next != NULL ; i++)
	{
		head = head->next; 			//找到pos前一个位置的节点
	}

	node->next = head->next;
	head->next = node;

	return 0;
}

//按顺序插入数据
int linklist_insert_sort(linklist_t *head, data_t value)
{
	linklist_t *node = (linklist_t *)malloc(sizeof(linklist_t));
	node->data = value;

	while(head->next != NULL  &&  head->next->data < value)
	{
		head = head->next;
	}

	node->next = head->next;
	head->next = node;

	return 0;
}

//数据反转
int linklist_revers(linklist_t *head)
{
	linklist_t *temp = NULL;
	linklist_t *old = head->next;
	head->next = NULL;

	while(old  != NULL)
	{
		temp = old; 				//头删法
		old = old->next;

		temp->next = head->next;	//头插法
		head->next = temp;
	}

	return 0;
}

//遍历链表
int linklist_show(linklist_t *head)
{
	while(head->next != NULL)
	{
		head = head->next; 				//指针指向下一个节点
		printf("%d  ",head->data); 		//访问节点里的数据
	}
	putchar(10);

	return 0;
}


int main(int argc, const char *argv[])
{
	linklist_t *h = linklist_create(); 	//h：记录头节点的地址
	linklist_insert(h,10);
	linklist_insert(h,20);
	linklist_insert(h,30);
	printf("头插入法：");
	linklist_show(h);

	linklist_insert_tail(h,14);
	printf("尾插入法：");
	linklist_show(h);

	linklist_delete_head(h);
	printf("头删除法：");
	linklist_show(h);

	linklist_change(h,10,13);
	printf("数据修改：");
	linklist_show(h);

	printf("查找位置：%d\n",linklist_data_search_pos(h,13));

	linklist_insert_pos(h,5,11);
	printf("按位置插：");
	linklist_show(h);

	puts("****************");

	linklist_t *w = linklist_create();
	linklist_insert_sort(w,3);
	linklist_insert_sort(w,7);
	linklist_insert_sort(w,1);
	linklist_insert_sort(w,2);
	linklist_insert_sort(w,5);
	printf("按顺序插：");
	linklist_show(w);

	linklist_revers(w);
	printf("数据反转：");
	linklist_show(w);





	
	return 0;
}
