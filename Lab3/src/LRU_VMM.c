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


#include <LRU_VMM.h>

static VMM new(const Builder, int);
static void delete(VMM);
static VMM clone(VMM);
static int get_frame_index(VMM);
static void update(VMM, int);

static struct VMM_LT lt = {
	NULL,
	false,
	&new,
	&delete,
	&clone,
	NULL,
	NULL,
	&get_frame_index,
	&update
};

const struct Builder __LRU_VMM__ = {
	&__VMM__,
	(Lookup_Table) &lt,
	sizeof(struct LRU_VMM),
	"LRU_VMM"
};

static VMM new(const Builder bld, int num_frames)
{
	LRU_VMM this = __LRU_VMM__.super->lt->new(bld, num_frames);
	int i;

	if(bld == &__LRU_VMM__)
		bld->lt->lt_initialized = true;

	this->list_ptrs = (struct Elem**)
		xmalloc(sizeof(struct Elem*) * num_frames);
	this->list_ptrs[num_frames - 1] = (struct Elem*)
		xmalloc(sizeof(struct Elem));
	this->list_ptrs[num_frames - 1]->index = num_frames - 1;
	this->list_ptrs[num_frames - 1]->next = NULL;

	for(i = num_frames - 2; i >= 0; i--) {
		this->list_ptrs[i] = (struct Elem*)
			xmalloc(sizeof(struct Elem));
		this->list_ptrs[i]->next = this->list_ptrs[i+1];
		this->list_ptrs[i]->index = i;
	}
	this->list_ptrs[0]->prev = NULL;
	for(i = 1; i < num_frames - 1; i++)
		this->list_ptrs[i]->prev = this->list_ptrs[i-1];

	this->sorted_list = this->list_ptrs[0];
	this->tail = this->list_ptrs[num_frames - 1];

	return (VMM) this;
}

static void delete(VMM vmm)
{
	LRU_VMM this = (LRU_VMM) vmm;
	unsigned i;

	for(i = 0; i < this->num_frames; i++)
		free(this->list_ptrs[i]);
	free(this->list_ptrs);
	
	__LRU_VMM__.super->lt->delete(this);
}

static VMM clone(const VMM vmm)
{
	struct Elem *tmp, *iterator;
	int index;
	LRU_VMM this = (LRU_VMM) vmm,
		new_lru = __LRU_VMM__.super->lt->clone(this);

	iterator = this->sorted_list;

	new_lru->sorted_list = tmp = (struct Elem*) xmalloc(sizeof(struct Elem));
	tmp->index = iterator->index;

	if(this->page_table[tmp->index].present) {
		index = this->page_table[tmp->index].frame_number;
		this->list_ptrs[index] = tmp;
	}
	while((iterator = iterator->next) != NULL) {
		tmp->next = (struct Elem*) xmalloc(sizeof(struct Elem));
		tmp->next->index = iterator->index;
		if(this->page_table[tmp->index].present) {
			index = this->page_table[tmp->index].frame_number;
			this->list_ptrs[index] = tmp;
		}
		tmp = tmp->next;
	}
	tmp->next = NULL;
	this->tail = tmp;

	return (VMM) new_lru;
}

static int get_frame_index(VMM vmm)
{
	LRU_VMM this = (LRU_VMM) vmm;
	struct Elem *tmp = this->sorted_list;

#ifdef DEBUG
	/*puts(__func__); */
	unsigned i;
	puts("BEFORE");
	printf("this->sorted_list: %p\tthis->tail: %p\n", this->sorted_list, this->tail);
	for(i = 0; i < this->num_frames; i++) {
		printf("%d: %p (%d) ->", i, this->list_ptrs[i], this->frame_table[this->list_ptrs[i]->index]);
		if(this->list_ptrs[i] != NULL)
			printf("%p\n", this->list_ptrs[i]->next);
		else
			puts("null");
	}
#endif
	/* remove list head */
	if(tmp->next != NULL) {
		this->sorted_list = tmp->next;
		tmp->next->prev = NULL;
	}
	/* put element tmp in last position of list */
	if(this->tail != tmp) {
		tmp->prev = this->tail;
		this->tail = this->tail->next = tmp;
		tmp->next = NULL;
	}
#ifdef DEBUG
	puts("AFTER");
	printf("this->sorted_list: %p\tthis->tail: %p\n", this->sorted_list, this->tail);
	for(i = 0; i < this->num_frames; i++) {
		printf("%d: %p (%d) ->", i, this->list_ptrs[i], this->frame_table[this->list_ptrs[i]->index]);
		if(this->list_ptrs[i] != NULL)
			printf("%p\n", this->list_ptrs[i]->next);
		else
			puts("null");
	}
#endif
	return tmp->index;
}

static void update(VMM vmm, int page_index)
{
	/*puts(__func__); */
	LRU_VMM this = (LRU_VMM) vmm;
	int frame_index = this->page_table[page_index].frame_number;
	struct Elem *tmp = this->list_ptrs[frame_index];
#ifdef DEBUG
	unsigned i;
	puts("BEFORE");
	printf("this->sorted_list: %p\tthis->tail: %p\n", this->sorted_list, this->tail);
	for(i = 0; i < this->num_frames; i++) {
		printf("%d: %p (%d) ->", i, this->list_ptrs[i], this->frame_table[this->list_ptrs[i]->index]);
		if(this->list_ptrs[i] != NULL)
			printf("%p\n", this->list_ptrs[i]->next);
		else
			puts("null");
	}
#endif
	/* update list head if need be (accessed element is head of list) */
	if(this->sorted_list == tmp) {
		this->sorted_list = tmp->next;
		/* assumes more than 1 frames */
		tmp->next->prev = NULL;
	}
	/* move accessed element at end of list */
	if(this->tail != tmp) {
		/* remove tmp from linked list */
		if(tmp->prev != NULL)
			tmp->prev->next = tmp->next;
		tmp->next->prev = tmp->prev; /* tmp != this->tail => tmp->next != NULL */
		/* add tmp to end of linked list */
		tmp->prev = this->tail;
		this->tail = this->tail->next = tmp;
		tmp->next = NULL;
	}

#ifdef DEBUG
	puts("AFTER");
	printf("this->sorted_list: %p\tthis->tail: %p\n", this->sorted_list, this->tail);
	for(i = 0; i < this->num_frames; i++) {
		printf("%d: %p (%d) ->", i, this->list_ptrs[i], this->frame_table[this->list_ptrs[i]->index]);
		if(this->list_ptrs[i] != NULL)
			printf("%p\n", this->list_ptrs[i]->next);
		else
			puts("null");
	}
#endif
}

