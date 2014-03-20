#include <utils.h>

void usage(char *progname)
{
	fprintf(stderr, "Usage: %s [OPTION]... path/to/input-file path/to/random/file\n" , progname);
	fputs(		"Options:	-v	verbose mode\n"
			"		-s=ARG	scheduler mode\n"
			"			ARG has the following specification: [ FLS | R<num> ]\n"
			"			The scheduler mode is FCFS by default\n\n", stderr);
}

void *xmalloc(size_t size)
{
	void *ptr = malloc(size);

	if(ptr == NULL) {
		perror("Error allocating memory ");
		exit(EXIT_FAILURE);
	} else
		return ptr;
}

void *xrealloc(void *ptr, size_t size)
{
	ptr = realloc(ptr, size);
	if(ptr == NULL) {
		perror("Error allocating memory ");
		exit(EXIT_FAILURE);
	} else
		return ptr;
}

char *xstrdup(const char *str)
{
	char *new_str = strdup(str);

	if(new_str == NULL) {
		perror("Error allocating memory ");
		exit(EXIT_FAILURE);
	} else
		return new_str;
}

FILE *xfopen(const char *path, const char *mode)
{
	FILE *f = fopen(path, mode);

	if(f == (FILE*) NULL) {
		perror("Error opening file ");
		exit(EXIT_FAILURE);
	} else
		return f;
}

bool is_valid_int(const char *str)
{
	for(; *str != '\0'; str++)
		if( ! isdigit(*str))
			return false;
	return true;
}

