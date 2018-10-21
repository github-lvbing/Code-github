#include <stdio.h>
#include <string.h>
#include <time.h>

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
	if((fp = fopen(argv[1], "a")) == NULL){
		perror("fail to fopen");
		return -1;
	}

	count = get_line(fp);
	
	time_t tm;
	struct tm *tv;
	while(1){
		tm = time(&tm);
		tv = localtime(&tm);

		fprintf(fp, "%d %d-%d-%d %d:%d:%d\n", ++count, tv->tm_year+1900, tv->tm_mon+1, tv->tm_mday,
				tv->tm_hour, tv->tm_min, tv->tm_sec);
		sleep(1);
		fflush(fp);
	}
	return 0;
}
