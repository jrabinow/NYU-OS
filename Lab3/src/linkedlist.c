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

#include <linkedlist.h>

typedef struct Elem {
	Object obj;
	struct Elem *next;
} Elem;

static LinkedList new(const Builder);
static void delete(LinkedList);
static LinkedList clone(LinkedList);
static unsigned size(const LinkedList);
static void put(LinkedList, Object);
static Object get(LinkedList);
static Object peek(LinkedList);
static void *apply(LinkedList this, void *accu, void *(*func)(Object, void*));

static struct LinkedList_LT lt = {
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

const struct Builder __LinkedList__ = {
	&__Object__,
	(Lookup_Table) &lt,
	sizeof(struct LinkedList),
	"LinkedList"
};

static LinkedList new(const Builder bld)
{
	LinkedList l = __LinkedList__.super->lt->new(bld);

	if(bld == &__LinkedList__)
		bld->lt->lt_initialized = true;
	l->head = NULL;
	l->size = 0;
	return l;
}

static void delete(LinkedList this)
{
	Elem *tmp;
	while((tmp = this->head) != NULL) {
		this->head = this->head->next;
		if(tmp->obj != NULL)
			tmp->obj->lt->delete(tmp->obj);
		free(tmp);
	}
	__LinkedList__.super->lt->delete(this);
}

static LinkedList clone(LinkedList this)
{
	Elem *tmp, *iterator = this->head;
	LinkedList l = __LinkedList__.super->lt->clone(this);

	if(this->head != NULL) {
		l->head = tmp = (Elem*) xmalloc(sizeof(Elem));
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
	return l;
}

static unsigned size(const LinkedList this)
{
	return this->size;
}

static void put(LinkedList this, Object obj)
{
	Elem *e = (Elem*) xmalloc(sizeof(Elem));

	e->obj = obj;
	e->next = this->head;
	this->head = e;
	this->size++;
}

static Object get(LinkedList this)
{
	Elem *tmp = this->head;
	Object ret = NULL;

	if(tmp != NULL) {
		ret = tmp->obj;
		this->head = tmp->next;
		free(tmp);
		this->size--;
	}
	return ret;
}

static Object peek(LinkedList this)
{
	return this->head == NULL ? NULL : this->head->obj;
}

static void *apply(LinkedList this, void *accu, void *(*func)(Object, void*))
{
	Elem *tmp = this->head;

	while(tmp != NULL) {
		accu = func(tmp->obj, accu);
		tmp = tmp->next;
	}

	return accu;
}
