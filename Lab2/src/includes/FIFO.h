#ifndef FIFO_H
#define FIFO_H

#include <object.h>
#include <utils.h>

struct FIFO_LT {
	Builder bld;
	bool lt_initialized;
	struct FIFO *(*new)(const Builder bld);
	void (*delete)(struct FIFO*);
	struct FIFO *(*clone)(struct FIFO*);
	void (*print)(const struct FIFO*);
	void (*put)(struct FIFO*, Object);
	Object (*get)(struct FIFO*);
	Object (*peek)(struct FIFO*);
	void *(*apply)(struct FIFO*, void*, void *(*)(Object, void*));
};

typedef struct FIFO_LT* FIFO_LT;

struct FIFO {
	FIFO_LT lt;
	struct Elem *in;
	struct Elem *out;
};

typedef struct FIFO* FIFO;

extern const struct Builder __FIFO__;

#endif
