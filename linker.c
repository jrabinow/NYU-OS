#include "linker.h"

int main(int argc, char **argv)
{
	bool conserve_mem = true;	/* keep all modules in memory by default */
	unsigned i;
	int module_id = 0;
	FILE *input = NULL;
	Module *mod = NULL;
	Array modules, symtable, unused_uselist;

	if(argc < 2) {
		usage(argv[0]);
		return 0;
	}

	/* ----- INIT ----- */
	input = xfopen(argv[1], "r");

	linenum = lineoffset = 1;
	global_offset = 0;

	symtable.size = 0;
	symtable.mem_size = 64;
	symtable.symbol = (Symbol**) xmalloc(64 * sizeof(Symbol*));

	if( ! conserve_mem) {
		modules.size = 0;
		modules.mem_size = 64;
		modules.module = (Module**) xmalloc(64 * sizeof(Module*));
	}

	/* ----- 1st PASS ----- */
	while((mod = read_module(input, &module_id, conserve_mem * USELIST)) != NULL) {
		if( ! conserve_mem) {
			modules.module[modules.size++] = mod;
			if(modules.size == modules.mem_size)
				modules.module = (Module**) xrealloc(modules.module, (modules.mem_size <<= 1) * sizeof(Module*));
		}
		addto_symbol_table(&symtable, mod);
		if(conserve_mem)
			delete_module(mod);
	}

	puts("Symbol Table");
	for(i = 0; i < symtable.size; i++) {
		printf("%s=%lu", symtable.symbol[i]->sym, (unsigned long) symtable.symbol[i]->offset);
		if(symtable.symbol[i]->status == MULTIPLE_DEFS)
			puts(" Error: This variable is multiple times defined; first value used");
		else /* symtable.symbol[i]->status == SINGLE_DEF */
			putchar('\n');
		symtable.symbol[i]->status = UNUSED;
	}
	putchar('\n');
	fclose(input);

	/* ----- 2nd PASS ----- */
	puts("Memory Map");
	unused_uselist.size = 0;
	unused_uselist.mem_size = 8;
	unused_uselist.uselist = (Uselist_Symbol*) xmalloc(8 * sizeof(Uselist_Symbol));

	if(conserve_mem) {
		linenum = lineoffset = 1;
		global_offset = 0;
		module_id = 0;
		input = xfopen(argv[1], "r");
		while((mod = read_module(input, &module_id, 0)) != NULL) {
			print_instructions(mod, &symtable);
			get_unused_uselist(mod, &unused_uselist);
			for(i = 0; i < mod->num_symbols; i++)
				delete_symbol(mod->symbols[i]);
			delete_module(mod);
		}
		fclose(input);
	} else {
		for(i = 0; i < modules.size; i++) {
			print_instructions(modules.module[i], &symtable);
			get_unused_uselist(modules.module[i], &unused_uselist);
			delete_module(modules.module[i]);
		}
		free(modules.module);
	}
	putchar('\n');

	print_unused_symbols(&symtable, &unused_uselist);

	/* ----- CLEANUP ----- */
	for(i = 0; i < symtable.size; i++)
		delete_symbol(symtable.symbol[i]);
	free(symtable.symbol);
	free(unused_uselist.uselist);

	return 0;
}

//void pass2_conserve_mem(FILE *input, pass2

void print_instructions(Module *m, Array *symtable)
{
	size_t i;
	Instruction *instr = NULL;

	for(i = 0; i < m->module_size; i++) {
		instr = m->instructions[i];
		switch(instr->addr) {
			case I:
				print_immediate(m->base_offset + i, instr->instruction);
				break;
			case A:
				print_absolute(m->base_offset + i, instr->instruction);
				break;
			case R:
				print_relative(m->base_offset + i, instr->instruction, m);
				break;
			case E:
				print_external(m->base_offset + i, instr->instruction, m, symtable);
		}
		putchar('\n');
	}
}

void print_immediate(int addr, int val)
{
	if(val >= 10000)
		printf("%03d: 9999 Error: Illegal immediate value; treated as 9999", addr);
	else
		printf("%03d: %04d", addr, val);
}

void print_absolute(int addr, int val)
{
	if(val >= 10000)
		printf("%03d: 9999 Error: Illegal opcode; treated as 9999", addr);
	else if(val % 1000 > ADDR_MEM_SIZE)
		printf("%03d: %04d Error: Absolute address exceeds machine size; zero used", addr, opcode(val));
	else
		printf("%03d: %04d", addr, val);
}

