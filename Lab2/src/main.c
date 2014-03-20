#include <main.h>

int main(int argc, char *argv[])
{
	bool trace = false;
	FILE *input = NULL;
	Scheduler scheduler = NULL;
	Process p = NULL;
	pid_t pid = 0;
	int AT, TC, CB, IO, count;

	if(argc < 3) {
		usage(argv[0]);
		return 0;
	}

	optind = parse_args(argc, argv, &trace, &scheduler);

	input = xfopen(argv[optind++], "r");
	init_random(argv[optind]);

	while((count = fscanf(input, "%d %d %d %d", &AT, &TC, &CB, &IO)) == 4) {
		p = new(Process, pid++, AT, TC, CB, IO);
		scheduler->put_event(scheduler, p);
		empty_buffer(input);	/* remove trailing newlines from file */
	}
	fclose(input);

	scheduler->run(scheduler, trace);

	scheduler->print(scheduler);
	scheduler->delete(scheduler);
	delete_random();

	return 0;
}

int parse_args(int argc, char *argv[], bool *trace, Scheduler *scheduler)
{
	int c, quantum;
	bool scheduler_initialized = false;

	while((c = getopt(argc, argv, "vs:")) != -1) {
		switch(c) {
			case 'v':
				*trace = true;
				break;
			case 's':
				scheduler_initialized = true;
				switch(optarg[0]) {
					case 'F':
						validate_optarg();	/* checks that strlen(optarg) == 1 */
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
						if( ! is_valid_int(optarg + 1) || (quantum = atoi(optarg + 1)) <= 0) {
							fputs("Error: invalid quantum value\n\n", stderr);
							usage(argv[0]);
							exit(EXIT_FAILURE);
						}
						*scheduler = new(RR_Scheduler, quantum);
						break;
					default:
						fputs("Error: invalid scheduler specified\n\n", stderr);
						usage(argv[0]);
						exit(EXIT_FAILURE);
				}
				break;
			default:
				usage(argv[0]);
		}
	}

	if( ! scheduler_initialized)
		*scheduler = new(FCFS_Scheduler);
	return optind;
}

