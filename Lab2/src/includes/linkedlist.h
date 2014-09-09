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
 *  along with Lab2-Scheduler.  If not, see <http://www.gnu.org/licenses/>.
 */

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
	char *(*to_string)(const struct LinkedList*);
	unsigned (*size)(struct LinkedList* const);
	void (*put)(struct LinkedList*, Object);
	Object (*get)(struct LinkedList*);
	Object (*peek)(struct LinkedList*);
	void *(*apply)(struct LinkedList*, void*, void *(*)(Object, void*));
};

typedef struct LinkedList_LT* LinkedList_LT;

struct LinkedList {
	LinkedList_LT lt;
	struct Elem *head;
	unsigned size;
};

typedef struct LinkedList* LinkedList;

extern const struct Builder __LinkedList__;

#endif
