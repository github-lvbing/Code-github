#include <stdio.h>

int main(int argc, char *argv[])
{
	FILE *fp;

	if((fp = fopen("test.txt", "w+")) == NULL){
		perror("fail to fopen");
		return -1;
	}

	fputc('a', fp);
	fputc('b', fp);
	fputc('c', fp);
	fputc('d', fp);

	int ch;
	long offset;

	fseek(fp, 2, SEEK_SET);
	offset = ftell(fp);
	printf("offset = %ld\n", offset);

	if((ch = fgetc(fp)) != EOF){
		printf("%c\n", ch);
	}

	fseek(fp, -1, SEEK_CUR);
	offset = ftell(fp);
	printf("offset = %ld\n", offset);

	return 0;
}
