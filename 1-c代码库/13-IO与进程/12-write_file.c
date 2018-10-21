#include <stdio.h>
#include <string.h>

#define N 32
int main(int argc, char *argv[])
{
	FILE *fp;
	char buf[N];
	if((fp = fopen("test.txt", "w")) == NULL){
		perror("fail to fopen");
		return -1;
	}
	
	while(fgets(buf, N, stdin) != NULL){
		buf[strlen(buf) - 1] = '\0';
		fputs(buf, fp);

		fflush(fp);
	}
	return 0;
}
