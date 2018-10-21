#include <stdio.h>

int main(int argc, const char *argv[])
{
	int a[3][4] = {0,1,2,3, 4,25,6,7, 28,9,10,11};
	int i,j, row = 0,  line = 0;     			 //row行下标， line列下标

	for(i=0; i<3; i++)
	{
		for(j=0; j<4; j++)
		{
			if(a[row][line] < a[i][j]) 				//row=0,line=1   i=0,j=2
			{
				row = i;
				line = j;
			}
		}
	}

	printf("MAX = %d, row = %d,  line = %d\n",a[row][line], row, line);



	return 0;
}
