#ifndef LINKER
#define LINKER

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "utils.h"

#define ADDR_MEM_SIZE 512
#define MAX_USELIST_SIZE 16
#define MAX_DEFLIST_SIZE 16

int linenum, lineoffset, global_offset;

typedef enum {
	NUM_EXPECTED,
	SYM_EXPECTED,
	ADDR_EXPECTED,
	SYM_TOLONG,
	TO_MANY_DEF_IN_MODULE,
	TO_MANY_USE_IN_MODULE,
	TO_MANY_SYMBOLS,
	TO_MANY_INSTR
} Error_Codes;

void __parseerror(int errcode, int linenum, int lineoffset);

#include "token.h"
#include "symbol.h"
#include "instruction.h"
#include "module.h"

void print_immediate(int addr, int val);
void print_absolute(int addr, int val);

#define opcode(i)	(((i) / 1000) * 1000)

#endif
