/*
 *  Copyright (C) 2014 Julien Rabinow <jnr305@nyu.edu>
 *
 *  This file is part of Lab3-Mem-manager.
 *
 *  Lab3-Mem-manager is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Lab3-Mem-manager is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Lab3-Mem-manager. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <utils.h>

struct Builder {
	const struct Builder *super;
	struct Lookup_Table *lt;
	const unsigned obj_size;
	const char *name;
};

/* super: object inherits from object type 'super'
 * lt: Lookup table, or struct of pointers to functions. See below for more
 * info.
 * obj_size: amount of memory to allocate for an object. Can be determined with
 * 'sizeof' lt_initialized: do we need to 'underride' methods for this builder?
 * You know what override means, now guess what underride does :)
 * name: char[] description of the object type */

typedef struct Builder* Builder;

/* The "lookup table" contains a pointer to the object builder. All objects
 * (structures that inherit the methods of Object) contain a lookup table.
 * Different objects extend the lookup table various ways, but the beginning
 * of the lookup table MUST be identical. This way, we can cast objects from
 * one type to another, and all functions will still be correctly located
 * from the compiler's point of view.
 * BEWARE: this technique depends on the compiler not internally rearranging
 * your structure definitions */

struct Lookup_Table {
	Builder bld;
	bool lt_initialized;
	void *(*new)(const Builder, ...);
	void (*delete)(void*);
	void *(*clone)(const void*);
	char *(*to_string)(const void*);
};

typedef struct Lookup_Table* Lookup_Table;

struct Object {
	Lookup_Table lt;
};

typedef struct Object* Object;

bool instance_of(const Object obj, const Builder bld);

extern const struct Builder __Object__;

#define instance_of(obj, bld)	instance_of((const Object) obj, (const Builder)\
		&__##bld##__)

#endif
