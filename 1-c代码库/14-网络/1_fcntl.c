#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define N 128

int main(int argc, const char *argv[])
{
	char buf[N] = {};

	int flags;

	//使用fcntl函数实现非阻塞I/O
	//对位或者寄存器的操作，一般使用读、改、写三步

	//第一步：读取文件描述符状态标志位
	if((flags = fcntl(0, F_GETFL)) < 0)
	{
		perror("fail to fcntl");
		exit(1);
	}

	//第二步：修改文件状态标识位
	flags = flags | O_NONBLOCK;
	//flags |= O_NONBLOCK;  //000004000

	//第三步：将新的文件状态标志位写回去
	if(fcntl(0, F_SETFL, flags) < 0)
	{
		perror("fail to fcntl");
		exit(1);
	}

	while(1)
	{
		fgets(buf, N, stdin);

		sleep(1);

		printf("****************\n");
	}

	return 0;
}
