/*
 *  Copyright (C) 2014 Julien Rabinow <jnr305@nyu.edu>
 *
 *  This file is part of Lab1-Linker.
 *
 *  Lab1-Linker is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Lab1-Linker is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Lab1-Linker.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LINKER
#define LINKER

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "utils.h"

#define ADDR_MEM_SIZE		512
#define MAX_USELIST_SIZE	16
#define MAX_DEFLIST_SIZE	16

int linenum, lineoffset, global_offset;
const char *current_workfile;

#include "token.h"
#include "symbol.h"
#include "instruction.h"
#include "module.h"

typedef struct {
	__extension__
	union {
		Symbol **symbol;
		Module **module;
		Uselist_Symbol *uselist;
	};
	size_t size, mem_size;
} Array;

void addto_symbol_table(Array *symtable, Module *m);
bool symbol_index(Array *symtable, char *symtoken, ssize_t *index);

void get_unused_uselist(Module *m, Array *unused_uselist);
void print_unused_symbols(Array *symtable, Array *unused_uselist);

void print_instructions(Module *m, Array *symtable);
void print_immediate(int addr, int val);
void print_absolute(int addr, int val);
void print_relative(int addr, int val, Module *m);
void print_external(int addr, int val, Module *m, Array *symtable);

#define opcode(i)	(((i) / 1000) * 1000)

#endif
