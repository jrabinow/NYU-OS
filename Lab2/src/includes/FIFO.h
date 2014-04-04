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
	char *(*to_string)(const struct FIFO*);
	unsigned (*size)(struct FIFO* const);
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
	unsigned size;
};

typedef struct FIFO* FIFO;

extern const struct Builder __FIFO__;

#endif
