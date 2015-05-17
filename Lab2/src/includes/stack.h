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
	char *(*to_string)(const struct Stack*);
	unsigned (*size)(struct Stack* const);
	void (*push)(struct Stack*, Object);
	Object (*pop)(struct Stack*);
	Object (*peek)(struct Stack*);
};

typedef struct Stack_LT* Stack_LT;

struct Stack {
	Stack_LT lt;
	struct Elem *sp;
	unsigned size;
};

typedef struct Stack* Stack;

extern const struct Builder __Stack__;

#endif
