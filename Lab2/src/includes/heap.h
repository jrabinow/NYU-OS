/*
 *  Copyright (C) 2014 Julien Rabinow <jnr305@nyu.edu>
 *
 *  This file is part of Lab2-Scheduler.
 *
 *  Lab2-Scheduler is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Lab2-Scheduler is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Lab2-Scheduler. If not, see <http://www.gnu.org/licenses/>.
 */

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
	char *(*to_string)(const struct Heap*);
	unsigned (*size)(struct Heap* const);
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
