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

#ifndef MODULE
#define MODULE

#include "token.h"
#include "symbol.h"
#include "instruction.h"

typedef struct {
	Symbol **symbols;
	Uselist_Symbol *use_list;
	Instruction **instructions;
	off_t base_offset;
	unsigned short module_size, num_symbols, num_use_list, module_id;
} Module;

#include "linker.h"

extern int global_offset;

typedef enum { SYMBOLS = 1, USELIST = 2, TEXT = 4 } Flag;

Module *read_module(FILE *stream, int *module_id, Flag ignore);

void read_symbol_list(FILE *stream, Module *m, Token *sym_list);
void read_use_list(FILE *stream, Module *m);
void read_program_text(FILE *stream, Module *m);

void skip_symbol_list(FILE *stream, Token *sym_list_size);
void skip_use_list(FILE *stream);
void skip_program_text(FILE *stream);

void delete_module(Module *m);

#endif
