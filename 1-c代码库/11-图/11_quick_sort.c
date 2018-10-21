#include <stdio.h>
#include <stdlib.h>

#define  N  10

//快速排序法
int quick_sort(int *a, int low, int high)
{
	int i = low, j = high;
	int key = a[i]; //将第一个数作为基准值

	while (i < j)
	{
		//如果基准值小于最右面的值时，继续执行，j往后
		while(i < j && a[j] >= key) 
		{
			j--;
		}
		//跳出循环，数据进行交换
		a[i] = a[j];	

		//将事先保存好的基准值与左边的值进行比较，如果基准值大，保持不变，i往前
		while(i < j && a[i] <= key)
		{
			i++;
		}
		//跳出循环，数据进行交换
		a[j] = a[i];
	}

	//跳出循环，将基准值放入数据中
	a[i] = key;

	//之前基准值左边的所有数据再次进行快速查找（递归）
	if (i-1 > low) 
	{
		quick_sort(a, low, i-1);
	}

	//之前基准值右边的所有数据再次进行快速查找（递归）
	if (i+1 < high)
	{
		quick_sort(a, i+1, high);
	}

	return 0;
} 

int main(int argc, const char *argv[])
{
	int a[N] = {1, 3, 5, 10, 23, 8, 7, 6, 2, 0};	

	int i = 0;
	
	quick_sort(a, 0, N-1);
	for(i = 0; i < N; i++)
	{
		printf("%d ", a[i]);
	}
	
	putchar(10);
	
	return 0;
}
