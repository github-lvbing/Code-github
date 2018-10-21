//循环链表

#include <stdio.h>
#include <stdlib.h>

typedef int data_t;

typedef struct linknode{
	data_t data;
	struct linknode *next;
}linklist_t;


//创建空的循环链表
linklist_t * linklist_loop_create()
{
	linklist_t *head = (linklist_t*)malloc(sizeof(linklist_t));
	head->next = head;

	return head;
}

//头插法
int linklist_loop_insert(linklist_t *head, data_t value)
{
	linklist_t *node = (linklist_t *)malloc(sizeof(linklist_t));

	node->data = value;
	node->next = head->next;
	head->next = node;

	return 0;
}

//遍历
int linklist_loop_show(linklist_t *head)
{
	linklist_t *temp = head; 				//临时记录头节点的位置
	while(head->next != temp) 
	{
		printf("%d  ",head->next->data); 	//打印每一个节点的数据
		head = head->next; 					//指针向后移动
	}
	putchar(10);

	return 0;
}

//砍头
linklist_t * linklist_loop_cut_head(linklist_t *head)
{
	linklist_t *temp = head;
	while(head->next != temp)
	{
		head = head->next; 					//跳出循环时：head指向最后一个节点
	}

	head->next = temp->next;
	free(temp);
	temp = NULL;

	return  head->next;
}

int linklist_loop_show_nohead(linklist_t *head)
{
	linklist_t *temp = head;

	while(head->next != temp)
	{
		printf("%d  ",head->data);
		head = head->next;
	}
	printf("%d\n",head->data);
	return 0;
}



int main(int argc, const char *argv[])
{
	linklist_t *l = linklist_loop_create();
	linklist_loop_insert(l,10);
	linklist_loop_insert(l,20);
	linklist_loop_insert(l,30);
	linklist_loop_insert(l,40);
	linklist_loop_insert(l,50);
	linklist_loop_show(l);

	l = linklist_loop_cut_head(l); 		//删除的是空的头节点，看不到现象
	l = linklist_loop_cut_head(l);
	linklist_loop_show_nohead(l);

	
	return 0;
}
