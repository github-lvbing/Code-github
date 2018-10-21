#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	DIR *dir;
	struct dirent *dp;

	if((dir = opendir(".")) == NULL){
		perror("fail to opendir");
		return -1;
	}
//执行一次，返回一个描述文件的结构体指针
	while((dp = readdir(dir)) != NULL){
		if(dp->d_name[0] == '.')
			continue;
		printf("%s\n", dp->d_name);
	}
	return 0;
}
