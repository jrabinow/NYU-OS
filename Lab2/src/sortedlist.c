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

#include <sortedlist.h>

typedef struct Elem {
	Comparable obj;
	struct Elem *next;
} Elem;

static SortedList new(const Builder, int);
static void delete(SortedList);
static SortedList clone(SortedList);
static unsigned size(const SortedList);
static void insert(SortedList, Comparable);
static Comparable get(SortedList);
static Comparable peek(SortedList);
static void *apply(SortedList this, void *accu, void *(*func)(Comparable, void*));

static struct SortedList_LT lt = {
	NULL,
	false,
	&new,
	&delete,
	&clone,
	NULL,
	&size,
	&insert,
	&get,
	&peek,
	&apply
};

const struct Builder __SortedList__ = {
	&__Object__,
	(Lookup_Table) &lt,
	sizeof(struct SortedList),
	"SortedList"
};

static SortedList new(const Builder bld, int cmp_func_index)
{
	SortedList l = __SortedList__.super->lt->new(bld);

	if(bld == &__SortedList__)
		bld->lt->lt_initialized = true;
	l->head = NULL;
	l->size = 0;
	l->cmp_func = cmp_func_index;
	return l;
}

static void delete(SortedList this)
{
	Elem *tmp;
	while((tmp = this->head) != NULL) {
		this->head = this->head->next;
		if(tmp->obj != NULL)
			tmp->obj->lt->delete(tmp->obj);
		free(tmp);
	}
	__SortedList__.super->lt->delete(this);
}

static SortedList clone(SortedList this)
{
	Elem *tmp, *iterator = this->head;
	SortedList l = __SortedList__.super->lt->clone(this);

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

static unsigned size(const SortedList this)
{
	return this->size;
}

static void insert(SortedList this, Comparable obj)
{
	Elem *e = (Elem*) xmalloc(sizeof(Elem)), *iterator = this->head, *prev = NULL;

	e->obj = obj;
	while(iterator != NULL &&
		obj->lt->cmp_array[this->cmp_func](obj, iterator->obj) <= 0) {
		prev = iterator;
		iterator = iterator->next;
	}
	e->next = iterator;
	if(prev == NULL)
		this->head = e;
	else
		prev->next = e;
	this->size++;

#ifdef DEBUG
	puts("---------- SORTED_LIST ---------");
	char *obj_str;
	fprintf(stderr, "size = %d\n", this->size);
	for(e = this->head; e != NULL; e = e->next) {
		obj_str = e->obj->lt->to_string(e->obj);
		puts(obj_str);
		free(obj_str);
	}
	puts("--------------------------------");
#endif
}

static Comparable get(SortedList this)
{
	Elem *tmp = this->head;
	Comparable ret = NULL;

	if(tmp != NULL) {
		ret = tmp->obj;
		this->head = tmp->next;
		free(tmp);
		this->size--;
	}
	return ret;
}

static Comparable peek(SortedList this)
{
#ifdef DEBUG
	Elem *e;
	puts("---------- SORTED_LIST ---------");
	char *obj_str;
	fprintf(stderr, "size = %d\n", this->size);
	for(e = this->head; e != NULL; e = e->next) {
		obj_str = e->obj->lt->to_string(e->obj);
		fputs(obj_str, stderr);
		free(obj_str);
	}
	puts("--------------------------------");
#endif
	return this->head == NULL ? NULL : this->head->obj;
}

static void *apply(SortedList this, void *accu, void *(*func)(Comparable, void*))
{
	Elem *tmp = this->head;

	while(tmp != NULL) {
		accu = func(tmp->obj, accu);
		tmp = tmp->next;
	}

	return accu;
}
