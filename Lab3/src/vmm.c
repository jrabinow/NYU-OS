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

static VMM new(const Builder, int);
static void delete(VMM);
static VMM clone(VMM);
static void run(VMM, FILE*, Verbose_Flag);
static void update(VMM, int);

/* Thanks to the underwriting mechanism described in object.c, all undefined
 * (NULL) functions are automatically overriden to the values of the parent
 * constructor */
static struct VMM_LT lt = {
	NULL,
	false,
	&new,
	&delete,
	&clone,
	NULL,
	&run,
	NULL,
	&update
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
	int init_value = EMPTY;

	if( ! lt->lt_initialized) {
		if(lt->clone == NULL)
			lt->clone = &clone;
		if(lt->run == NULL)
			lt->run = &run;
		if(lt->delete == NULL)
			lt->delete = &delete;
		if(lt->update == NULL)
			lt->update = &update;
	}
	this = __VMM__.super->lt->new(bld);
	if(bld == &__VMM__)
		bld->lt->lt_initialized = true;

#define new(builder, ...)	((__##builder##__).lt->new((const Builder)\
		&__##builder##__, ##__VA_ARGS__))

	this->num_frames = num_frames;
	this->frame_table = (int*) xmalloc(num_frames * sizeof(int));
	initialize_vector(this->frame_table, &init_value, sizeof(int),
			num_frames);
	memset(this->page_table, 0, NUM_VIRT_PAGES * sizeof(PTE));
	this->free_frames = new(FIFO);
	for(init_value = 0; init_value < (int) this->num_frames; init_value++)
		this->free_frames->lt->put(this->free_frames,
				(void*)(intptr_t) init_value);
	this->instr_count = this->unmaps = this->maps = this->pageins =
	this->pageouts = this->zeros = 0;

	return this;
}

static void delete(VMM this)
{
	free(this->frame_table);

	this->free_frames->lt->delete(this->free_frames);

	__VMM__.super->lt->delete(this);
}

static VMM clone(VMM this)
{
	VMM new_vmm = __VMM__.super->lt->clone(this);
	new_vmm->free_frames = this->free_frames->lt->clone(this->free_frames);

	return new_vmm;
}

Instruction read_instruction(FILE *stream)
{
	Instruction ret = { 0, 0, 0 };
	int val1, val2;

	swallow_whitespace(stream);	/* will skip whitespace AND COMMENT LINES */
	if(fscanf(stream, "%d %d", &val1, &val2) == 2) {
		ret.valid = true;
		switch(val1) {
			case READ:
			case WRITE:
				ret.operation_type = val1;
				break;
			default:
				fprintf(stderr, "Error: invalid operation (neither read nor write) %d\n", val1);
				exit(EXIT_FAILURE);
		}
		if(val2 < 0 || val2 >= NUM_VIRT_PAGES) {
			fprintf(stderr, "Error: invalid virtual page number %d\n", val2);
			exit(EXIT_FAILURE);
		}
		ret.virtual_page = val2;
	}
	return ret;
}

static void run(VMM this, FILE *input, Verbose_Flag flags)
{
	Instruction instr;
	int index;

	while((instr = read_instruction(input)).valid) {
		if( ! this->page_table[instr.virtual_page].present) {
			index = this->free_frames->size != 0 ?
				(int) (intptr_t) this->free_frames->lt->get(this->free_frames) :
				this->lt->get_frame_index(this);

			if(flags & OUTPUT)
				printf("==> inst: %d %d\n", instr.operation_type,
						instr.virtual_page);
			if(this->frame_table[index] != EMPTY) {
				unmap(this, index, flags);
				if(this->page_table[this->frame_table[index]].modified)
					page_out(this, index, flags);
			}
			if(this->page_table[instr.virtual_page].pagedout)
				page_in(this, index, instr.virtual_page, flags);
			else
				zeros(this, index, flags);
			map(this, index, instr.virtual_page, flags);
		} else {
			if(flags & OUTPUT)
				printf("==> inst: %d %d\n", instr.operation_type,
						instr.virtual_page);
			this->lt->update(this, instr.virtual_page);
		}
		if(instr.operation_type == WRITE)
			this->page_table[instr.virtual_page].modified = true;

		this->page_table[instr.virtual_page].referenced = true;
		this->instr_count++;
#ifdef DEBUG
		int i;

		if(flags & PAGE_TABLE_RUNNING) {
			for(i = 0; i < NUM_VIRT_PAGES; i++) {
				switch(this->page_table[i].present) {
					case 0:
						printf("%c ", this->page_table[i].pagedout ? '#': '*');
						break;
					case 1:
						printf("%d:%c%c%c ", i,
								(this->page_table[i].referenced ? 'R' : '-'),
								(this->page_table[i].modified ? 'M' : '-'),
								(this->page_table[i].pagedout ? 'S' : '-'));
				}
			}
			putchar('\n');
		}
		if(flags & FRAME_TABLE_RUNNING) {
			for(i = 0; i < (int) this->num_frames; i++)
				switch(this->frame_table[i]) {
					case EMPTY:
						fwrite("* ", 2, sizeof(char), stdout);
						break;
					default:
						printf("%d ", this->frame_table[i]);
				}
			putchar('\n');
		}

		free(this->lt->to_string(this));
#endif
	}
	print_final_stats(this, flags);
}

