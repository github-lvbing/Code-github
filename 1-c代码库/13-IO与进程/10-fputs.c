#include <stdio.h>

#define N 32
int main(int argc, char *argv[])
{
	FILE *fp;
	char buf[N] = "hello world";	
	if((fp = fopen("test.txt", "w")) == NULL){
		perror("fail to fopen");
		return -1;
	}

	if(fputs(buf, fp) == EOF){
		perror("fail to fputs");
		return -1;
	}
	return 0;
}
