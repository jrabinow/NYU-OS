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

#include <process.h>

static Process new(const Builder bld, pid_t pid, unsigned arrival_time,
		unsigned cpu_time, unsigned cpu_burst, unsigned io_burst);
static char *to_string(const Process p);
static int cmp_total_cpu(const Process p1, const Process p2);
static int cmp_finish_evt_time(const Process p1, const Process p2);
/*static int compare(const Process, const Process);*/

static struct Process_LT lt = {
	NULL,
	false,
	&new,
	NULL,
	NULL,
	&to_string,
	{ &cmp_total_cpu,
	  &cmp_finish_evt_time }
	/*&compare */
};

const struct Builder __Process__ = {
	&__Object__,			/* Process implements Comparable, but
					   Comparable is a virtual class
					 * Therefore, Process descends from
					 * Object */
	(Lookup_Table) &lt,
	sizeof(struct Process),
	"Process"
};

static Process new(const Builder bld, pid_t pid, unsigned arrival_time,
		unsigned cpu_time, unsigned cpu_burst, unsigned io_burst)
{
	Process p = __Process__.super->lt->new(bld);

	if(bld == &__Process__)
		bld->lt->lt_initialized = true;
	p->pid = pid;
	p->AT = p->FT = arrival_time;
	p->TC = cpu_time;
	p->CB = cpu_burst;
	p->IO = io_burst;
	p->IT = p->CW = p->cpu_time = 0;
	p->preempt_after = UINT_MAX;
	p->state = INIT;
	p->preempted = false;

	return p;
}

static char *to_string(const Process p)
{
	char *str = NULL;
#ifdef DEBUG
	fprintf(stderr, "State = %s\tArrival time = %d\tFinishing time = %d\n",
			p->state == INIT ? "INIT" :
			p->state == READY ? "READY" :
			p->state == RUNNING ? "RUNNING" :
			p->state == BLOCKED ? "BLOCKED" :
			p->state == PREEMPTED ? "PREEMPTED" :
			"INVALID", p->AT, p->FT); 
#endif
	if(asprintf(&str, "%04u: %4u %4u %4u %4u | %4u %4u %4u %4u", p->pid,
		p->AT, p->TC, p->CB, p->IO, p->FT, p->FT - p->AT, p->IT, p->CW)
		== -1) {
		perror("Error allocating memory ");
		exit(EXIT_FAILURE);
	} else
		return str;
}

static int cmp_total_cpu(const Process p1, const Process p2)
{
	return p2->TC - p1->TC;
}

static int cmp_finish_evt_time(const Process p1, const Process p2)
{
	return p2->FT - p1->FT;
}

/*
static int compare(const Process p1, const Process p2)
{
	if(p1->remaining_CPU_time < p2->remaining_CPU_time)
		return -1;
	else
		return p1->remaining_CPU_time > p2->remaining_CPU_time;
} */
