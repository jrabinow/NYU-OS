#ifndef STACK_H
#define STACK_H

#include <object.h>
#include <utils.h>

struct Stack_LT {
	Builder bld;
	bool lt_initialized;
	struct Stack *(*new)(const Builder);
	void (*delete)(struct Stack*);
	struct Stack *(*clone)(struct Stack*);
	void (*print)(const struct Stack*);
	void (*push)(struct Stack*, Object);
	Object (*pop)(struct Stack*);
	Object (*peek)(struct Stack*);
};

typedef struct Stack_LT* Stack_LT;

struct Stack {
	Stack_LT lt;
	struct Elem *sp;
};

typedef struct Stack* Stack;

extern const struct Builder __Stack__;

#endif
