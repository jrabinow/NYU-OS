/*
 *  Copyright (C) 2014 Julien Rabinow <jnr305@nyu.edu>
 *
 *  This file is part of Lab2-Scheduler.
 *
 *  Lab2-Scheduler is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Lab2-Scheduler is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Lab2-Scheduler. If not, see <http://www.gnu.org/licenses/>.
 */

#include <scheduler.h>

static Scheduler new(const Builder bld);
static void delete(Scheduler sched);
static Scheduler clone(const Scheduler sched);
static char *to_string(const Scheduler sched);
static void read_input(Scheduler, FILE*);
static void run(Scheduler sched, bool trace);
static void print_info(Scheduler this);

static struct Scheduler_LT lt = {
	NULL,
	false,
	&new,
	&delete,
	&clone,
	&to_string,
	&read_input,
	NULL,
	NULL,
	NULL,
	&run,
	&print_info
};

const struct Builder __Scheduler__ = {
	&__Object__,
	(Lookup_Table) &lt,
	sizeof(struct Scheduler),
	"Scheduler"
};

/* See object.c for explanation on builders, destructors and the like */
static Scheduler new(const Builder bld)
{
	Scheduler this;
	Scheduler_LT lt = (Scheduler_LT) bld->lt;

	/* we underride functions BEFORE calling the super constructor, as this
	 * allows each object to inherit the function most specific to itself */
	if( ! lt->lt_initialized) {
		if(lt->new == NULL)
			lt->new = &new;
		if(lt->delete == NULL)
			lt->delete = &delete;
		if(lt->clone == NULL)
			lt->clone = &clone;
		if(lt->to_string == NULL)
			lt->to_string = &to_string;
		if(lt->read_input == NULL)
			lt->read_input = &read_input;
		if(lt->run == NULL)
			lt->run = &run;
		if(lt->print_info == NULL)
			lt->print_info = &print_info;
	}
	this = __Scheduler__.super->lt->new(bld);

#define new(builder, ...)	((__##builder##__).lt->new((const Builder)\
			&__##builder##__, ##__VA_ARGS__))
	if(bld == &__Scheduler__)
		bld->lt->lt_initialized = true;
	this->input_queue = new(FIFO);
	this->IO_blocking = new(FIFO);
	this->last_event = 0;
	this->CPU_use = this->IO_use = this->turnaround = this->cpu_waiting =
		this->throughput = 0.0;

	return this;
}

static void delete(Scheduler this)
{
	this->input_queue->lt->delete(this->input_queue);
	this->IO_blocking->lt->delete(this->IO_blocking);
	/* call super destructor to finish the job */
	__Scheduler__.super->lt->delete(this);
}

static Scheduler clone(const Scheduler this)
{
	Scheduler s = __Scheduler__.super->lt->clone(this);

	s->input_queue = this->input_queue->lt->clone(this->input_queue);
	s->IO_blocking = this->IO_blocking->lt->clone(this->IO_blocking);

	/* copying individual primitive values is taken care of
	 * in super's clone method */
	return s;
}

static char *to_string(const Scheduler this)
{
	char *str = NULL;

	if(asprintf(&str, "%s", this->lt->bld->name) == -1) {
		perror("Error allocating memory ");
		exit(EXIT_FAILURE);
	} else
		return str;
}


static void read_input(Scheduler this, FILE *stream)
{
	Process p;
	pid_t pid = 0;

	int AT, TC, CB, IO, count;
	while((count = fscanf(stream, "%d %d %d %d", &AT, &TC, &CB, &IO)) == 4) {
		p = new(Process, pid++, AT, TC, CB, IO);
		this->input_queue->lt->put(this->input_queue, (Object) p);
		empty_buffer(stream);	/* remove trailing newlines from file */
	}

/*	char *debug_str = NULL;
	while(this->input_queue->lt->size(this->input_queue) > 0) {
		p = (Process) this->input_queue->lt->get(this->input_queue);
		debug_str = p->lt->to_string(p);
		puts(debug_str);
		free(debug_str);
		p->lt->delete(p);
	}
	*/
}
#include <heap.h>
static void run(Scheduler this, bool trace)
{
	Process p = NULL;
	int runtime, quantum;

	Heap h = new(Heap);
	while(this->input_queue->lt->size(this->input_queue) != 0) {
		h->lt->put(h, this->input_queue->lt->get(this->input_queue));
	}

	char *debug_str;
	while(h->lt->size(h) != 0) {
		p = h->lt->get(h);
		debug_str = p->lt->to_string(p);
		puts(debug_str);
		free(debug_str);
		p->lt->delete(p);
	}
	h->lt->delete(h);

/*
	if(instance_of(this, RR_Scheduler)) {
		quantum = this->get_quantum(this);
		while((p = this->get_event(this)) != NULL) {
			runtime = myrandom(p->CB);
		}
	} else {
		puts("lol");
	}
	*/
}

static void select_process(Scheduler this)
{

}

static void print_info(Scheduler this)
{
	printf("SUM: %ld %.2lf %.2lf %.2lf %.2lf %.3lf\n", this->last_event,
		this->CPU_use, this->IO_use, this->turnaround, this->cpu_waiting,
		this->throughput);
}
