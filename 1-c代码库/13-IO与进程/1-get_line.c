#include <stdio.h>
#include <string.h>

#define N 128
int get_line(FILE *fp){
	char buf[N] = {0};
	int line = 0;
	while(fgets(buf, N, fp) != NULL){
		if(buf[strlen(buf) - 1] == '\n')
			line++;
	}
	return line;
}
int main(int argc, char *argv[])
{
	FILE *fp;
	int count = 0;
	if((fp = fopen(argv[1], "r")) == NULL){
		perror("fail to fopen");
		return -1;
	}

	count = get_line(fp);

	printf("line = %d\n", count);
	return 0;
}
