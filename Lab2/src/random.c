#include <random.h>

/*
 * No need to read ALL random values into array at once: if we only need 3 random values, why
 * read the whole file?
 * 32 values are read into the array at start. Every time we need an additional $ 2^n+1 $th random number,
 * we try to read array_size * 2 more random numbers from the file.
 * To avoid static variables as much as possible, array_size is saved in array[-1],
 * and the offset into the file is saved in array[-2].
 * Once the file is empty, emptied_file is set to true and array[-2] represents the final size
 * of the array
 */

static unsigned long *randvals = NULL;
static bool emptied_file = false;
static char *random_file = NULL;

/* MUST BE A POWER OF TWO */
#define START_MEM	32

#define needs_more_mem(array)		((array)[-1] >= START_MEM && ((array)[-1] & ((array)[-1] - 1)) == 0)

void init_random(char *input_path)
{
	int i;
	FILE *input = NULL;

	random_file = input_path;
	input = xfopen(input_path, "r");
	randvals = (unsigned long*) xmalloc((START_MEM + 2) * sizeof(unsigned long)) + 2;
	randvals[-1] = 0;

	if(fscanf(input, "%*u") != 0) {	/* ignore number of random values in file. We don't need it */
		perror("Error reading value from random number file ");
		fputs("Skipping line…", stderr);
		clearerr(input);
	}

	for(i = 0; i < START_MEM && ! emptied_file; i++) {
		if(fscanf(input, "%lu", &randvals[i]) == EOF) {
			switch(ferror(input)) {
				case 0:	/* real end of file */
					randvals[-2] = i;
					emptied_file = true;
					randvals = (unsigned long*) xrealloc(randvals - 2, (2 + i) * sizeof(unsigned long)) + 2;
					break;
				default: /* invalid matching by fscanf */
					perror("Error reading value from random number file ");
					fputs("Skipping line…", stderr);
					clearerr(input);
					i--;
			}
		}
		empty_buffer(input);
	}
	if(! emptied_file)
		randvals[-2] = ftell(input);
	fclose(input);
}

void fill_randvals_array(void)
{
	int i, upper_bound;
	FILE *input = xfopen(random_file, "r");

	fseek(input, randvals[-2], SEEK_SET);
	upper_bound = randvals[-1] << 1;
	randvals = (unsigned long*) xrealloc(randvals - 2, (upper_bound + 2) * sizeof(unsigned long)) + 2;

	for(i = randvals[-1]; i < upper_bound && ! emptied_file; i++) {
		if(fscanf(input, "%lu", &randvals[i]) == EOF) {
			switch(ferror(input)) {
				case 0:
					randvals[-2] = i;
					emptied_file = true;
					randvals = (unsigned long*) xrealloc(randvals - 2, (2 + i) * sizeof(unsigned long)) + 2;
					break;
				default:
					perror("Error reading value from random number file ");
					fputs("Skipping line…", stderr);
					clearerr(input);
					i--;
			}
		}
		empty_buffer(input);
	}
	if(! emptied_file)
		randvals[-2] = ftell(input);
	fclose(input);
}

int myrandom(int burst)
{
	if(randvals == NULL) {
		if(random_file != NULL)
			init_random(random_file);
		else
			return 0;
	}
	if( ! emptied_file && needs_more_mem(randvals))
		fill_randvals_array();
	else if(randvals[-1] >= randvals[-2])
	/* logically, we should check if the file is empty in this condition.
	 * In practice, there is no need, as the file offset will always be larger than the number of
	 * values read from the file */
		randvals[-1] = 0;
	return 1 + (randvals[randvals[-1]++] % burst);
}

void delete_random(void)
{
	free(randvals - 2);
}

#undef START_MEM
