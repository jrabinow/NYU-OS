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

#include <NRU_VMM.h>

static VMM new(const Builder bld, int num_frames);
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

const struct Builder __NRU_VMM__ = {
	&__VMM__,
	(Lookup_Table) &lt,
	sizeof(struct NRU_VMM),
	"NRU_VMM"
};

static VMM new(const Builder bld, int num_frames)
{
	NRU_VMM this = __NRU_VMM__.super->lt->new(bld, num_frames);

	if(bld == &__NRU_VMM__)
		bld->lt->lt_initialized = true;
	this->replacement_request = this->used_frames = 0;

	return (VMM) this;
}

static void delete(VMM vmm)
{
	NRU_VMM this = (NRU_VMM) vmm;

	__NRU_VMM__.super->lt->delete(this);
}

static int get_frame_index(VMM vmm)
{
	NRU_VMM this = (NRU_VMM) vmm;
	unsigned i;
	int cpt[4] = {0}, class;
	int classes[4][64];

	if(this->used_frames >= this->num_frames) {
		this->replacement_request++;
		if(this->replacement_request % REFERENCE_RESET_INTERVAL == 0) {
			for(i = 0; i < NUM_VIRT_PAGES; i++)
				if(this->page_table[i].present) {
					class = (this->page_table[i].referenced
					<< 1) | this->page_table[i].modified;
					classes[class][cpt[class]++] = i;
					this->page_table[i].referenced = false;
				}
		} else
			for(i = 0; i < NUM_VIRT_PAGES; i++)
				if(this->page_table[i].present) {
					class = (this->page_table[i].referenced
					<< 1) | this->page_table[i].modified;
					classes[class][cpt[class]++] = i;
				}
		for(i = 0; i < 4; i++) {
			if(cpt[i] > 0)
				return this->page_table[classes[i]
					[myrandom(cpt[i])]].frame_number;
		}
	}
	return this->used_frames++;
}
