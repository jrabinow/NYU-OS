#include "includes/utils.h"
#include <assert.h>

#define MEMPOOL_SIZE	5

typedef struct {
	struct Elem *next;
	char data[5];
} Elem;

int main(int argc, char **argv)
{
	int i;
	struct mempool mp;
	void *memptrs[MEMPOOL_SIZE], *tmpptr;

	assert(MEMPOOL_SIZE == 5);
	mempool_create(&mp, sizeof(Elem), MEMPOOL_SIZE);
	assert(mp.mem != NULL);
	assert(mp.ptrs != NULL);

	for(i = 0; i < MEMPOOL_SIZE - 1; i++) {
		memptrs[i] = mempool_alloc(&mp);
		if(i > 0)
			assert(memptrs[i-1] + sizeof(Elem) + sizeof(unsigned) == memptrs[i]);
	}
	tmpptr = memptrs[1];
	mempool_free(&mp, memptrs[1]);
	memptrs[1] = mempool_alloc(&mp);
	assert(tmpptr == memptrs[1]);
	memptrs[MEMPOOL_SIZE - 1] = mempool_alloc(&mp);
	assert(memptrs[MEMPOOL_SIZE - 2] + sizeof(Elem) + sizeof(unsigned) == memptrs[MEMPOOL_SIZE - 1]);
	tmpptr = mempool_alloc(&mp);
	assert(tmpptr == NULL);

	mempool_delete(&mp);

	puts("All tests passed");

	return 0;
}
