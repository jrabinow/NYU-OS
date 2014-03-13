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

#include "instruction.h"

Instruction *read_instruction(FILE *stream)
{
	Token *t = NULL;
	Instruction *i = (Instruction*) xmalloc(sizeof(Instruction));

	t = read_token(stream);
	if( ! is_valid_addressing(t)) {
		__parseerror(ADDR_EXPECTED, t->linenum, t->lineoffset);
		exit(EXIT_FAILURE);
	}
	switch(t->token[0]) {
		case 'I':
			i->addr = I;
			break;
		case 'A':
			i->addr = A;
			break;
		case 'R':
			i->addr = R;
			break;
		case 'E':
			i->addr = E;
	}
	delete_token(t);

	t = read_token(stream);
	if( ! is_valid_int(t)) {
		__parseerror(NUM_EXPECTED, t->linenum, t->lineoffset);
		exit(EXIT_FAILURE);
	}
	i->instruction = atoi(t->token);
	delete_token(t);
	return i;
}

bool is_valid_addressing(Token *t)
{
	return is_valid_token(t) && t->size == 1 && (t->token[0] == 'I' || t->token[0] == 'A' ||
		t->token[0] == 'R' || t->token[0] == 'E');
}

