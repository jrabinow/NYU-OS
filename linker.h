#ifndef LINKER
#define LINKER

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "utils.h"

#define ADDR_MEM_SIZE		512
#define MAX_USELIST_SIZE	16
#define MAX_DEFLIST_SIZE	16

int linenum, lineoffset, global_offset;

#include "token.h"
#include "symbol.h"
#include "instruction.h"
#include "module.h"

typedef struct {
	union {
		Symbol **symbol;
		Module **module;
		Uselist_Symbol *uselist;
	};
	size_t size, mem_size;
} Array;

void print_immediate(int addr, int val);
void print_absolute(int addr, int val);
int symbol_index(Array *symtable, char *symtoken);
void addto_symbol_table(Array *symtable, Module *m);
void print_instructions(Module *m, Array *symtable);
void print_relative(int addr, int val, Module *m);
void print_external(int addr, int val, Module *m, Array *symtable);
void get_unused_uselist(Module *m, Array *unused_uselist);
void print_unused_symbols(Array *symtable, Array *unused_uselist);

#define opcode(i)	(((i) / 1000) * 1000)

#endif
