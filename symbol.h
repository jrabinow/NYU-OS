#ifndef SYMBOL
#define SYMBOL

#include <stdio.h>
#include "token.h"

typedef enum { UNUSED, USED, MULTIPLE_DEFS, TOO_LARGE } Status;

typedef struct {
	char *sym;
	off_t offset, local_offset;
	short module_id;
	Status status;
} Symbol;

typedef struct {
	char *sym;
	Status status;
	int module_id;
} Uselist_Symbol;

Symbol *read_symbol(FILE *stream);
bool is_valid_symbol_name(Token *t);
void delete_symbol(Symbol *s);

#include "linker.h"

#endif
