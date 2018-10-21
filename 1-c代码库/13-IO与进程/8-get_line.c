#include <stdio.h>

int main(int argc, char *argv[])
{
	FILE *fp;
	int ch;
	int count = 0;

	if(argc != 2){
		printf("Usage: %s + filename\n", argv[0]);
		return -1;
	}
	if((fp = fopen(argv[1], "r")) == NULL){
		perror("fail to fopen");
		return -1;
	}
	
	while((ch = fgetc(fp)) != EOF){
		if(ch == '\n'){
			count++;
		}
	}

	printf("count = %d\n", count);
	return 0;
}
