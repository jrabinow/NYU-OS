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

#include <VAging_VMM.h>

static VMM new(const Builder, int);
static int get_frame_index(VMM);
static char *to_string(VMM);

static struct VMM_LT lt = {
	NULL,
	false,
	&new,
	NULL,
	NULL,
	&to_string,
	NULL,
	&get_frame_index,
	NULL
};

const struct Builder __VAging_VMM__ = {
	&__VMM__,
	(Lookup_Table) &lt,
	sizeof(struct VAging_VMM),
	"VAging_VMM"
};

static VMM new(const Builder bld, int num_frames)
{
	VAging_VMM this = __VAging_VMM__.super->lt->new(bld, num_frames);

	if(bld == &__VAging_VMM__)
		bld->lt->lt_initialized = true;
	
	memset(this->age_vector, 0, sizeof(uint32_t) * NUM_VIRT_PAGES);
	this->used_frames = 0;

	return (VMM) this;
}

static char *to_string(const VMM vmm)
{
	unsigned i;
	VAging_VMM this = (VAging_VMM) vmm;

	for(i = 0; i < this->num_frames; i++)
		switch(this->frame_table[i]) {
			case EMPTY:
				fwrite("* ", 2, sizeof(char), stdout);
				break;
			default:
				printf("%d ", this->frame_table[i]);
		}
	printf(" || ");
	for(i = 0; i < NUM_VIRT_PAGES; i++)
		if(this->page_table[i].present)
			printf("%d:%x ", i, this->age_vector[i]);
		else
			printf("* ");
	putchar('\n');
	return NULL;
}

static int get_frame_index(VMM vmm)
{
	VAging_VMM this = (VAging_VMM) vmm;
	uint32_t min = UINT_MAX;
	unsigned i, ret = 0;

	if(this->used_frames >= this->num_frames) {
		for(i = 0; i < NUM_VIRT_PAGES; i++) {
			if(this->page_table[i].present) {
				this->age_vector[i] >>= 1;
				if(this->page_table[i].referenced) {
					this->age_vector[i] |= 1LU << 31;
					this->page_table[i].referenced = false;
				}
				if(this->age_vector[i] < min) {
					ret = i;
					min = this->age_vector[i];
				}
			}
		}
		this->age_vector[ret] = 0;
		return this->page_table[ret].frame_number;
	}

	return this->used_frames++;
}
