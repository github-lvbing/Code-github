#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	printf("exit before");

//	_exit(0);
	exit(0);
	printf("exit after\n");
	return 0;
}
