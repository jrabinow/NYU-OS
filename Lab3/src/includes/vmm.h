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

#include <utils.h>
#include <random.h>
#include <object.h>
#include <token.h>

#define NUM_VIRT_PAGES 64

typedef enum { O = 1, P = 2, F = 4, S = 8 } Verbose_Flag;

typedef struct {
	unsigned index:28;
	unsigned modified:1;
	unsigned pagedout:1;
	unsigned present:1;
	unsigned referenced:1;
} PTE;

struct VMM_LT {
	Builder bld;
	bool lt_initialized;
	struct VMM *(*new)(const Builder, int);
	void (*delete)(struct VMM*);
	struct VMM *(*clone)(const struct VMM*);
	char *(*to_string)(const struct VMM*);
	void (*run)(struct VMM*, FILE*, Verbose_Flag);
	void (*put)(struct VMM*, PTE);
	PTE (*get)(struct VMM*);
};

typedef struct VMM_LT* VMM_LT;

struct VMM {
	VMM_LT lt;
	unsigned num_frames;
	
	long long unmaps, maps, pageins, pageouts, zeros, totalcost;
};

typedef struct VMM* VMM;

extern const struct Builder __VMM__, __NRU_VMM__, __LRU_VMM__, __Random_VMM__, __FIFO_VMM__,
	__SecChance_VMM__, __PClock_VMM__, __VClock_VMM__, __PAging_VMM__, __VAging_VMM__;

typedef struct {
	unsigned char valid:1;
	unsigned char operation_type:1;
	unsigned char virtual_page:6;
} Instruction;

Instruction read_instruction(FILE *f);

#endif
