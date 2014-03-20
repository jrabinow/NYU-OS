#ifndef MAIN_H
#define MAIN_H

/* standard header files */
#include <getopt.h>

/* header files that are specific to this project (passed to gcc with -I option) */
#include <scheduler.h>
#include <roundrobin.h>
#include <random.h>
#include <utils.h>

#define validate_optarg()	if(optarg[1] != '\0') {\
					fputs("Error: invalid scheduler specified\n\n", stderr);\
					usage(argv[0]);\
					exit(EXIT_FAILURE);\
				}

int parse_args(int argc, char *argv[], bool *trace, Scheduler *scheduler);

#define new(builder, ...)	((__##builder##__).lt->new((const Builder) &__##builder##__, ##__VA_ARGS__))
#define	delete(obj)		lt->delete(obj);
#define clone(obj)		lt->clone(obj);
#define print(obj)		lt->print(obj);
#define put_event(sched, p)	lt->put_event(sched, p)
#define get_event(sched)	lt->get_event(sched)
#define run(sched, trace)	lt->run(sched, trace);

#endif
