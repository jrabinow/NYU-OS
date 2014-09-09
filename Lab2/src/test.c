#include <stdio.h>

#define START_MEM 8

int main(int argc, char **argv)
{
	int i;
	for(i = 0; i < 65; i++)
		if(i >= START_MEM && (i & (i - 1)) == 0)
			printf("%d\n", i);
	printf("%d\n", sizeof(size_t));
	return 0;
}
