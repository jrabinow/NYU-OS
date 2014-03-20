#ifndef RR_H
#define RR_H

#include <scheduler.h>

struct RR_Scheduler_LT {
	/* ----------- OBJECT ----------- */
	const Builder bld;
	bool lt_initialized;
	Scheduler (*new)(const Builder bld, int quantum);
	void (*delete)(Scheduler);
	Scheduler (*clone)(const Scheduler);
	void (*print)(const Scheduler);
	/* ---------- SCHEDULER ---------- */
	Process (*get_event)(Scheduler);
	void (*put_event)(Scheduler, Process);
	void (*run)(Scheduler, bool);
	void (*reap)(Scheduler, Process);
	/* --------- RR_SCHEDULER --------- */
	int (*get_quantum)(const Scheduler);
	void (*set_quantum)(Scheduler, int);
};

typedef struct RR_Scheduler_LT* RR_Scheduler_LT;

struct RR_Scheduler {
	RR_Scheduler_LT lt;
	time_t last_event;
	double CPU_use, IO_use, turnaround, cpu_waiting, throughput;
	FIFO reaped, IO_blocking, ready_queue;
	int quantum;
};

typedef struct RR_Scheduler* RR_Scheduler;

#endif
