#include <stdio.h>

#define N 32
int main(int argc, char *argv[])
{
	FILE *fp;
	char buf[N];
	if((fp = fopen("test.txt", "r")) == NULL){
		perror("fail to fopen");
		return -1;
	}

	if(fgets(buf, N, fp) != NULL){
		printf("%s\n", buf);
	}
	return 0;
}
