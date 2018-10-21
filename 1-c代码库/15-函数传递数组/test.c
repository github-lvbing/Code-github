#include "stdio.h"


int  *k;

void func(int pp[])
{
	k = pp;
	printf("%d\n",k[0]);
	printf("%d\n",k[1]);

}

int main(int argc, const char *argv[])
{
	int  k[2] = {11,24};
	func(k);
	return 0;
}
