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

#ifndef LRU_VMM_H
#define LRU_VMM_H

#include <vmm.h>

struct Elem {
	int index;
	struct Elem *next, *prev;
};

struct LRU_VMM {
	VMM_LT lt;
	unsigned num_frames;
	int *frame_table;
	PTE page_table[NUM_VIRT_PAGES];
	unsigned instr_count, unmaps, maps, pageins, pageouts, zeros;
	struct Elem **list_ptrs;
	struct Elem *sorted_list, *tail;
};

typedef struct LRU_VMM* LRU_VMM;

#endif
