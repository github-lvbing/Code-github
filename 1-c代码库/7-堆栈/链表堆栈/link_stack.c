//链表实现栈

#include <stdio.h>
#include <stdlib.h>

typedef int data_t;

//数据类型
typedef struct linknode{
	data_t data;
	struct linknode *next; 		
}linklist_t;

//创建空栈
linklist_t * link_stack_create()
{
	linklist_t *stack = (linklist_t *)malloc(sizeof(linklist_t));
	stack->next = NULL;

	return stack;		
}

//判断是否为空
int link_stack_is_empty(linklist_t *stack)
{
	return stack->next == NULL ? 1 : 0;
}

//入栈（头插法）
int link_stack_push(linklist_t *stack, data_t value)
{
	linklist_t *node = (linklist_t *)malloc(sizeof(linklist_t));

	node->data = value;
	node->next = stack->next;
	stack->next = node;

	return 0;
}


//出栈（头删法）
int link_stack_pop(linklist_t *stack)
{
	if(link_stack_is_empty(stack))
	{
		puts("empty");
		return -1;
	}

	linklist_t *temp = stack->next;
	data_t value = temp->data;

	stack->next = temp->next;
	free(temp);
	temp = NULL;
	
	return value;
}



int main(int argc, const char *argv[])
{
	int i;
	linklist_t *stack = link_stack_create();

	for(i=0; i<10; i++)
	{
		link_stack_push(stack, i+10);
	}

	for(i=0; i<10; i++)
	{
		printf("%d ",link_stack_pop(stack));
	}
	putchar(10);
	
	return 0;
}
