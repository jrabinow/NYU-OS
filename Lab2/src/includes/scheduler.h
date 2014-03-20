#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <utils.h>
#include <object.h>
#include <process.h>
#include <random.h>
#include <FIFO.h>

struct Scheduler {
	struct Scheduler_LT *lt;
	time_t last_event;
	double CPU_use, IO_use, turnaround, cpu_waiting, throughput;
	FIFO reaped, IO_blocking;
};

typedef struct Scheduler* Scheduler;

struct Scheduler_LT {
	/* ----------- OBJECT ----------- */
	const Builder bld;
	bool lt_initialized;
	Scheduler (*new)(const Builder);
	void (*delete)(Scheduler);
	Scheduler (*clone)(const Scheduler);
	void (*print)(const Scheduler);
	/* ---------- SCHEDULER ---------- */
	Process (*get_event)(Scheduler);
	void (*put_event)(Scheduler, Process);
	void (*run)(Scheduler, bool);
	void (*reap)(Scheduler, Process);
};

typedef struct Scheduler_LT* Scheduler_LT;

extern const struct Builder __Scheduler__, __FCFS_Scheduler__, __LCFS_Scheduler__, __SJF_Scheduler__, __RR_Scheduler__;

#endif
