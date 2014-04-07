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
static void put(VMM, PTE);
static PTE get(VMM);

static struct VMM_LT lt = {
	NULL,
	false,
	&new,
	&delete,
	NULL,
	NULL,
	NULL,
	&put,
	&get
};

const struct Builder __FIFO_VMM__ = {
	&__VMM__,
	(Lookup_Table) &lt,
	sizeof(struct FIFO_VMM),
	"FIFO_VMM"
};

static VMM new(const Builder bld, int num_frames)
{
	FIFO_VMM this = __FIFO_VMM__.super->lt->new(bld, num_frames);
	
	if(bld == &__FIFO_VMM__)
		bld->lt->lt_initialized = true;
#define new(builder, ...)	((__##builder##__).lt->new((const Builder)\
			&__##builder##__, ##__VA_ARGS__))
	this->frames = new(FIFO);

	return (VMM) this;
}

static void delete(VMM vmm)
{
	FIFO_VMM this = (FIFO_VMM) vmm;
	
	this->frames->lt->delete(this->frames);

	__FIFO_VMM__.super->lt->delete(this);
}

static void put(VMM vmm, PTE pte)
{
	FIFO_VMM this = (FIFO_VMM) vmm;
	/*this->frames->lt->put(this->frames, (void*) pte);*/
}

static PTE get(VMM vmm)
{
	FIFO_VMM this = (FIFO_VMM) vmm;
	PTE tmp;

	return tmp; /*this->frames->lt->get(this->frames); */
}
