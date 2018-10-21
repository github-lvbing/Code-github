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
	if((fp = fopen("test.txt", "r")) == NULL){
		perror("fail to fopen");
		return -1;
	}

	if(fread(&m, sizeof(struct worker), 1, fp) > 0){
		printf("num = %d\n", m.num);
		printf("name = %s\n", m.name);
	}

	return 0;
}
