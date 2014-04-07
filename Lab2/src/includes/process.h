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

#ifndef PROCESS_H
#define PROCESS_H

#include <comparable.h>
#include <utils.h>
#include <unistd.h>
#include <time.h>

typedef enum { READY, RUNNING, BLOCKED } State;

struct Process {
	struct Process_LT *lt;
	pid_t pid;
	unsigned AT,	/* Arrival time */
		 TC,	/* Total CPU time */
		 CB,	/* CPU burst */
		 IO,	/* IO burst */
		 FT,	/* Finishing time */
		 IT,	/* IO time (time in blocked state) */
		 CW,	/* CPU waiting time (time in ready state) */
		 remaining_CPU_time;
	State state;
};

typedef struct Process* Process;

struct Process_LT {
	Builder bld;
	bool lt_initialized;
	Process (*new)(const Builder, pid_t, int, int, int, int);
	void (*delete)(Process);
	Process (*clone)(const Process);
	char *(*to_string)(const Process);
	int (*compare)(const Process, const Process);
};

typedef struct Process_LT* Process_LT;

extern const struct Builder __Process__;

#endif
