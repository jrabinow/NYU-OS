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

const struct Builder __SecChance_VMM__ = {
	&__VMM__,
	(Lookup_Table) &lt,
	sizeof(struct SecChance_VMM),
	"SecChance_VMM"
};

static VMM new(const Builder bld, int num_frames)
{
	SecChance_VMM this = __SecChance_VMM__.super->lt->new(bld, num_frames);

	if(bld == &__SecChance_VMM__)
		bld->lt->lt_initialized = true;

	return (VMM) this;
}

static void delete(VMM vmm)
{
	SecChance_VMM this = (SecChance_VMM) vmm;

	__SecChance_VMM__.super->lt->delete(this);
}

static void put(VMM vmm, PTE pte)
{
	SecChance_VMM this = (SecChance_VMM) vmm;
}

static PTE get(VMM vmm)
{
	PTE tmp;
	return tmp;
}
