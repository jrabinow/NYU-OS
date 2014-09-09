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

#ifndef VMM_H
#define VMM_H

/* standard header files */
#include <stdint.h>	/* uint64_t, uint_fast16_t, intptr_t */
#include <inttypes.h>	/* macro for portable printf_ing of uint64_t */
#include <limits.h>	/* INT_MIN */

/* header files that are specific to this project (passed to gcc with -I option) */
#include <utils.h>
#include <object.h>

#define NUM_VIRT_PAGES 64

typedef enum {
	OUTPUT			=  1,
	PAGE_TABLE		=  2,
	FRAME_TABLE		=  4,
	SUMMARY			=  8,
	PAGE_TABLE_RUNNING	= 16,
	FRAME_TABLE_RUNNING	= 32
} Verbose_Flag;

typedef enum { READ = 0, WRITE = 1 } Operation;

#define UNMAP_COST	 400
#define MAP_COST	 400
#define PAGEIN_COST	3000
#define PAGEOUT_COST	3000
#define ZEROS_COST	 150

typedef struct {
	uint32_t frame_number:28;	/* can have at most 2^28 virtual pages */
	uint32_t modified:1;
	uint32_t pagedout:1;
	uint32_t present:1;
	uint32_t referenced:1;
} PTE;

struct VMM {
	struct VMM_LT *lt;
	unsigned num_frames;
	int *frame_table;
	PTE page_table[NUM_VIRT_PAGES];
	unsigned instr_count, unmaps, maps, pageins, pageouts, zeros;
};

/* empty frames have value EMPTY */
#define EMPTY	INT_MIN

typedef struct VMM* VMM;

struct VMM_LT {
	Builder bld;
	bool lt_initialized;
	VMM (*new)(const Builder, int);
	void (*delete)(VMM);
	VMM (*clone)(const VMM);
	char *(*to_string)(const VMM);
	void (*run)(VMM, FILE*, Verbose_Flag);
	int (*get_frame_index)(VMM);
	void (*update)(VMM, int);
};

typedef struct VMM_LT* VMM_LT;

extern const struct Builder __VMM__, __NRU_VMM__, __LRU_VMM__, __Random_VMM__, __FIFO_VMM__,
	__SecChance_VMM__, __PClock_VMM__, __VClock_VMM__, __PAging_VMM__, __VAging_VMM__;

typedef struct {
	uint_fast16_t valid:1;
	uint_fast16_t operation_type:1;
	uint_fast16_t virtual_page:14;
} Instruction;

Instruction read_instruction(FILE *f);

void map(VMM this, int frame_index, int page_index, Verbose_Flag flags);
void unmap(VMM this, int frame_index, Verbose_Flag flags);
void page_out(VMM this, int frame_index, Verbose_Flag flags);
void page_in(VMM this, int frame_index, int page_index, Verbose_Flag flags);
void zeros(VMM this, int frame_index, Verbose_Flag flags);

void print_final_stats(VMM this, Verbose_Flag flags);

#endif
