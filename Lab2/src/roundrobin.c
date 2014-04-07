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

#include <roundrobin.h>

static Scheduler new(const Builder bld, int quantum);
static void delete(Scheduler sched);
static Scheduler clone(const Scheduler sched);
static char *to_string(const Scheduler sched);
static unsigned readyq_size(const Scheduler sched);
static void put_event(Scheduler sched, Process proc);
static Process get_event(Scheduler sched);
static int get_quantum(const Scheduler sched);
static void set_quantum(Scheduler sched, int quantum);
static Process peek_readyq(Scheduler sched);

static struct RR_Scheduler_LT RR_lt = {
	NULL,
	false,
	&new,
	&delete,
	&clone,
	&to_string,
	NULL,
	&readyq_size,
	&get_event,
	&put_event,
	&peek_readyq,
	NULL,
	NULL,
	&get_quantum,
	&set_quantum
};

const struct Builder __RR_Scheduler__ = {
	&__Scheduler__,
	(Lookup_Table) &RR_lt,
	sizeof(struct RR_Scheduler),
	"RR"
};


static Scheduler new(const Builder bld, int quantum)
{
	RR_Scheduler sched = __RR_Scheduler__.super->lt->new(bld);

#define new(builder, ...)	((__##builder##__).lt->new((const Builder)\
			&__##builder##__, ##__VA_ARGS__))
	if(bld == &__RR_Scheduler__)
		bld->lt->lt_initialized = true;
	sched->ready_queue = new(FIFO);
	sched->quantum = quantum;

	return (Scheduler) sched;
}

static void delete(Scheduler sched)
{
	RR_Scheduler this = (RR_Scheduler) sched;

	this->ready_queue->lt->delete(this->ready_queue);
	__RR_Scheduler__.super->lt->delete(sched);
}

static Scheduler clone(const Scheduler sched)
{
	RR_Scheduler this = (RR_Scheduler) sched,
		     s = __RR_Scheduler__.super->lt->clone(sched);

	s->ready_queue = this->ready_queue->lt->clone(this->ready_queue);

	return (Scheduler) s;
}

static char *to_string(const Scheduler sched)
{
	RR_Scheduler this = (RR_Scheduler) sched;
	char *str = NULL;

	if(asprintf(&str, "%s %d", this->lt->bld->name, this->quantum)
		== -1) {
		perror("Error allocating memory ");
		exit(EXIT_FAILURE);
	} else
		return str;
}


static unsigned readyq_size(const Scheduler sched)
{
	RR_Scheduler this = (RR_Scheduler) sched;

	return this->ready_queue->lt->size(this->ready_queue);
}


static Process get_event(Scheduler sched)
{
	RR_Scheduler this = (RR_Scheduler) sched;

	return (Process) this->ready_queue->lt->get(this->ready_queue);
}

static void put_event(Scheduler sched, Process proc)
{
	RR_Scheduler this = (RR_Scheduler) sched;

	this->ready_queue->lt->put(this->ready_queue, (Object) proc);
}

static Process peek_readyq(Scheduler sched)
{
	RR_Scheduler this = (RR_Scheduler) sched;

	return (Process) this->ready_queue->lt->peek(this->ready_queue);
}

static int get_quantum(const Scheduler this)
{
	return ((RR_Scheduler) this)->quantum;
}

static void set_quantum(Scheduler this, int quantum)
{
	((RR_Scheduler) this)->quantum = quantum;
}
