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

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <utils.h>
#include <object.h>
#include <process.h>
#include <random.h>
#include <sortedlist.h>

struct Scheduler {
	struct Scheduler_LT *lt;
	pid_t maxpid;
	Process oldproc;
	time_t last_event;
	time_t last_io_event;
	bool process_running;
	unsigned size, cpu_use, io_use, total_turnaround, total_cpu_waiting;
	SortedList event_queue;
};

typedef struct Scheduler* Scheduler;

struct Scheduler_LT {
	/* ----------- OBJECT ----------- */
	const Builder bld;
	bool lt_initialized;
	Scheduler (*new)(const Builder);
	void (*delete)(Scheduler);
	Scheduler (*clone)(const Scheduler);
	char *(*to_string)(const Scheduler);
	/* ---------- SCHEDULER ---------- */
	unsigned (*get_readyqueue_size)(Scheduler);
	Process (*get_event)(Scheduler);
	void (*put_event)(Scheduler, Process);
	Process (*peek_readyq)(const Scheduler);
	void (*run)(Scheduler, FILE*, bool);
	void (*print_info)(Scheduler);
};

typedef struct Scheduler_LT* Scheduler_LT;

extern const struct Builder __Scheduler__, __FCFS_Scheduler__,
       __LCFS_Scheduler__, __SJF_Scheduler__, __RR_Scheduler__;

#endif
