//测试主机字节序
//方法一：使用地址（指针）

#include <stdio.h>

int main(int argc, const char *argv[])
{
	int a = 0x12345678;

	char *p = (char *)&a;

	printf("a = %#x\n", a);
	printf("*p = %#x\n", *p);

	if(*p == 0x78)
	{
		printf("小端存储\n");
	}
	else
	{
		printf("大端存储\n");
	}

	return 0;
}
