#include "instruction.h"

Instruction *read_instruction(FILE *stream)
{
	Token *t = NULL;
	Instruction *i = (Instruction*) xmalloc(sizeof(Instruction));
	
	t = read_token(stream);
	if( ! is_valid_addressing(t)) {
		__parseerror(ADDR_EXPECTED, t->linenum, t->lineoffset);
		exit(EXIT_FAILURE);
	}
	switch(t->token[0]) {
		case 'I':
			i->addr = I;
			break;
		case 'A':
			i->addr = A;
			break;
		case 'R':
			i->addr = R;
			break;
		case 'E':
			i->addr = E;
	}
	delete_token(t);
	
	t = read_token(stream);
	if( ! is_valid_int(t)) {
		__parseerror(NUM_EXPECTED, t->linenum, t->lineoffset);
		exit(EXIT_FAILURE);
	}
	i->instruction = atoi(t->token);
	delete_token(t);
	return i;
}

bool is_valid_addressing(Token *t)
{
	return is_valid_token(t) && t->size == 1 && (t->token[0] == 'I' || t->token[0] == 'A' ||
			t->token[0] == 'R' || t->token[0] == 'E');
}

