#include <stdio.h>

int main(int argc, char *argv[])
{
	FILE *fp;

	if((fp = fopen("test.txt", "r")) == NULL){
		perror("fail to fopen");
		return -1;
	}

	int ch;

	if((ch = fgetc(fp)) != EOF){
		printf("ch = %c\n", ch);
	}
	return 0;
}
