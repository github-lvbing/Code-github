#ifndef _STACK_H_
#define _STACK_H_

#include <stdio.h>
#include <stdlib.h>

#define N 11

typedef int data_t; 	

typedef struct {
	data_t a[N]; 		
	int top; 				//栈顶
}sqstack_t; 				


//开辟结构体空间(空的栈)
sqstack_t * sq_stack_create();
int sq_stack_is_full(sqstack_t *stack, int n);
//空：return 1     非空：return 0
int sq_stack_is_empty(sqstack_t *stack);
//满栈：return -1    非满：return 0
int sq_stack_push(sqstack_t *stack , data_t value, int n);
data_t sq_stack_pop(sqstack_t *stack);

#endif



