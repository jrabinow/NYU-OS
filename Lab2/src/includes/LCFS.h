#ifndef LCFS_H
#define LCFS_H

#include <scheduler.h>
#include <stack.h>

struct LCFS_Scheduler {
	Scheduler_LT lt;
	time_t last_event;
	double CPU_use, IO_use, turnaround, cpu_waiting, throughput;
	FIFO reaped, IO_blocking;
	Stack ready_queue;
};

typedef struct LCFS_Scheduler* LCFS_Scheduler;

#endif
