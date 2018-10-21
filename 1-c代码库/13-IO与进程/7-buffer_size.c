#include <stdio.h>

int main(int argc, char *argv[])
{
	FILE *fp;

	if((fp = fopen("test", "w")) == NULL){
		perror("fail to fopen");
		return -1;
	}

	fputc('a', fp);
	printf("total size = %d\n", fp->_IO_buf_end - fp->_IO_buf_base);
	getchar();
	printf("stdin size = %d\n", stdin->_IO_buf_end - stdin->_IO_buf_base);
	printf("stdout size = %d\n", stdout->_IO_buf_end - stdout->_IO_buf_base);
	printf("stderr size = %d\n", stderr->_IO_buf_end - stderr->_IO_buf_base);


	return 0;
}
