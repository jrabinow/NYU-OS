#include "module.h"

Module *read_module(FILE *stream, int *module_id)
{
	Token *t = NULL;
	Module *mod = (Module*) xmalloc(sizeof(Module));

	/* Test if additional token to read. If not, we have read the entire file
	 * -> return NULL
	 * If there is another token to be read, there must be an entire module to be read
	 * if there is an invalid token further on, it is a syntax error and we can 
	 * "throw" an error at that point */
	t = read_token(stream);
	if( ! is_valid_token(t)) {
		delete_token(t);
		free(mod);
		return NULL;
	}

	mod->module_id = ++(*module_id);
	mod->symbols = (Symbol**) (mod->use_list = (Uselist_Symbol*) (mod->instructions = (Instruction**) NULL));
	mod->base_offset = global_offset;

	read_symbol_list(stream, mod, t);
	delete_token(t);
	read_use_list(stream, mod);
	read_program_text(stream, mod);

	return mod;
}

void read_symbol_list(FILE *stream, Module *m, Token *sym_list_size)
{
	size_t i;
	int num_symbols;
	Token *t = NULL;

	if( ! is_valid_int(sym_list_size)) {
		__parseerror(NUM_EXPECTED, sym_list_size->linenum, sym_list_size->lineoffset);
		exit(EXIT_FAILURE);
	}
	num_symbols = atoi(sym_list_size->token);

	if(num_symbols < 0 || num_symbols > MAX_DEFLIST_SIZE) {	// invalid domain
		__parseerror(TO_MANY_DEF_IN_MODULE, sym_list_size->linenum, sym_list_size->lineoffset);
		exit(EXIT_FAILURE);
	}
	m->num_symbols = (unsigned short) num_symbols;

	delete_token(t);
	if(m->num_symbols > 0) {
		m->symbols = (Symbol**) xmalloc(m->num_symbols * sizeof(Symbol*));
		for(i = 0; i < m->num_symbols; i++)
			m->symbols[i] = read_symbol(stream);
	}
}

void read_use_list(FILE *stream, Module *m)
{
	size_t i;
	int num_use_list;
	Token *t = read_token(stream);

	if( ! is_valid_int(t)) {
		__parseerror(NUM_EXPECTED, t->linenum, t->lineoffset);
		exit(EXIT_FAILURE);
	}
	num_use_list = atoi(t->token);
	if(num_use_list < 0 || num_use_list > MAX_USELIST_SIZE) {
		__parseerror(TO_MANY_USE_IN_MODULE, t->linenum, t->lineoffset);
		exit(EXIT_FAILURE);
	}
	m->num_use_list = (unsigned short) num_use_list;
	delete_token(t);

	if(m->num_use_list > 0) {
		m->use_list = (Uselist_Symbol*) xmalloc(m->num_use_list * sizeof(Uselist_Symbol));
		for(i = 0; i < m->num_use_list; i++) {
			t = read_token(stream);
			if(is_valid_symbol_name(t)) {
				m->use_list[i].sym = t->token;
				m->use_list[i].status = UNUSED;
				free(t);
			} else {
				__parseerror(SYM_EXPECTED, t->linenum, t->lineoffset);
				exit(EXIT_FAILURE);
			}
		}
	}
}

void read_program_text(FILE *stream, Module *m)
{
	size_t i;
	int module_size;
	Token *t = read_token(stream);

	if( ! is_valid_int(t)) {
		__parseerror(NUM_EXPECTED, t->linenum, t->lineoffset);
		exit(EXIT_FAILURE);
	}
	module_size = atoi(t->token);
	if(module_size < 0 || (global_offset += module_size) > ADDR_MEM_SIZE) {
		__parseerror(TO_MANY_INSTR, t->linenum, t->lineoffset);
		exit(EXIT_FAILURE);
	}
	m->module_size = (unsigned short) module_size;
	delete_token(t);
	
	if(m->module_size > 0) {
		m->instructions = (Instruction**) xmalloc(m->module_size * sizeof(Instruction*));
		for(i = 0; i < m->module_size; i++) {
			m->instructions[i] = read_instruction(stream);
		}
	}
}

void delete_module(Module *m)
{
	size_t i;

	if(m != NULL) {
		if(m->num_symbols > 0) {
			for(i = 0; i < m->num_symbols; i++)
				delete_symbol(m->symbols[i]);
			free(m->symbols);
		}
		if(m->num_use_list > 0) {
			for(i = 0; i < m->num_use_list; i++)
				free(m->use_list[i].sym);
			free(m->use_list);
		}
		if(m->module_size > 0) {
			for(i = 0; i < m->module_size; i++)
				delete_instr(m->instructions[i]);
			free(m->instructions);
		}
		free(m);
	}
}
