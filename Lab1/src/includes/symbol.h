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

#ifndef SYMBOL
#define SYMBOL

#include <stdio.h>
#include "token.h"

typedef enum { UNUSED, USED, SINGLE_DEF, MULTIPLE_DEFS } Status;

typedef struct {
	char *sym;
	off_t offset, local_offset;
	short module_id;
	Status status;
} Symbol;

typedef struct {
	char *sym;
	Status status;
	short module_id;
} Uselist_Symbol;

Symbol *read_symbol(FILE *stream);
bool is_valid_symbol_name(Token *t);
void delete_symbol(Symbol *s);

#endif
