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

#include <stack.h>

typedef struct Elem {
	Object obj;
	struct Elem *next;
} Elem;

static Stack new(const Builder);
static void delete(Stack);
static Stack clone(Stack);
static unsigned size(const Stack);
static void push(Stack, Object);
static Object pop(Stack);
static Object peek(Stack);

static struct Stack_LT lt = {
	NULL,
	false,
	&new,
	&delete,
	&clone,
	NULL,
	&size,
	&push,
	&pop,
	&peek
};

const struct Builder __Stack__ = {
	&__Object__,
	(Lookup_Table) &lt,
	sizeof(struct Stack),
	"Stack"
};

static Stack new(const Builder bld)
{
	Stack s = __Stack__.super->lt->new(bld);

	if(bld == &__Stack__)
		bld->lt->lt_initialized = true;
	s->sp = NULL;
	s->size = 0;
	return s;
}

static void delete(Stack this)
{
	Elem *tmp;
	while((tmp = this->sp) != NULL) {
		this->sp = this->sp->next;
		if(tmp->obj != NULL)
			tmp->obj->lt->delete(tmp->obj);
		free(tmp);
	}
	__Stack__.super->lt->delete(this);
}

static Stack clone(Stack this)
{
	Elem *tmp, *iterator = this->sp;
	Stack s = __Stack__.super->lt->clone(this);

	if(iterator != NULL) {
		s->sp = tmp = (Elem*) xmalloc(sizeof(Elem));
		tmp->obj = iterator->obj != NULL ?
			iterator->obj->lt->clone(iterator->obj) : NULL;

		while((iterator = iterator->next) != NULL) {
			tmp->next = (Elem*) xmalloc(sizeof(Elem));
			tmp->next->obj = iterator->obj != NULL ?
				iterator->obj->lt->clone(iterator->obj) : NULL;
			tmp = tmp->next;
		}
		tmp->next = NULL;
	}
	return s;
}

static unsigned size(const Stack this)
{
	return this->size;
}

static void push(Stack this, Object obj)
{
	Elem *e = (Elem*) xmalloc(sizeof(Elem));

	e->obj = obj;
	e->next = this->sp;
	this->sp = e;
	this->size++;
}

static Object pop(Stack this)
{
	Elem *tmp = this->sp;
	Object ret = NULL;

	if(tmp != NULL) {
		ret = tmp->obj;
		this->sp = tmp->next;
		free(tmp);
		this->size--;
	}
	return ret;
}

static Object peek(Stack this)
{
	return this->sp == NULL ? NULL : this->sp->obj;
}

