#ifndef COMPARABLE_H
#define COMPARABLE_H

/* Interface. No actual Comparable object will be created. However, we now have a
 * common interface to interact with all objects implementing comparable.
 * To implement an interface, all that needs to be done is to define the object's
 * lookup table as extending the interface's lookup table. Also, all variables
 * declared in the interface's class definition must also be declared in the object */

#include <object.h>

struct Comparable_LT {
	Builder bld;
	bool lt_initialized;
	struct Comparable *(*new)(const struct Builder*, ...);
	void (*delete)(struct Comparable*);
	struct Comparable *(*clone)(const struct Comparable*);
	void (*print)(const struct Comparable*);
	int (*compare)(const struct Comparable*, const struct Comparable*);
};

typedef struct Comparable_LT* Comparable_LT;

struct Comparable {
	Comparable_LT lt;
};

typedef struct Comparable* Comparable;

#endif
