#include "symbol.h"

Symbol *read_symbol(FILE *stream)
{
	Symbol *s = NULL;
	Token *t = read_token(stream);

	if(is_valid_symbol_name(t)) {
		s = (Symbol*) xmalloc(sizeof(Symbol));;
		s->sym = t->token;
		free(t);

		t = read_token(stream);
		if(is_valid_int(t)) {
			s->local_offset = atoi(t->token);
			s->status = SINGLE_DEF;
			delete_token(t);
			return s;
		} else {
			__parseerror(NUM_EXPECTED, t->linenum, t->lineoffset);
			exit(EXIT_FAILURE);
		}
	} else {
		if(is_valid_token(t) && t->size > 16)
			__parseerror(SYM_TOLONG, t->linenum, t->lineoffset);
		else
			__parseerror(SYM_EXPECTED, t->linenum, t->lineoffset);
		exit(EXIT_FAILURE);
	}
}

bool is_valid_symbol_name(Token *t)
{
	int i;

	if( ! is_valid_token(t) || t->size > 16 || ! isalpha(t->token[0]))
		return false;
	for(i = 1; t->token[i] != '\0'; i++)
		if(! isalnum(t->token[i]))
			return false;
	return true;
}

void delete_symbol(Symbol *s)
{
	if(s != NULL) {
		if(s->sym != NULL)
			free(s->sym);
		free(s);
	}
}
