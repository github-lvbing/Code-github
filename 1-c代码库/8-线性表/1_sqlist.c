//线性表的顺序存储

#include <stdio.h>
#include <stdlib.h>

#define N 8

typedef int data_t; 		//方便修改

typedef struct {
	data_t a[N]; 			//开辟一个连续的存储空间
	int last; 				//最后一个数据的下标
}sqlist_t; 					//类型名


//开辟结构体空间(空的顺序表)
sqlist_t * sqlist_create()
{
	sqlist_t *p = (sqlist_t *)malloc(sizeof(sqlist_t)); 		//在动态区开辟一个结构体类型的区域
	p->last = -1;

	return p;
}

//判断顺序表是否为满
//满：return 1     不满：return 0
int sqlist_is_full(sqlist_t *p)
{
	return p->last == N-1 ? 1 : 0;
}

//判断顺序表是否为空
//空：return 1     不空：return 0
int sqlist_is_empty(sqlist_t *p)
{
	return p->last == -1 ? 1 : 0;
}

//向顺序表插入数据
int sqlist_insert(sqlist_t *p , data_t value)
{
	if(sqlist_is_full(p) == 1)
	{
		puts("full");
		return -1;
	}

	p->last++;
	p->a[p->last] = value;

	return 0;
}

//删除一个数据
int sqlist_delete(sqlist_t *p)
{
	if(sqlist_is_empty(p))
	{
		puts("empty");
		return -1;
	}

	p->last--;
	return 0;
}

//修改数据
int sqlist_change(sqlist_t *p, data_t old, data_t new)
{
	int i,sign = 0;
	for(i=0; i<=p->last;  i++)
	{
		if(p->a[i] == old)
		{
			p->a[i] = new;
			sign = 1;
		}
	}
	if(sign == 0)
	{
		puts("no search");
	}

	return 0;
}

//根据数据查找位置
int sqlist_data_search_pos(sqlist_t *p, data_t value)
{
	int i = -1;
	for(i=0; i<=p->last; i++)
	{
		if(p->a[i] == value)
		{
			return i;
		}
	}
	return -1;
}

//根据下标查找数据
data_t sqlist_pos_search_data(sqlist_t *p, int pos)
{
	if(pos < 0 || pos > p->last)
	{
		puts("pos error");
		return -1;
	}

	return p->a[pos];
}

//按照位置插入数据
int sqlist_insert_pos(sqlist_t *p, int pos, data_t value)
{
	int i;
	if(sqlist_is_full(p))
	{
		puts("full");
		return -1;
	}

	if(pos < 0 || pos > p->last+1 )
	{
		puts("pos error");
		return -1;
	}

	for(i=p->last; i>=pos; i--)
	{
		p->a[i+1] = p->a[i];
	}
	p->a[pos] = value;
	p->last++;

	return 0;
}

//按照位置删除数据
int sqlist_delete_pos(sqlist_t *p, int pos)
{
	int i;
	if(pos < 0 || pos > p->last)
	{
		puts("pos error");
		return -1;
	}

	for(i=pos+1; i<=p->last; i++)
	{
		p->a[i-1] = p->a[i];
	}

	p->last--;

	return 0;
}

//删除重复数据
int sqlist_delete_repeat(sqlist_t *p)
{
	int i,j; 								//i：基准值的下标， j：依次与基准值比较的下标
	for(i=0; i<p->last; i++)
	{
		for(j=i+1; j<=p->last; j++)
		{
			if(p->a[i] == p->a[j])
			{
				sqlist_delete_pos(p,j);
				j--;
			}
		}
	}
	return 0;
}


//查看顺序表的数据
int sqlist_show(sqlist_t *p)
{
	int i;
	for(i=0; i<=p->last; i++)
	{
		printf("%d  ",p->a[i]);
	}
	putchar(10);

	return 0;
}


int main(int argc, const char *argv[])
{
	sqlist_t *p = sqlist_create();
	sqlist_insert(p,10);
	sqlist_insert(p,20);
	sqlist_insert(p,30);
	sqlist_insert(p,40);
	sqlist_insert(p,50);
	sqlist_insert(p,60);
	printf("插入数据：");
	sqlist_show(p);

	sqlist_delete(p); 			
	printf("删除数据：");
	sqlist_show(p);
	
	sqlist_change(p,30,90);
	printf("更改数据：");
	sqlist_show(p);

	printf("查找位置：%d\n",sqlist_data_search_pos(p,30));
	printf("查找数据：%d\n",sqlist_pos_search_data(p,3));

	sqlist_insert_pos(p,1,15);
	printf("按位置插：");
	sqlist_show(p);

	sqlist_delete_pos(p,1);
	printf("按位置删：");
	sqlist_show(p);

	puts("*********************");

	sqlist_t *w = sqlist_create();
	sqlist_insert(w,20);
	sqlist_insert(w,10);
	sqlist_insert(w,10);
	sqlist_insert(w,10);
	sqlist_insert(w,30);
	sqlist_insert(w,40);
	sqlist_insert(w,50);
	printf("插入数据：");
	sqlist_show(w);

	sqlist_delete_repeat(w);
	printf("删除重复：");
	sqlist_show(w);

	free(p); 				//主动申请，主动释放
	free(w); 				//free释放的是指针所指向的动态区域
	p = NULL;
	w = NULL;


	return 0;
}
