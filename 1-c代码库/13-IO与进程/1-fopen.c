#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
	FILE *fp;

	if((fp = fopen("test.txt", "w")) == NULL){
		perror("fail to fopen");
//		printf("fail to fopen:%s\n", strerror(errno));
		return -1;
	}
	return 0;
}
