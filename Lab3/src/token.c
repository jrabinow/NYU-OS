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

#include <token.h>

static int linenum = 1, lineoffset = 1;

/* read token into allocated buffer. No limit on token size
 * Bizarre code behaviour for EOF: getchar() will always read a newline character
 * right before newline. A lot of code here is just to make sure lineoffset and linenum
 * are updated properly anyways */
Token *read_token(FILE *stream)
{
	int c, i = 0, mem_size = 16;
	Token *t = (Token*) xmalloc(sizeof(Token));

	t->token = (char*) xmalloc(mem_size);

	// swallow whitespace and comments
	do {
		while((c = fgetc(stream)) == ' ' || c == '\t' || c == '\n')
			switch(c) {
				case ' ':
				case '\t':
					lineoffset++;
					break;
				case '\n':
					if((c = fgetc(stream)) != EOF) {
						linenum++;
						lineoffset = 1;
						ungetc(c, stream);
					}
			}
		if(c == '#') {
			empty_buffer(stream);	// empties line completely
			lineoffset = 1;
			linenum++;
		}
	} while(c == '#');


	t->linenum = linenum;
	t->lineoffset = lineoffset;

	if(c == EOF) {
		free(t->token);
		t->token = NULL;
		t->size = 0;
		return t;
	}

	t->linenum = linenum;
	t->lineoffset = lineoffset;
	do {
		t->token[i++] = c;
		if(i == mem_size)
			t->token = (char*) xrealloc(t->token, mem_size <<= 1);
	} while((c = fgetc(stream)) != EOF && c != ' ' && c != '\t' && c != '\n');

	ungetc(c, stream);
	lineoffset += i;
	t->token[i] = '\0';
	t->size = i;
	t->token = (char*) realloc(t->token, i+1);	// adjust allocated memory to correct size
	return t;
}

bool is_valid_token(Token *t)
{
	return t != NULL && t->token != NULL;
}

void delete_token(Token *t)
{
	if(t != NULL) {
		if(t->token != NULL)
			free(t->token);
		free(t);
	}
}

