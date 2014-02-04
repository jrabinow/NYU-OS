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

Module *read_module(FILE *stream, int *module_id);
void read_symbol_list(FILE *stream, Module *m, Token *sym_list);
void read_use_list(FILE *stream, Module *m);
void read_program_text(FILE *stream, Module *m);
void delete_module(Module *m);

#endif
