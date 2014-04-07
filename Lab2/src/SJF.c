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

#include <SJF.h>

static Scheduler new(const Builder bld);
static void delete(Scheduler sched);
static Scheduler clone(const Scheduler sched);
static unsigned readyq_size(const Scheduler sched);
static void put_event(Scheduler sched, Process proc);
static Process get_event(Scheduler sched);
static Process peek_readyq(Scheduler sched);

static struct Scheduler_LT SJF_lt = {
	NULL,
	false,
	&new,
	&delete,
	&clone,
	NULL,
	NULL,
	&readyq_size,
	&get_event,
	&put_event,
	&peek_readyq,
	NULL,
	NULL
};

const struct Builder __SJF_Scheduler__ = {
	&__Scheduler__,
	(Lookup_Table) &SJF_lt,
	sizeof(struct SJF_Scheduler),
	"SJF"
};

static Scheduler new(const Builder bld)
{
	SJF_Scheduler sched = __SJF_Scheduler__.super->lt->new(bld);

#define new(builder, ...)	((__##builder##__).lt->new((const Builder)\
			&__##builder##__, ##__VA_ARGS__))
	if(bld == &__SJF_Scheduler__)
		bld->lt->lt_initialized = true;
	sched->ready_queue = new(Heap);

	return (Scheduler) sched;
}

static void delete(Scheduler sched)
{
	SJF_Scheduler this = (SJF_Scheduler) sched;

	this->ready_queue->lt->delete(this->ready_queue);
	__SJF_Scheduler__.super->lt->delete(sched);
}

static Scheduler clone(const Scheduler sched)
{
	SJF_Scheduler this = (SJF_Scheduler) sched,
		      new_sched = __SJF_Scheduler__.super->lt->clone(sched);

	new_sched->ready_queue = this->ready_queue->lt->clone(this->ready_queue);

	return (Scheduler) new_sched;
}

static unsigned readyq_size(const Scheduler sched)
{
	SJF_Scheduler this = (SJF_Scheduler) sched;

	return this->ready_queue->lt->size(this->ready_queue);
}

static Process get_event(Scheduler sched)
{
	SJF_Scheduler this = (SJF_Scheduler) sched;

	return (Process) this->ready_queue->lt->get(this->ready_queue);
}

static void put_event(Scheduler sched, Process proc)
{
	SJF_Scheduler this = (SJF_Scheduler) sched;

	this->ready_queue->lt->put(this->ready_queue, (Comparable) proc);
}

static Process peek_readyq(Scheduler sched)
{
	SJF_Scheduler this = (SJF_Scheduler) sched;

	return (Process) this->ready_queue->lt->peek(this->ready_queue);
}
