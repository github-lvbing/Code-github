#include <stdio.h>
#include <string.h>

#define N 32
struct worker{
	int num;
	char name[N];
};
int main(int argc, char *argv[])
{
	FILE *fp;
	struct worker m;
	if((fp = fopen("test.txt", "w")) == NULL){
		perror("fail to fopen");
		return -1;
	}

	m.num = 10;
	strcpy(m.name, "beijing");

	if(fwrite(&m, sizeof(struct worker), 1, fp) < 0){
		perror("fail to fwrite");
		return -1;
	}

	return 0;
}
