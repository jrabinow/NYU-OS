#ifndef SJF_H
#define SJF_H

#include <scheduler.h>
#include <FIFO.h>
#include <heap.h>

struct SJF_Scheduler {
	Scheduler_LT lt;
	time_t last_event;
	double CPU_use, IO_use, turnaround, cpu_waiting, throughput;
	FIFO reaped, IO_blocking;
	Heap heap;
};

#endif
