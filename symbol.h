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
