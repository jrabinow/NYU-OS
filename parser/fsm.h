#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
//#include <ctype.h>
//#include "utils.h"

#include "parser.h"

typedef struct {
	size_t linenum, lineoffset, size;
	char *token;
} Token;

Token read_token(File *f);
bool is_valid_token(Token *t);
void delete_token(Token *t);
void check_token(Token *t, File *f, char *expected_data, char *err_msg);

/* MUST BE A POWER OF TWO OR needs_more_mem() MACRO WILL NOT WORK */
#define __TOKEN_MEM_SIZE__	16

#define needs_more_mem(tok)	((tok)->size + 1 >= __TOKEN_MEM_SIZE && ((tok)->size & ((tok)->size + 1)) == 0)
#define add_mem(tok)		(tok)->token = (char*) xrealloc((tok)->token, ((tok)->size + 1) << 1)

#endif
