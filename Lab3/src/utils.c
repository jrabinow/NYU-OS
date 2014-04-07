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

#include <utils.h>

void usage(char *progname, FILE *stream)
{
	fprintf(stream, "Usage: %s [OPTION]... INPUT-FILE [RANDOM FILE]\n" , progname);
	fputs(		"Options:	-a=ARG	algorithm used by the Virtual Memory Manager\n"
			"			ARG has the following specification: [ NlrfscCaA ]\n"
			"			The default algorithm is LRU\n"
			"		-f=ARG	Number of frames. The default number of frames is 32\n"
			"		-h	print this help message\n"
			"		-o=ARG	output format\n"
			"			ARG has the following specification: [OPFS]{1,4}\n", stream);
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
	char *new_str = NULL;
	int len = strlen(str);

	/* strdup is not ANSI C. We do its equivalent in this function */
	new_str = malloc(len + 1);
	if(new_str == NULL) {
		perror("Error allocating memory ");
		exit(EXIT_FAILURE);
	} else
		return memcpy(new_str, str, len + 1);
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