void print_relative(int addr, int val, Module *m)
{
	if(val >= 10000)
		printf("%03d: 9999 Error: Illegal opcode; treated as 9999", addr);
	else if(val % 1000 > m->module_size)
		printf("%03d: %04lu Error: Relative address exceeds module size; zero used", addr, (unsigned long) (opcode(val) + m->base_offset));
	else
		printf("%03d: %04lu", addr, (unsigned long) (val + m->base_offset));
}

void print_external(int addr, int val, Module *m, Array *symtable)
{
	ssize_t index;

	if(val >= 10000)
		printf("%03d: 9999 Error: Illegal opcode; treated as 9999", addr);
	else if(val % 1000 >= m->num_use_list) {
		print_immediate(addr, val);
		printf(" Error: External address exceeds length of uselist; treated as immediate");
	} else {
		m->use_list[val % 1000].status = USED;
		if( ! symbol_index(symtable, m->use_list[val % 1000].sym, &index))
			printf("%03d: %04d Error: %s is not defined; zero used", addr,
					opcode(val), m->use_list[val % 1000].sym);
		else {
			printf("%03d: %04lu", addr, (unsigned long) (opcode(val) + symtable->symbol[index]->offset));
			symtable->symbol[index]->status = USED;
		}
	}
}

void addto_symbol_table(Array *symtable, Module *m)
{
	size_t i;
	ssize_t index = 0;

	while(symtable->size + m->num_symbols > symtable->mem_size)
		symtable->symbol = xrealloc(symtable->symbol, (symtable->mem_size <<= 1) * sizeof(Symbol));

	for(i = 0; i < m->num_symbols; i++) {
		if( ! symbol_index(symtable, m->symbols[i]->sym, &index)) {
			if(m->symbols[i]->local_offset >= m->module_size) {
				printf("Warning: Module %u: %s to big %lu (max=%lu) assume zero relative\n", m->module_id,
						m->symbols[i]->sym, (unsigned long) m->symbols[i]->local_offset, (unsigned long) (m->module_size - 1));
				m->symbols[i]->local_offset = 0;
			}
			memmove(&symtable->symbol[index], &symtable->symbol[index + 1], (symtable->size - index) * sizeof(Symbol*));
			symtable->symbol[index] = m->symbols[i];
			symtable->symbol[index]->offset = m->symbols[i]->local_offset + m->base_offset;
			symtable->size++;
		} else {
			symtable->symbol[index]->status = MULTIPLE_DEFS;
			delete_symbol(m->symbols[i]);
		}
	}
}

void get_unused_uselist(Module *m, Array *unused_uselist)
{
	size_t i;

	for(i = 0; i < m->num_use_list; i++)
		if(m->use_list[i].status == UNUSED) {
			if(unused_uselist->size == unused_uselist->mem_size)
				unused_uselist->uselist = (Uselist_Symbol*) xrealloc(unused_uselist->uselist,
						(unused_uselist->mem_size <<= 1) * sizeof(Uselist_Symbol));
			unused_uselist->uselist[unused_uselist->size] = m->use_list[i];
			unused_uselist->uselist[unused_uselist->size++].module_id = m->module_id;
		}
}

void print_unused_symbols(Array *symtable, Array *unused_uselist)
{
	size_t i;

	for(i = 0; i < symtable->size; i++)
		if(symtable->symbol[i]->status == UNUSED)
			printf("Warning: Module %u: %s was defined but never used\n",
				symtable->symbol[i]->module_id,	symtable->symbol[i]->sym);

	for(i = 0; i < unused_uselist->size; i++) {
		printf("Warning: Module %u: %s appeared in the uselist but was not actually used\n",
			unused_uselist->uselist[i].module_id, unused_uselist->uselist[i].sym);
		free(unused_uselist->uselist[i].sym);
	}
}

bool symbol_index(Array *symtable, char *symtoken, ssize_t *index)
{
	ssize_t start = 0, end = symtable->size - 1;
	int ret = 0;

	while(start <= end) {
		*index = (start + end) / 2;
		ret = strcmp(symtoken, symtable->symbol[*index]->sym);
		if(ret > 0)
			start = *index + 1;
		else if(ret < 0)
			end = *index - 1;
		else
			return true;
	}

	if(ret != 0) {
		if(ret < 0)
			*index -= 1;
		else
			*index += 1;
	}

	return false;
}
