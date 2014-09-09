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
 *  along with Lab2-Scheduler.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <scheduler.h>

static Scheduler new(const Builder);
static void delete(Scheduler);
static Scheduler clone(const Scheduler);
static char *to_string(const Scheduler);
static void run(Scheduler, FILE*, bool);

static struct Scheduler_LT lt = {
	NULL,
	false,
	&new,
	&delete,
	&clone,
	&to_string,
	NULL,
	NULL,
	NULL,
	&run,
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

	/* we underride functions BEFORE calling the super constructor, as this
	 * allows each object to inherit the function most specific to itself */
	if( ! bld->lt->lt_initialized) {
		if(bld->lt->new == NULL)
			bld->lt->new = (void *(*)(const Builder, ...)) &new;
		if(bld->lt->delete == NULL)
			bld->lt->delete = (void (*)(void*)) &delete;
		if(bld->lt->clone == NULL)
			bld->lt->clone = (void *(*)(const void*)) &clone;
		if(bld->lt->to_string == NULL)
			bld->lt->to_string = (char *(*)(const void*)) &to_string;
		if(((Scheduler_LT) bld->lt)->run == NULL)
			((Scheduler_LT) bld->lt)->run = &run;
	}
	this = __Scheduler__.super->lt->new(bld);

#define new(builder, ...)	((__##builder##__).lt->new((const Builder)\
			&__##builder##__, ##__VA_ARGS__))
	if(bld == &__Scheduler__)
		bld->lt->lt_initialized = true;
	this->event_queue = new(SortedList, 1);
	this->last_event = this->last_io_event = this->size = this->maxpid =
	this->cpu_use = this->io_use = this->total_turnaround =
	this->total_cpu_waiting = 0;
	this->oldproc = NULL;
	this->process_running = false;

	return this;
}

static void delete(Scheduler this)
{
	this->event_queue->lt->delete(this->event_queue);
	/* call super destructor to finish the job */
	__Scheduler__.super->lt->delete(this);
}

