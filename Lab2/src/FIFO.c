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

#include <FIFO.h>

typedef struct Elem {
	Object obj;
	struct Elem *next;
} Elem;

static FIFO new(const Builder);
static void delete(FIFO);
static FIFO clone(FIFO);
static unsigned size(const FIFO);
static void put(FIFO, Object);
static Object get(FIFO);
static Object peek(FIFO);
static void *apply(FIFO this, void *accu, void *(*func)(Object, void*));

static struct FIFO_LT lt = {
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

const struct Builder __FIFO__ = {
	&__Object__,
	(Lookup_Table) &lt,
	sizeof(struct FIFO),
	"FIFO"
};

static FIFO new(const Builder bld)
{
	FIFO f = __FIFO__.super->lt->new(bld);

	if(bld == &__FIFO__)
		bld->lt->lt_initialized = true;
	f->in = NULL;
	f->out = NULL;
	f->size = 0;
	return f;
}

static void delete(FIFO this)
{
	Elem *tmp;

	while((tmp = this->out) != NULL) {
		this->out = this->out->next;
		if(tmp->obj != NULL)
			tmp->obj->lt->delete(tmp->obj);
		free(tmp);
	}
	__FIFO__.super->lt->delete(this);
}

static FIFO clone(FIFO this)
{
	Elem *tmp, *iterator = this->out;
	FIFO f = __FIFO__.super->lt->clone(this);

	if(iterator != NULL) {
		f->out = tmp = (Elem*) xmalloc(sizeof(Elem));
		tmp->obj = iterator->obj != NULL ?
			iterator->obj->lt->clone(iterator->obj) : NULL;

		while((iterator = iterator->next) != NULL) {
			tmp->next = (Elem*) xmalloc(sizeof(Elem));
			tmp->next->obj = iterator->obj != NULL ?
				iterator->obj->lt->clone(iterator->obj) : NULL;
			tmp = tmp->next;
		}
		tmp->next = NULL;
		f->in = tmp;
	}
	return f;
}

static unsigned size(const FIFO this)
{
	return this->size;
}

static void put(FIFO this, Object obj)
{
	Elem *e = (Elem*) xmalloc(sizeof(Elem));

	e->obj = obj;
	e->next = NULL;
	if(this->out != NULL)
		this->in = this->in->next = e;
	else
		this->in = this->out = e;
	this->size++;
}

static Object get(FIFO this)
{
	Elem *tmp = this->out;
	Object ret = NULL;

	if(tmp != NULL) {
		ret = tmp->obj;
		this->out = tmp->next;
		free(tmp);
		this->size--;
	} else
		this->in = NULL;
	return ret;
}

static Object peek(FIFO this)
{
	return this->out == NULL ? NULL : this->out->obj;
}

static void *apply(FIFO this, void *accu, void *(*func)(Object, void*))
{
	Elem *tmp = this->out;

	while(tmp != NULL) {
		accu = func(tmp->obj, accu);
		tmp = tmp->next;
	}

	return accu;
}
