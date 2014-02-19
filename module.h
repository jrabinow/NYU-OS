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
