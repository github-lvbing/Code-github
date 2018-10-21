#include "1_sq_stack.h"

//开辟结构体空间(空的栈)
sqstack_t * sq_stack_create()
{
	sqstack_t* stack = (sqstack_t *)malloc(sizeof(sqstack_t));
	stack->top = -1;

	return stack;
}

int sq_stack_is_full(sqstack_t *stack, int n)
{
	return stack->top == n-1 ? 1 : 0;
}

int sq_stack_is_empty(sqstack_t *stack)
{
	return stack->top == -1 ? 1 : 0;
}

//入栈(插入)
int sq_stack_push(sqstack_t *stack , data_t value, int n)
{
	if(sq_stack_is_full(stack, n))
	{
		return -1;
	}
	stack->top++;
	stack->a[stack->top] = value;

	return 0;
}

//出栈（删除）
data_t sq_stack_pop(sqstack_t *stack)
{
	data_t value = 0;
	if(sq_stack_is_empty(stack))
	{
		return -1;
	}
	value = stack->a[stack->top];
	stack->top--;
	
	return value;
}



