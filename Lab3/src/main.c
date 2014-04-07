/*
 *  Copyright (C) 2014 Julien Rabinow <jnr305@nyu.edu>
 *
 *  This file is part of Lab3-Mem-manager.
 *
 *  Lab3-Mem-manager is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Lab3-Mem-manager is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Lab3-Mem-manager. If not, see <http://www.gnu.org/licenses/>.
 */

#include <main.h>

int main(int argc, char *argv[])
{
	FILE *input = NULL;
	VMM vmm;
	Verbose_Flag vflags;
	bool need_random;

	optind = parse_args(argc, argv, &vmm, &vflags);
	need_random = instance_of(vmm, NRU_VMM) || instance_of(vmm, Random_VMM);

	if((need_random && argc - optind != 2) || (! need_random && argc - optind != 1)) {	// if improper number of args
		usage(argv[0], stderr);
		exit(EXIT_FAILURE);
	} else if(need_random)
		init_random(argv[optind + 1]);
	input = xfopen(argv[optind], "r");

	vmm->run(vmm, input, vflags);

	fclose(input);
	if(need_random)
		delete_random();

	return 0;
}

int parse_args(int argc, char *argv[], VMM *vmm, Verbose_Flag *vflags)
{
	int c, num_frames = 32;
	char alg = 'l';

	*vflags = 0;
	while((c = getopt(argc, argv, "a:f:ho:")) != -1) {
		switch(c) {
			case 'a':
				if(optarg[1] != '\0') {
					fputs(ERR_MSG, stderr);
					usage(argv[0], stderr);
					exit(EXIT_FAILURE);
				}
				alg = optarg[0];
				break;
			case 'f':
				num_frames = (unsigned) atoi(optarg);
				if(num_frames <= 0 || 268435456 <= num_frames) {	/* 28 bit PTE */
					fputs("Error: invalid number of frames.\n\n", stderr);
					usage(argv[0], stderr);
					exit(EXIT_FAILURE);
				}
				break;
			case 'h':
				usage(argv[0], stdout);
				exit(EXIT_SUCCESS);
			case 'o':
				for(c = 0; optarg[c] != '\0'; c++)
					switch(optarg[c]) {
						case 'O':
							*vflags |= O;
							break;
						case 'P':
							*vflags |= P;
							break;
						case 'F':
							*vflags |= F;
							break;
						case 'S':
							*vflags |= S;
							break;
						default:
							fprintf(stderr, "Invalid output option '%c'\n\n", c);
							usage(argv[0], stderr);
							exit(EXIT_FAILURE);
					}
				break;
			default:
				usage(argv[0], stderr);
				exit(EXIT_FAILURE);
		}
	}

	switch(alg) {
		case 'N':
			*vmm = new(NRU_VMM, num_frames);
			break;
		case 'l':
			*vmm = new(LRU_VMM, num_frames);
			break;
		case 'r':
			*vmm = new(Random_VMM, num_frames);
			break;
		case 'f':
			*vmm = new(FIFO_VMM, num_frames);
			break;
		case 's':
			*vmm = new(SecChance_VMM, num_frames);
			break;
		case 'c':
			*vmm = new(PClock_VMM, num_frames);
			break;
		case 'C':
			*vmm = new(VClock_VMM, num_frames);
			break;
		case 'a':
			*vmm = new(PAging_VMM, num_frames);
			break;
		case 'A':
			*vmm = new(VAging_VMM, num_frames);
			break;
		default:
			fputs(ERR_MSG, stderr);
			usage(argv[0], stderr);
			exit(EXIT_FAILURE);
	}
	return optind;
}

