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

#include <SecChance_VMM.h>

static VMM new(const Builder, int);
static void delete(VMM);
static int get_frame_index(VMM);

static struct VMM_LT lt = {
	NULL,
	false,
	&new,
	&delete,
	NULL,
	NULL,
	NULL,
	&get_frame_index,
	NULL
};

const struct Builder __SecChance_VMM__ = {
	&__VMM__,
	(Lookup_Table) &lt,
	sizeof(struct SecChance_VMM),
	"SecChance_VMM"
};

static VMM new(const Builder bld, int num_frames)
{
	intptr_t index;
	SecChance_VMM this = __SecChance_VMM__.super->lt->new(bld, num_frames);

	if(bld == &__SecChance_VMM__)
		bld->lt->lt_initialized = true;

#define new(builder, ...)	((__##builder##__).lt->new((const Builder)\
		&__##builder##__, ##__VA_ARGS__))
	this->frames = new(FIFO);
	for(index = 1; index <= this->num_frames; index++)
		this->frames->lt->put(this->frames, (void*) index);

	return (VMM) this;
}

static void delete(VMM vmm)
{
	SecChance_VMM this = (SecChance_VMM) vmm;

	/* we are storing values as opposed to objects in the FIFO. If we call
	 * delete immediately on the FIFO, the destructor will try to call delete
	 * on every single of the values. Which will lead to a segfault.
	 * So before calling delete on the FIFO, we simply empty it of all the 
	 * values it contains */
	while(this->frames->lt->get(this->frames));
	this->frames->lt->delete(this->frames);

	__SecChance_VMM__.super->lt->delete(this);
}

static int get_frame_index(VMM vmm)
{
	void *index;
	int idx;
	SecChance_VMM this = (SecChance_VMM) vmm;

	do {
		index = this->frames->lt->get(this->frames);
		idx = (int) (intptr_t) index - 1;
		this->frames->lt->put(this->frames, index);
		if(this->frame_table[idx] == EMPTY ||
				! this->page_table[this->frame_table[idx]].referenced)
			break;
		else
			this->page_table[this->frame_table[idx]].referenced = false;
	} while(index != 0);

	/* we added 1 to the index value in fifo->put. We decrement by 1 here */
	return idx;
}
