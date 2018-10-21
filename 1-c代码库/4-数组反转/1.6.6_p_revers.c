#include <stdio.h>

int main(int argc, const char *argv[])
{
	int a[] = {1,2,3,4,5,6,7};
	int *p = a;
	int i,j,temp;
	int n = sizeof(a)/sizeof(int);

	for(i=0,j=n-1;  i<j ; i++,j--)
	{
	//	 temp = p[i];
	//	 p[i] = p[j];
	//	 p[j] = temp;
	
		p[i] ^= p[j];
		p[j] ^= p[i];
		p[i] ^= p[j];
	}

	for(i=0; i<n; i++)
	{
		printf("a[%d] = %d\n",i, a[i]);
	}


	return 0;
}
