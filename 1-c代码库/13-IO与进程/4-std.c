#include <stdio.h>

int main(int argc, char *argv[])
{
	int ch;

	while((ch = fgetc(stdin)) != EOF){
		fputc(ch, stdout);
	}
	return 0;
}
