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

#include <PAging_VMM.h>

static VMM new(const Builder, int);
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

const struct Builder __PAging_VMM__ = {
	&__VMM__,
	(Lookup_Table) &lt,
	sizeof(struct PAging_VMM),
	"PAging_VMM"
};

static VMM new(const Builder bld, int num_frames)
{
	PAging_VMM this = __PAging_VMM__.super->lt->new(bld, num_frames);

	if(bld == &__PAging_VMM__)
		bld->lt->lt_initialized = true;

	this->age_vector = (uint32_t*) xmalloc(this->num_frames *
					sizeof(uint32_t));
	memset(this->age_vector, 0, sizeof(uint32_t) * this->num_frames);
	this->used_frames = 0;

	return (VMM) this;
}

static void delete(VMM vmm)
{
	PAging_VMM this = (PAging_VMM) vmm;

	free(this->age_vector);
	__PAging_VMM__.super->lt->delete(this);
}

static VMM clone(VMM vmm)
{
	PAging_VMM this = (PAging_VMM) vmm,
		   pvmm = __PAging_VMM__.super->lt->clone(this);
	
	pvmm->age_vector = (uint32_t*) xmalloc(this->num_frames *
					sizeof(uint32_t));
	memcpy(pvmm->age_vector, this->age_vector, this->num_frames * 
			sizeof(uint32_t));
	return (VMM) pvmm;
}

static int get_frame_index(VMM vmm)
{
	PAging_VMM this = (PAging_VMM) vmm;
	uint32_t min = UINT_MAX;
	unsigned i, ret = 0;
	int index;

	if(this->used_frames >= this->num_frames) {
		for(i = 0; i < this->num_frames; i++) {
			index = this->frame_table[i];
			this->age_vector[i] >>= 1;
			if(this->page_table[index].referenced) {
				this->age_vector[i] |= 1LU << 31;
				this->page_table[index].referenced = false;
			}
			if(this->age_vector[i] < min) {
				ret = i;
				min = this->age_vector[i];
			}
		}
		this->age_vector[ret] = 0;
		return ret;
	}

	return this->used_frames++;
}
