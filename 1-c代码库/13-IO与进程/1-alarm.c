#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	unsigned int ret;
	ret = alarm(7);
	printf("ret = %d\n", ret);

	sleep(3);

	ret = alarm(4);
	printf("ret = %d\n", ret);

	while(1)
		;
		
	return 0;
}
