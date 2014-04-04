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

#include <heap.h>

#define START_MEM	16

static Heap new(const Builder);
static void delete(Heap);
static Heap clone(Heap);
static unsigned size(const Heap);
static void put(Heap, Comparable);
static Comparable get(Heap);
static Comparable peek(Heap);
static void *apply(Heap this, void *accu, void *(*func)(Comparable, void*));

static struct Heap_LT lt = {
	NULL,
	false,
	&new,
	&delete,
	&clone,
	NULL,
	&size,
	&put,
	&get,
	&peek,
	&apply
};

const struct Builder __Heap__ = {
	&__Object__,
	(Lookup_Table) &lt,
	sizeof(struct Heap),
	"Heap"
};

static Heap new(const Builder bld)
{
	Heap h = __Heap__.super->lt->new(bld);

	if(bld == &__Heap__)
		bld->lt->lt_initialized = true;
	h->data = (Comparable*) xmalloc(START_MEM * sizeof(Comparable));
	h->size = 0;

	return h;
}

static void delete(Heap this)
{
	unsigned i;

	for(i = 0; i < this->size; i++)
		this->data[i]->lt->delete(this->data[i]);
	free(this->data);

	__Heap__.super->lt->delete(this);
}

static Heap clone(Heap this)
{
	unsigned i;
	Heap h = __Heap__.super->lt->clone(this);
	
	/* find smallest power of 2 >= this->size. Compatible with 32-bit
	 * integers */
	i = this->size + (this->size == 0) - 1;
	i |= i >> 1;
	i |= i >> 2;
	i |= i >> 4;
	i |= i >> 8;
	i |= i >> 16;
	i++;

	h->data = (Comparable*) xmalloc(i * sizeof(Comparable));
	for(i = 0; i < h->size; i++)
		h->data[i] = this->data[i]->lt->clone(this->data[i]);

	return h;
}

static unsigned size(Heap this)
{
	return this->size;
}

static void put(Heap this, Comparable cp)
{
	Comparable swap;
	unsigned i = this->size++;

	/* if needs more memory */
	if(i >= START_MEM && (i & (i - 1)) == 0)
		this->data = (Comparable*) xrealloc(this->data, (i << 1) * sizeof(Comparable));
	this->data[i] = cp;
	while(i > 0 && cp->lt->compare(cp, this->data[i >> 1]) < 0) {
		swap = this->data[i >> 1];
		this->data[i >> 1] = cp;
		this->data[i] = swap;
		i >>= 1;
	}
}

static Comparable get(Heap this)
{
	Comparable cp = NULL, swap;
	unsigned i = 1, min;

	if(this->size > 0) {
		cp = this->data[0];
		this->data[0] = this->data[--this->size];
		while(i + 1 < this->size) {
			min = this->data[i]->lt->compare(this->data[i], this->data[i + 1]) < 0 ? i : i + 1;
			if(this->data[i]->lt->compare(this->data[i << 1], this->data[min]) > 0) {
				swap = this->data[i >> 1];
				this->data[i >> 1] = this->data[min];
				this->data[min] = swap;
				i = min << 1;
			} else
				break;
		}
		if(i < this->size && this->data[i]->lt->compare(this->data[i >> 1], this->data[i]) > 0) {
			swap = this->data[i >> 1];
			this->data[i >> 1] = this->data[i];
			this->data[i] = swap;
		}
	}

	return cp;
}

static Comparable peek(Heap this)
{
	return this->data[0];
}

static void *apply(Heap this, void *accu, void *(*func)(Comparable, void*))
{
	unsigned i;

	for(i = 0; i < this->size; i++)
		accu = func(this->data[i], accu);

	return accu;
}
