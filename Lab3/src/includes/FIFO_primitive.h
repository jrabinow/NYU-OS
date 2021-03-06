/*
 *  Copyright (C) 2014 Julien Rabinow <jnr305@nyu.edu>
 *
 *  This file is part of Lab4-ioscheduler.
 *
 *  Lab4-iocheduler is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Lab4-ioscheduler is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Lab4-ioscheduler. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FIFO_H
#define FIFO_H

#include <object.h>
#include <utils.h>

#include <request.h>

struct FIFO_LT {
	Builder bld;
	bool lt_initialized;
	struct FIFO *(*new)(const Builder bld);
	void (*delete)(struct FIFO*);
	struct FIFO *(*clone)(struct FIFO*);
	char *(*to_string)(const struct FIFO*);
	unsigned (*size)(struct FIFO* const);
	void (*put)(struct FIFO*, Request);
	Request (*get)(struct FIFO*);
	Request (*peek)(struct FIFO*);
	void *(*apply)(struct FIFO*, void*, void *(*)(Request, void*));
};

typedef struct FIFO_LT* FIFO_LT;

struct FIFO {
	FIFO_LT lt;
	struct Elem *in;
	struct Elem *out;
	unsigned size;
	struct mempool mp;
};

typedef struct FIFO* FIFO;

extern const struct Builder __FIFO__;

#endif
