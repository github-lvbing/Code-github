#include <stdio.h>
#include <stdlib.h>

static int  itoa(char *str)
{
  int i = 0, sum = 0;
  while('0' < str[i]  &&  str[i] < '9'){
    sum =  sum *10  +  str[i] - '0';
    i++;
  }
  return sum;
}


int main(int argc, const char *argv[])
{
	char a[] = "123A45";

	printf("%s\n",a);
//	printf("%d\n", atoi(a) ); 		//将数值型的字符串，转化成整数；遇到非数值形式的字符，停止转换


	printf("sum = %d\n",itoa(a));


	return 0;
}
