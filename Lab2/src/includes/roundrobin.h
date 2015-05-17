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

#ifndef RR_H
#define RR_H

#include <scheduler.h>
#include <FIFO.h>
#include <heap.h>

struct RR_Scheduler_LT {
	/* ----------- OBJECT ----------- */
	const Builder bld;
	bool lt_initialized;
	Scheduler (*new)(const Builder bld, int quantum);
	void (*delete)(Scheduler);
	Scheduler (*clone)(const Scheduler);
	char *(*to_string)(const Scheduler);
	/* ---------- SCHEDULER ---------- */
	void (*read_input)(Scheduler, FILE*);
	unsigned (*get_readyqueue_size)(const Scheduler);
	Process (*get_event)(Scheduler);
	void (*put_event)(Scheduler, Process);
	Process (*peek_readyq)(const Scheduler);
	void (*run)(Scheduler, bool);
	void (*print_info)(Scheduler);
	/* --------- RR_SCHEDULER --------- */
	int (*get_quantum)(const Scheduler);
	void (*set_quantum)(Scheduler, int);
};

typedef struct RR_Scheduler_LT* RR_Scheduler_LT;

struct RR_Scheduler {
	RR_Scheduler_LT lt;
	pid_t maxpid;
	Process oldproc;
	time_t last_event;
	time_t last_io_event;
	bool process_running;
	unsigned size, cpu_use, io_use, turnaround, cpu_waiting, throughput;
	SortedList event_queue;
	FIFO ready_queue;
	int quantum;
};

typedef struct RR_Scheduler* RR_Scheduler;

#endif
