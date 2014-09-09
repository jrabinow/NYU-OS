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

#include <LCFS.h>

static Scheduler new(const Builder bld);
static void delete(Scheduler sched);
static Scheduler clone(const Scheduler sched);
static unsigned readyq_size(const Scheduler sched);
static void put_event(Scheduler sched, Process proc);
static Process get_event(Scheduler sched);

static struct Scheduler_LT LCFS_lt = {
	NULL,
	false,
	&new,
	&delete,
	&clone,
	NULL,
	&readyq_size,
	&get_event,
	&put_event,
	NULL
};

const struct Builder __LCFS_Scheduler__ = {
	&__Scheduler__,
	(Lookup_Table) &LCFS_lt,
	sizeof(struct LCFS_Scheduler),
	"LCFS"
};

static Scheduler new(const Builder bld)
{
	LCFS_Scheduler sched = __LCFS_Scheduler__.super->lt->new(bld);

#define new(builder, ...)	((__##builder##__).lt->new((const Builder)\
			&__##builder##__, ##__VA_ARGS__))
	if(bld == &__LCFS_Scheduler__)
		bld->lt->lt_initialized = true;
	sched->ready_queue = new(Stack);
	return (Scheduler) sched;
}

static void delete(Scheduler sched)
{
	LCFS_Scheduler this = (LCFS_Scheduler) sched;

	this->ready_queue->lt->delete(this->ready_queue);
	__LCFS_Scheduler__.super->lt->delete(this);
}

static Scheduler clone(const Scheduler sched)
{
	LCFS_Scheduler s = __LCFS_Scheduler__.super->lt->clone(sched),
		       this = (LCFS_Scheduler) sched;

	s->ready_queue = this->ready_queue->lt->clone(this->ready_queue);
	return (Scheduler) s;
}

static unsigned readyq_size(const Scheduler sched)
{
	LCFS_Scheduler this = (LCFS_Scheduler) sched;

	return this->ready_queue->lt->size(this->ready_queue);
}

static void put_event(Scheduler sched, Process proc)
{
	LCFS_Scheduler this = (LCFS_Scheduler) sched;
	this->ready_queue->lt->push(this->ready_queue, (Object) proc);
	this->size++;
}

static Process get_event(Scheduler sched)
{
	LCFS_Scheduler this = (LCFS_Scheduler) sched;

	if(this->size > 0)
		this->size--;
	return (Process) this->ready_queue->lt->pop(this->ready_queue);
}

