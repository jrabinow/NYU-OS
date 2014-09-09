#include "includes/utils.h"

typedef struct {
	struct Elem *next;
	char data[5];
} Elem;

int main(int argc, char **argv)
{
	int i;
	struct mempool mp;

	void *a, *b, *c, *d, *e;

	printf("%d %d\n", sizeof(Elem), sizeof(unsigned));
	mempool_create(&mp, sizeof(Elem), 5);

	a = mempool_alloc(&mp);
	b = mempool_alloc(&mp);
	c = mempool_alloc(&mp);
	d = mempool_alloc(&mp);
	printf("%d\n%d\n%d\n%d\n", a, b, c, d);
	mempool_free(&mp, b);
	e = mempool_alloc(&mp);
	b = mempool_alloc(&mp);
	printf("----------------\n%d\n%d\n%d\n%d\n%d\n", a, b, c, d, e);

	mempool_delete(&mp);

	return 0;
}
