#ifndef HEAP_H
#define HEAP_H

#include <comparable.h>
#include <utils.h>

struct Heap_LT {
	Builder bld;
	bool lt_initialized;
	struct Heap *(*new)(const Builder);
	void (*delete)(struct Heap*);
	struct Heap *(*clone)(struct Heap*);
	void (*print)(const struct Heap*);
	void (*put)(struct Heap*, Comparable);
	Comparable (*get)(struct Heap*);
	Comparable (*peek)(struct Heap*);
	void *(*apply)(struct Heap*, void*, void *(*)(Comparable, void*));
};

typedef struct Heap_LT* Heap_LT;

struct Heap {
	Heap_LT lt;
	Comparable *data;
	unsigned size;
};

typedef struct Heap* Heap;

extern const struct Builder __Heap__;

#endif
