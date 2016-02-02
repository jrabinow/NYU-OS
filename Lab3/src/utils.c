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
	fputs(		"Options:"	"\t-a=ARG"	"\talgorithm used by the Virtual Memory Manager\n"
							"\t\t\tARG has the following specification: [ NlrfscCaA ]\n"
							"\t\t\tThe default algorithm is LRU\n"
					"\t\t-f=ARG"	"\tNumber of frames. The default number of frames is 32\n"
					"\t\t-h"	"\tprint this help message\n"
					"\t\t-o=ARG"	"\toutput format\n"
							"\t\t\tARG has the following specification: [OPFS]{1,4}\n", stream);
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

void *initialize_vector(void *dest, const void *src, size_t size, size_t nmemb)
{
	size_t i;

	memcpy(dest, src, size);
	for(i = 1; i << 1 <= nmemb; i <<= 1)
		memcpy(dest + i * size, dest, i * size);
	memcpy(dest + i * size, dest, (nmemb - i) * size);
	return dest;
}

void swallow_whitespace(FILE *stream)
{
	int c;
	
	do {
		while((c = getc_unlocked(stream)) == ' ' || c == '\t' || c == '\n');
		if(c == '#')
			empty_buffer(stream); 
	} while(c == '#');
	ungetc(c, stream);
}

void mempool_create(struct mempool *mp, size_t size, size_t nmemb)
{
	unsigned i;
	void *val;

	mp->mem = xmalloc((sizeof(unsigned) + size) * nmemb);
	mp->ptrs = xmalloc(sizeof(unsigned*) * nmemb);
	mp->size = size;
	mp->nmemb = nmemb;
	mp->index = 0;

	val = mp->mem;
	for(i = 0; i < nmemb; i++) {
		mp->ptrs[i] = val;
		val += sizeof(unsigned) + size;
	}
}

void *mempool_alloc(struct mempool *mp)
{
/*
 * Problem when enlarging memory pool: mp->mem will point to a different chunk of mem,
 * but the pointers pointing to the memory in the mempool will not be updated. Not much
 * we can do about that as of now => let's  disable pool enlarging 
 *
 * WARNING: this means that if you call mempool_alloc more than is reasonable, you WILL
 * have problems
 *
	unsigned i;
	void *val;

	if(mp->index == mp->nmemb) {
		mp->mem = xrealloc(mp->mem, (sizeof(unsigned) + mp->size) * (mp->nmemb <<= 1));
		mp->ptrs = xrealloc(mp->ptrs, sizeof(unsigned*) * mp->nmemb);
		
		val = mp->mem;
		for(i = 0; i < mp->nmemb; i++) {
			mp->ptrs[i] = val;
			val += sizeof(unsigned) + mp->size;
		}
	}
*/
	void *ptr = NULL;

	if(mp->index < mp->nmemb) {
		*(unsigned*) mp->ptrs[mp->index] = mp->index;
		ptr = (void*) mp->ptrs[mp->index++] + sizeof(unsigned);
	}
	return ptr;
}

void mempool_free(struct mempool *mp, void *ptr)
{
	void *swap;
	unsigned idx = *(unsigned*) (ptr - sizeof(unsigned));

	swap = mp->ptrs[idx];
	mp->ptrs[idx] = mp->ptrs[--mp->index];
	mp->ptrs[mp->index] = swap;
}

void mempool_delete(struct mempool *mp)
{
	free(mp->mem);
	free(mp->ptrs);
}
