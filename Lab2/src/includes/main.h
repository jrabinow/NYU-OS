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

#ifndef MAIN_H
#define MAIN_H

/* standard header files */
#include <getopt.h>

/* header files that are specific to this project (passed to gcc with -I
 * option) */
#include <scheduler.h>
#include <roundrobin.h>
#include <random.h>
#include <utils.h>

#define validate_optarg()\
	if(optarg[1] != '\0') {\
		fputs("Error: invalid scheduler specified\n\n", stderr);\
		usage(argv[0], stderr);\
		exit(EXIT_FAILURE);\
	}

int parse_args(int argc, char *argv[], bool *trace, Scheduler *scheduler);

#define new(builder, ...)\
	((__##builder##__).lt->new((const Builder) &__##builder##__,\
	##__VA_ARGS__))
#define	delete(obj)		lt->delete(obj);
#define clone(obj)		lt->clone(obj);
#define print(obj)		lt->print(obj);
#define to_string(obj)		lt->to_string(obj);
#define run(sched, file, trace)	lt->run(sched, file, trace);

#endif
