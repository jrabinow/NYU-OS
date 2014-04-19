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

#include <FIFO_VMM.h>

static VMM new(const Builder bld, int num_frames);
static void delete(VMM);
static VMM clone(const VMM);
static int get_frame_index(VMM);

static struct VMM_LT lt = {
	NULL,
	false,
	&new,
	&delete,
	&clone,
	NULL,
	NULL,
	&get_frame_index,
	NULL
};

const struct Builder __FIFO_VMM__ = {
	&__VMM__,
	(Lookup_Table) &lt,
	sizeof(struct FIFO_VMM),
	"FIFO_VMM"
};

static VMM new(const Builder bld, int num_frames)
{
	intptr_t index;
	FIFO_VMM this = __FIFO_VMM__.super->lt->new(bld, num_frames);
	
	if(bld == &__FIFO_VMM__)
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
	FIFO_VMM this = (FIFO_VMM) vmm;

	/* we are storing values as opposed to objects in the FIFO. If we call
	 * delete immediately on the FIFO, the destructor will try to call delete
	 * on every single of the values. Which will lead to a segfault.
	 * So before calling delete on the FIFO, we simply empty it of all the 
	 * values it contains */
	while(this->frames->lt->get(this->frames));
	this->frames->lt->delete(this->frames);

	__FIFO_VMM__.super->lt->delete(this);
}

static VMM clone(const VMM vmm)
{
	void *data;
	FIFO_VMM this = (FIFO_VMM) vmm,
		 new_fifo_vmm = __FIFO_VMM__.super->lt->clone(this);
	/* same thing as for delete(). Since the FIFO contains values, we must
	 * clone it manually */
	FIFO tmp = new(FIFO);

	while((data = this->frames->lt->get(this->frames)))
		tmp->lt->put(tmp, data);
	while((data = tmp->lt->get(this->frames))) {
		this->frames->lt->put(this->frames, data);
		new_fifo_vmm->frames->lt->put(new_fifo_vmm->frames, data);
	}
	tmp->lt->delete(tmp);

	return (VMM) new_fifo_vmm;
}

static int get_frame_index(VMM vmm)
{
	FIFO_VMM this = (FIFO_VMM) vmm;
	void *index = this->frames->lt->get(this->frames);
	this->frames->lt->put(this->frames, index);

	/* we added 1 to the index value in fifo->put. We decrement by 1 here */
	return ((int) (intptr_t) index) - 1;
}
