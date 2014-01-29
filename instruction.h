#ifndef INSTR
#define INSTR

#include "token.h"

typedef enum {
	I,	// Immediate
	A,	// Absolute
	R,	// Relative
	E	// External
} Addressing;

typedef struct {
	Addressing addr;
	int instruction;
} Instruction;

Instruction *read_instruction(FILE *stream);
bool is_valid_instr(Token *t);
bool is_valid_addressing(Token *t);
#define delete_instr 	free

#include "linker.h"

#endif
