#include <stdio.h>

#define N 10

int main(int argc, const char *argv[])
{
	int i,j,temp = 0;
	int a[N] = {9,8,17,6,15,4,3,12,1,10};

	for(i=0; i<N-1; i++) 				//i：循环的轮数
	{
		for(j=0; j<N-i-1; j++) 			//j：代表前一个数的下标
		{
			if(a[j] > a[j+1])
			{
			//	temp = a[j+1];
			//	a[j+1] = a[j];
			//	a[j] = temp;

				a[j+1] ^= a[j];
				a[j]   ^= a[j+1];
				a[j+1] ^= a[j];
			}
		}
	}

	for(i=0; i<N; i++)
	{
		printf("%d ",a[i]);
	}
	putchar(10);


	return 0;
}
