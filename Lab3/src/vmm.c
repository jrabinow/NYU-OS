/*
 *  Copyright (C) 2014 Julien Rabinow <jnr305@nyu.edu>
 *
 *  This file is part of Lab3-Mem-manager.
 *
 *  Lab3-Mem-manager is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Lab3-Mem-manager is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Lab3-Mem-manager. If not, see <http://www.gnu.org/licenses/>.
 */

#include <vmm.h>

static VMM new(const Builder bld, int num_frames);
static void run(VMM, FILE*, Verbose_Flag);

/* Thanks to the underwriting mechanism described in object.c, all undefined (NULL)
 * functions are automatically overriden to the values of the "parent" constructor */
static struct VMM_LT lt = {
	NULL,
	false,
	&new,
	NULL,
	NULL,
	NULL,
	&run,
	NULL,
	NULL
};

const struct Builder __VMM__ = {
	&__Object__,
	(Lookup_Table) &lt,
	sizeof(struct VMM),
	"VMM"
};

static VMM new(const Builder bld, int num_frames)
{
	VMM this = NULL;
	VMM_LT lt = (VMM_LT) bld->lt;

	if( ! lt->lt_initialized) {
		if(lt->run == NULL)
			lt->run = &run;
	}
	this = __VMM__.super->lt->new(bld);

	this->num_frames = num_frames;
	if(bld == &__VMM__)
		bld->lt->lt_initialized = true;

	return this;
}

Instruction read_instruction(FILE *f)
{
	Instruction ret = { 0, 0, 0 };
	int tmp;
	Token *t = read_token(f);

	if( ! is_valid_int(t->token))
		return ret;
	switch((tmp = atoi(t->token))) {
		case 0:
		case 1:
			ret.operation_type = tmp;
			break;
		default:
			fprintf(stderr, "Error: invalid value %d at line %lu offset %lu\n", tmp, t->linenum, t->lineoffset);
			exit(EXIT_FAILURE);
	}
	delete_token(t);
	t = read_token(f);
	if( ! is_valid_int(t->token))
		return ret;
	tmp = atoi(t->token);
	delete_token(t);
	if(tmp < 0 || NUM_VIRT_PAGES < tmp) {
		fputs("WHAT THE HELL!!!\n", stderr);
		fprintf(stderr, "Error: invalid value %d at line %lu offset %lu\n", tmp, t->linenum, t->lineoffset);
		exit(EXIT_FAILURE);
	} else {
		ret.virtual_page = tmp;
		ret.valid = true;
	}
	return ret;
}

static void run(VMM vmm, FILE *input, Verbose_Flag flags)
{
	Instruction instruction;

	while((instruction = read_instruction(input)).valid) {
		printf("%d %d %d\n", instruction.valid, instruction.operation_type, instruction.virtual_page);
	}
}
