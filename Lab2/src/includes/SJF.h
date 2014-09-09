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

#ifndef SJF_H
#define SJF_H

#include <scheduler.h>
#include <FIFO.h>
#include <heap.h>

struct SJF_Scheduler {
	Scheduler_LT lt;
	pid_t maxpid;
	Process oldproc;
	time_t last_event;
	time_t last_io_event;
	bool process_running;
	unsigned size, cpu_use, io_use, turnaround, cpu_waiting, throughput;
	SortedList event_queue;
	Heap ready_queue;
};

typedef struct SJF_Scheduler* SJF_Scheduler;

#endif
