#include <stdio.h>

int main(int argc, char *argv[])
{
	FILE *fp;

	if((fp = fopen("test.txt", "w")) == NULL){
		perror("fail to fopen");
		return -1;
	}

	if(fputc(99, fp) == EOF){
		perror("fail to fputc");
		return -1;
	}

	return 0;
}
