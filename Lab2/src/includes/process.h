#ifndef PROCESS_H
#define PROCESS_H

#include <comparable.h>
#include <utils.h>
#include <unistd.h>
#include <time.h>

typedef enum { READY, RUNNING, BLOCKED, REAPED } State;

typedef Comparable_LT Process_LT;

struct Process {
	Process_LT lt;
	pid_t pid;
	unsigned AT,	/* Arrival time */
		 TC,	/* Total CPU time */
		 CB,	/* CPU burst */
		 IO,	/* IO burst */
		 FT,	/* Finishing time */
		 IT,	/* IO time (time in blocked state) */
		 CW;	/* CPU waiting time (time in ready state) */
	State state;
};

typedef struct Process* Process;

extern const struct Builder __Process__;

#endif
