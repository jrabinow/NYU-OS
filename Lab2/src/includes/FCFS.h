#ifndef FCFS_H
#define FCFS_H

#include <scheduler.h>

struct FCFS_Scheduler {
	Scheduler_LT lt;
	time_t last_event;
	double CPU_use, IO_use, turnaround, cpu_waiting, throughput;
	FIFO reaped, IO_blocking, ready_queue;
};

typedef struct FCFS_Scheduler* FCFS_Scheduler;

#endif
