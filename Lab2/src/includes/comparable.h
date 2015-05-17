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

#ifndef COMPARABLE_H
#define COMPARABLE_H

/* Interface. No actual Comparable object will be created. However, we now have
 * a common interface to interact with all objects implementing comparable.
 * To implement an interface, all that needs to be done is to define the
 * object's lookup table as extending the interface's lookup table. Also, all
 * variables declared in the interface's class definition must also be
 * declared in the object */

#include <object.h>

struct Comparable_LT {
	Builder bld;
	bool lt_initialized;
	struct Comparable *(*new)(const struct Builder*, ...);
	void (*delete)(struct Comparable*);
	struct Comparable *(*clone)(const struct Comparable*);
	char *(*to_string)(const struct Comparable*);
	int (*compare)(const struct Comparable*, const struct Comparable*);
};

typedef struct Comparable_LT* Comparable_LT;

struct Comparable {
	Comparable_LT lt;
};

typedef struct Comparable* Comparable;

#endif