void map(VMM this, int frame_index, int page_index, Verbose_Flag flags)
{
	if(flags & OUTPUT)
		printf("%u: MAP    %2d  %2d\n",
				this->instr_count, page_index, frame_index);
	this->page_table[page_index].present = true;
	this->page_table[page_index].referenced = true;
	this->page_table[page_index].frame_number = frame_index;
	this->frame_table[frame_index] = page_index;
	this->maps++;
}

void unmap(VMM this, int frame_index, Verbose_Flag flags)
{
	int old_page_index = this->frame_table[frame_index];

	if(flags & OUTPUT)
		printf("%u: UNMAP  %2d  %2d\n",
				this->instr_count, old_page_index, frame_index);
	this->page_table[old_page_index].present = false;
	this->page_table[old_page_index].referenced = false;
	/* Technically, we should set this.frame_table[frame_index] to EMPTY.
	 * However, the only reason we are unmapping this page is to map in
	 * another page. So the EMPTY value will be overwritten very soon */
//	this->free_frames->lt->put(this->free_frames, (Object) (intptr_t) frame_index);
	this->unmaps++;
}

void page_out(VMM this, int frame_index, Verbose_Flag flags)
{
	int old_page_index = this->frame_table[frame_index];

	if(flags & OUTPUT)
		printf("%u: OUT    %2d  %2d\n",
				this->instr_count, old_page_index, frame_index);
	this->page_table[old_page_index].modified = false;
	this->page_table[old_page_index].referenced = false;
	this->page_table[old_page_index].pagedout = true;
	this->pageouts++;
}

void page_in(VMM this, int frame_index, int page_index, Verbose_Flag flags)
{
	if(flags & OUTPUT)
		printf("%u: IN     %2d  %2d\n",
				this->instr_count, page_index, frame_index);
	this->page_table[page_index].modified = false;
	this->pageins++;
}

void zeros(VMM this, int frame_index, Verbose_Flag flags)
{
	if(flags & OUTPUT)
		printf("%u: ZERO       %2d\n", this->instr_count, frame_index);
	this->zeros++;
}

static void update(VMM this, int page_index)
{
	(void) this;
	(void) page_index;
}

void print_final_stats(VMM this, Verbose_Flag flags)
{
	unsigned i;
	uint64_t totalcost;

	if(flags & PAGE_TABLE) {
		for(i = 0; i < NUM_VIRT_PAGES; i++) {
			switch(this->page_table[i].present) {
				case 0:
					printf("%c ", this->page_table[i].pagedout ? '#': '*');
					break;
				case 1:
					printf("%d:%c%c%c ", i,
							(this->page_table[i].referenced ? 'R' : '-'),
							(this->page_table[i].modified ? 'M' : '-'),
							(this->page_table[i].pagedout ? 'S' : '-'));
			}
		}
		putchar('\n');
	}
	if(flags & FRAME_TABLE) {
		for(i = 0; i < this->num_frames; i++)
			switch(this->frame_table[i]) {
				case EMPTY:
					fwrite("* ", 2, sizeof(char), stdout);
					break;
				default:
					printf("%d ", this->frame_table[i]);
			}
		putchar('\n');
	}
	if(flags & SUMMARY) {
		totalcost = this->instr_count + this->unmaps * UNMAP_COST +
			this->maps * MAP_COST + this->pageins * PAGEIN_COST +
			this->pageouts * PAGEOUT_COST + this->zeros * ZEROS_COST;
		printf("SUM %d U=%d M=%d I=%d O=%d Z=%d ===> %" PRIu64 "\n",
				this->instr_count, this->unmaps, this->maps, this->pageins,
				this->pageouts, this->zeros, totalcost);
	}
}

