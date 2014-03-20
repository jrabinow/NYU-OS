#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <object.h>
#include <utils.h>

struct LinkedList_LT {
	Builder bld;
	bool lt_initialized;
	struct LinkedList *(*new)(const Builder);
	void (*delete)(struct LinkedList*);
	struct LinkedList *(*clone)(struct LinkedList*);
	void (*print)(const struct LinkedList*);
	void (*put)(struct LinkedList*, Object);
	Object (*get)(struct LinkedList*);
	Object (*peek)(struct LinkedList*);
	void *(*apply)(struct LinkedList*, void*, void *(*)(Object, void*));
};

typedef struct LinkedList_LT* LinkedList_LT;

struct LinkedList {
	LinkedList_LT lt;
	struct Elem *head;
};

typedef struct LinkedList* LinkedList;

extern const struct Builder __LinkedList__;

#endif