static Scheduler clone(const Scheduler this)
{
	Scheduler s = __Scheduler__.super->lt->clone(this);

	s->event_queue = this->event_queue->lt->clone(this->event_queue);

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


Process read_process(FILE *stream, pid_t pid)
{
	Process p = NULL;
	unsigned AT, TC, CB, IO;

	if(fscanf(stream, "%u %u %u %u", &AT, &TC, &CB, &IO) == 4)
		p = new(Process, pid, AT, TC, CB, IO);

	return p;
}

bool add_arriving_procs(Scheduler this, FILE *input)
{
	Process p = this->oldproc;

	if((this->oldproc != NULL && this->oldproc->AT <=
			(unsigned long) this->last_event) || this->event_queue->size == 0) {
		if(this->oldproc != NULL) {
			printf("current time = %ld\n", this->last_event);
			this->event_queue->lt->insert(this->event_queue,
					(Comparable) this->oldproc);
		}
		while((p = read_process(input, this->maxpid++)) != NULL &&
			(p->AT <= (unsigned long) this->last_event || this->event_queue->size == 0)) {
			printf("current time = %ld\n", this->last_event);
			this->event_queue->lt->insert(this->event_queue, (Comparable) p);
		}
		this->oldproc = p;
	}
	return p != NULL;
}

void print_final_stats(Scheduler this)
{
	double cpu_use_percentage = 100.0 * this->cpu_use / (double) this->last_event, 
	       io_use_percentage = 100.0 * this->io_use / (double) this->last_event,
	       avg_turnaround = this->total_turnaround / (double) this->maxpid,
	       avg_cpu_waiting = this->total_cpu_waiting / (double) this->maxpid,
	       throughput = 100.0 * this->maxpid / (double) this->last_event;

	printf("SUM: %ld %.2lf %.2lf %.2lf %.2lf %.3lf\n", (long int) this->last_event,
			cpu_use_percentage, io_use_percentage, avg_turnaround,
			avg_cpu_waiting, throughput);
}

void run_process(Scheduler this)
{
	Process p = this->lt->get_event(this);
#ifdef DEBUG
	State state;
#endif

	if(p != NULL) {
#ifdef DEBUG
		state = p->state;
#endif
		p->CW += (unsigned long) this->last_event - p->FT;
		p->runtime = myrandom(p->CB);
		if(p->runtime > p->preempt_after) {
			p->runtime = p->preempt_after;
			p->preempted = true;
		}
		if(p->runtime + p->cpu_time > p->TC)
			p->runtime = p->TC - p->cpu_time;
	//	printf("cb=%u\n", p->runtime);
		p->cpu_time += p->runtime;
		p->FT += p->runtime;
		p->state = RUNNING;
		this->event_queue->lt->insert(this->event_queue, (Comparable) p);
		this->process_running = true;
#ifdef DEBUG
	fprintf(stderr, "==> %ld %d ts=%u %s  dur=%ld\n", this->last_event, p->pid, p->FT, 
			p->state == INIT ||
			p->state == READY ? "READY" :
			p->state == RUNNING ? "RUNNG" :
			p->state == BLOCKED ? "BLOCK" :
			p->state == PREEMPTED ? "PREEMPTED" : "INVALID",
			this->last_event - p->FT);
	/*
	fprintf(stderr, "T(%d:%ld): %s -> %s", p->pid, this->last_event,
			state == INIT ||
			state == READY ? "READY" :
			state == RUNNING ? "RUNNG" :
			state == BLOCKED ? "BLOCK" :
			state == PREEMPTED ? "PREEMPTED" : "INVALID",
			p->state == INIT ||
			p->state == READY ? "READY" :
			p->state == RUNNING ? "RUNNG" :
			p->state == BLOCKED ? "BLOCK" :
			p->state == PREEMPTED ? "PREEMPTED" : "INVALID");
	if(p->state == BLOCKED)
		fprintf(stderr, " ib=%d rem=%d\n", p->runtime, p->TC - p->cpu_time);
	else if(p->state == RUNNING)
		fprintf(stderr, " cb=%d rem=%d\n", p->runtime, p->TC - p->cpu_time);
	else
		putc('\n', stderr);
	putc('\n', stderr); */
#endif
	}
}

void transition_process(Scheduler this, Process p)
{
	char *proc_stats;
#ifdef DEBUG
	State state = p->state;
#endif

	switch(p->state) {
		case BLOCKED:
			p->IT += p->runtime;
			this->io_use += this->last_event - this->last_io_event;
		case PREEMPTED:
		case INIT:
			p->state = READY;
			this->lt->put_event(this, p);
			break;
		case RUNNING:
			this->process_running = false;
			if(p->cpu_time == p->TC) {
				this->total_cpu_waiting += p->CW;
				this->cpu_use += p->TC;
				this->total_turnaround += p->FT - p->AT;
				proc_stats = p->lt->to_string(p);
				puts(proc_stats);
				free(proc_stats);
				p->lt->delete(p);
			} else {
				if( ! p->preempted) {
					if(p->cpu_time == p->TC)
						this->total_turnaround += p->FT - p->AT;
					p->state = BLOCKED;
					p->runtime = myrandom(p->IO);
					//printf("ib=%u\n", p->runtime);
					p->FT += p->runtime;
					this->last_io_event = this->last_event;
				} else {
					p->state = PREEMPTED;
					p->preempted = false;
				}
				this->event_queue->lt->insert(this->event_queue, (Comparable) p);
			}
			break;
		default:
			fputs("ERROR: process with invalid state found in event queue\n", stderr);
			proc_stats = p->lt->to_string(p);
			puts(proc_stats);
			free(proc_stats);
			exit(EXIT_FAILURE);
	}
#ifdef DEBUG
	fprintf(stderr, "==> %ld %d ts=%u %s  dur=%ld\n", this->last_event, p->pid, p->FT, 
			p->state == INIT || 
			p->state == READY ? "READY" :
			p->state == RUNNING ? "RUNNG" :
			p->state == BLOCKED ? "BLOCK" :
			p->state == PREEMPTED ? "PREEMPTED" : "INVALID",
			this->last_event - p->FT);
	/*
	   fprintf(stderr, "T(%d:%ld): %s -> %s", p->pid, this->last_event,
	   state == INIT ||
	   state == READY ? "READY" :
	   state == RUNNING ? "RUNNG" :
	   state == BLOCKED ? "BLOCK" :
	   state == PREEMPTED ? "PREEMPTED" : "INVALID",
	   p->state == INIT || 
	   p->state == READY ? "READY" :
	   p->state == RUNNING ? "RUNNG" :
	   p->state == BLOCKED ? "BLOCK" :
	   p->state == PREEMPTED ? "PREEMPTED" : "INVALID");
	   if(p->state == BLOCKED)
	   fprintf(stderr, " ib=%d rem=%d\n", p->runtime, p->TC - p->cpu_time);
	   else if(p->state == RUNNING)
	   fprintf(stderr, " cb=%d rem=%d\n", p->runtime, p->TC - p->cpu_time);
	   else
	   putc('\n', stderr);
	   putc('\n', stderr); */
#endif
}

static void run(Scheduler this, FILE *stream, bool trace)
{
	Process p = NULL;
	bool reading_input = true;
	(void) trace;

	reading_input = add_arriving_procs(this, stream);
	do {
		p = (Process) this->event_queue->lt->peek(this->event_queue);
		if(p != NULL) {
			this->last_event = p->FT;
			while((p = (Process) this->event_queue->lt->peek(this->event_queue)) != NULL
					&& p->FT == (long unsigned) this->last_event) {
				transition_process(this,
						(Process) this->event_queue->lt->get(this->event_queue));
			}
		}
		if( ! this->process_running)
			run_process(this);
		if(reading_input)
			reading_input = add_arriving_procs(this, stream);
	} while(this->event_queue->size != 0);

	print_final_stats(this);
}

