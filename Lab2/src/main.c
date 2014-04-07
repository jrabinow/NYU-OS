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

#include <main.h>

int main(int argc, char *argv[])
{
	bool trace = false;
	FILE *input = NULL;
	Scheduler scheduler = NULL;

	optind = parse_args(argc, argv, &trace, &scheduler);
	if(argc - optind != 2) {
		usage(argv[0], stderr);
		exit(EXIT_FAILURE);
	}

	/* give list of events to process to scheduler */
	input = xfopen(argv[optind++], "r");
	scheduler->read_input(scheduler, input);
	fclose(input);
	/* initialize random number generator */
	init_random(argv[optind]);

	scheduler->run(scheduler, trace);

	scheduler->delete(scheduler);
	delete_random();

	return 0;
}

int parse_args(int argc, char *argv[], bool *trace, Scheduler *scheduler)
{
	int c, quantum;
	bool scheduler_initialized = false;

	while((c = getopt(argc, argv, "hs:v")) != -1) {
		switch(c) {
			case 'h':
				usage(argv[0], stdout);
				exit(EXIT_SUCCESS);
			case 's':
				scheduler_initialized = true;
				switch(optarg[0]) {
					case 'F':
						/* checks that strlen(optarg) == 1 */
						validate_optarg();
						*scheduler = new(FCFS_Scheduler);
						break;
					case 'L':
						validate_optarg();
						*scheduler = new(LCFS_Scheduler);
						break;
					case 'S':
						validate_optarg();
						*scheduler = new(SJF_Scheduler);
						break;
					case 'R':
						if( ! is_valid_int(optarg + 1)
								|| (quantum = atoi(optarg + 1)) <= 0) {
							fputs("Error: invalid quantum value\n\n", stderr);
							usage(argv[0], stderr);
							exit(EXIT_FAILURE);
						}
						*scheduler = new(RR_Scheduler,
								quantum);
						break;
					default:
						fputs("Error: invalid scheduler "
							"specified\n\n", stderr);
						usage(argv[0], stderr);
						exit(EXIT_FAILURE);
				}
				break;
			case 'v':
				*trace = true;
				break;
			default:
				usage(argv[0], stderr);
				exit(EXIT_FAILURE);
		}
	}

	if( ! scheduler_initialized)
		*scheduler = new(FCFS_Scheduler);
	return optind;
}

