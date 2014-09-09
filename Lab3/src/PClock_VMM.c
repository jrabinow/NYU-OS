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

#include <PClock_VMM.h>

static VMM new(const Builder, int);
static int get_frame_index(VMM);

static struct VMM_LT lt = {
	NULL,
	false,
	&new,
	NULL,
	NULL,
	NULL,
	NULL,
	&get_frame_index,
	NULL
};

const struct Builder __PClock_VMM__ = {
	&__VMM__,
	(Lookup_Table) &lt,
	sizeof(struct PClock_VMM),
	"PClock_VMM"
};

static VMM new(const Builder bld, int num_frames)
{
	PClock_VMM this = __PClock_VMM__.super->lt->new(bld, num_frames);

	if(bld == &__PClock_VMM__)
		bld->lt->lt_initialized = true;

	this->hand = 0;

	return (VMM) this;
}

static int get_frame_index(VMM vmm)
{
	PClock_VMM this = (PClock_VMM) vmm;
	int index;

	if(this->frame_table[this->hand] == EMPTY) {
		index = this->hand;
		this->hand = (this->hand + 1) % this->num_frames;
		return index;
	}

	while(this->page_table[this->frame_table[this->hand]].referenced) {
		this->page_table[this->frame_table[this->hand]].referenced = false;
		this->hand = (this->hand + 1) % this->num_frames;
	}
	index = this->hand;
	this->hand = (this->hand + 1) % this->num_frames;

	return index;
}
