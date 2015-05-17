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

#ifndef TOKEN
#define TOKEN

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

typedef struct {
	size_t linenum, lineoffset, size;
	char *token;
} Token;

extern int linenum, lineoffset;

Token *read_token(FILE *stream);
bool is_valid_int(Token *t);
bool is_valid_token(Token *t);
void delete_token(Token *t);

#endif
