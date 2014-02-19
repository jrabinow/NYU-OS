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
