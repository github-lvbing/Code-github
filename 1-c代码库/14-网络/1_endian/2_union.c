//测试主机字节序
//方法二：使用共用体

#include <stdio.h>

union un{
	int a;
	char b;
};

int main(int argc, const char *argv[])
{
	union un myun;

	myun.a = 0x12345678;

	printf("a = %#x\n", myun.a);
	printf("b = %#x\n", myun.b);

	if(myun.b == 0x78)
	{
		printf("小端存储\n");
	}
	else
	{
		printf("大端存储\n");
	}

	return 0;
}
