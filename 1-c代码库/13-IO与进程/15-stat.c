#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	struct stat buf;
	if(stat(argv[1], &buf) == 0){
		printf("ino = %d link = %d uid = %d gid = %d size = %d\n", 
				(int)buf.st_ino, buf.st_nlink, buf.st_uid, buf.st_gid, (int)buf.st_size);
	}
	return 0;
}
