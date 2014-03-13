/*
 *  Copyright (C) 2014 Julien Rabinow <jnr305@nyu.edu>
 *
 *  This file is part of Lab1-Linker.
 *
 *  Lab1-Linker is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Lab1-Linker is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Lab1-Linker.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "module.h"

Module *read_module(FILE *stream, int *module_id, Flag ignore)
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

	if(ignore & SYMBOLS) {
		skip_symbol_list(stream, t);
		mod->num_symbols = 0;
	} else
		read_symbol_list(stream, mod, t);
	delete_token(t);

	if(ignore & USELIST) {
		skip_use_list(stream);
		mod->num_use_list = 0;
	} else
		read_use_list(stream, mod);
	if(ignore & TEXT) {
		skip_program_text(stream);
		mod->module_size = 0;
	}
	read_program_text(stream, mod);

	return mod;
}

void read_symbol_list(FILE *stream, Module *m, Token *sym_list_size)
{
	size_t i;
	int num_symbols;

	if( ! is_valid_int(sym_list_size)) {
		__parseerror(NUM_EXPECTED, sym_list_size->linenum, sym_list_size->lineoffset);
		exit(EXIT_FAILURE);
	}
	num_symbols = atoi(sym_list_size->token);

	if(num_symbols < 0 || num_symbols > MAX_DEFLIST_SIZE) {	/* invalid domain */
		__parseerror(TOO_MANY_DEF_IN_MODULE, sym_list_size->linenum, sym_list_size->lineoffset);
		exit(EXIT_FAILURE);
	}
	m->num_symbols = (unsigned short) num_symbols;

	if(m->num_symbols > 0) {
		m->symbols = (Symbol**) xmalloc(m->num_symbols * sizeof(Symbol*));
		for(i = 0; i < m->num_symbols; i++) {
			m->symbols[i] = read_symbol(stream);
			m->symbols[i]->module_id = m->module_id;
		}
	}
}

void skip_symbol_list(FILE *stream, Token *sym_list_size)
{
	int i;
	int num_symbols;

	if( ! is_valid_int(sym_list_size)) {
		__parseerror(NUM_EXPECTED, sym_list_size->linenum, sym_list_size->lineoffset);
		exit(EXIT_FAILURE);
	}
	num_symbols = atoi(sym_list_size->token);

	if(num_symbols < 0 || num_symbols > MAX_DEFLIST_SIZE) {	/* invalid domain */
		__parseerror(TOO_MANY_DEF_IN_MODULE, sym_list_size->linenum, sym_list_size->lineoffset);
		exit(EXIT_FAILURE);
	}

	for(i = 0; i < num_symbols; i++)
		delete_symbol(read_symbol(stream));
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
		__parseerror(TOO_MANY_USE_IN_MODULE, t->linenum, t->lineoffset);
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

void skip_use_list(FILE *stream)
{
	unsigned short i;
	int num_use_list;
	Token *t = read_token(stream);

	if( ! is_valid_int(t)) {
		__parseerror(NUM_EXPECTED, t->linenum, t->lineoffset);
		exit(EXIT_FAILURE);
	}
	num_use_list = atoi(t->token);
	if(num_use_list < 0 || num_use_list > MAX_USELIST_SIZE) {
		__parseerror(TOO_MANY_USE_IN_MODULE, t->linenum, t->lineoffset);
		exit(EXIT_FAILURE);
	}
	delete_token(t);

	if(num_use_list > 0) {
		for(i = 0; i < num_use_list; i++) {
			t = read_token(stream);
			if( ! is_valid_symbol_name(t)) {
				__parseerror(SYM_EXPECTED, t->linenum, t->lineoffset);
				exit(EXIT_FAILURE);
			}
			delete_token(t);
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
		__parseerror(TOO_MANY_INSTR, t->linenum, t->lineoffset);
		exit(EXIT_FAILURE);
	}
	m->module_size = (unsigned short) module_size;
	delete_token(t);

	if(m->module_size > 0) {
		m->instructions = (Instruction**) xmalloc(m->module_size * sizeof(Instruction*));
		for(i = 0; i < m->module_size; i++)
			m->instructions[i] = read_instruction(stream);
	}
}

void skip_program_text(FILE *stream)
{
	int i, module_size;
	Token *t = read_token(stream);

	if( ! is_valid_int(t)) {
		__parseerror(NUM_EXPECTED, t->linenum, t->lineoffset);
		exit(EXIT_FAILURE);
	}
	module_size = atoi(t->token);
	if(module_size < 0 || (global_offset += module_size) > ADDR_MEM_SIZE) {
		__parseerror(TOO_MANY_INSTR, t->linenum, t->lineoffset);
		exit(EXIT_FAILURE);
	}
	delete_token(t);

	for(i = 0; i < module_size; i++)
		delete_instr(read_instruction(stream));
}

void delete_module(Module *m)
{
	size_t i;

	if(m != NULL) {
		if(m->num_symbols != 0)
			free(m->symbols);
		if(m->num_use_list != 0) {
			for(i = 0; i < m->num_use_list; i++)
				if(m->use_list[i].status != UNUSED)
					free(m->use_list[i].sym);
			free(m->use_list);
		}

		if(m->module_size != 0) {
			for(i = 0; i < m->module_size; i++)
				delete_instr(m->instructions[i]);
			free(m->instructions);
		}
		free(m);
	}
}
