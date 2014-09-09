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

#ifndef UTILS_H
#define UTILS_H

/* generic utils, useful for any program */

#if defined(__STDC__)
# define C89
# if defined(__STDC_VERSION__)
#  define C90
#  if (__STDC_VERSION__ >= 199901L) && ! defined C99
#   define C99
#  endif
# endif
#endif

#ifdef __STRICT_ANSI__
#include <sys/types.h>
#endif

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* https://lkml.org/lkml/2013/8/31/138
 * No problems so far, but will probably take Torvalds advice and stop
 * using this */
#ifdef C99
# include <stdbool.h>
#else
 typedef enum { false = 0, true = 1 } bool;
#endif

void usage(char *progname, FILE *outstream);

/* swallow whitespace and comment lines */
void swallow_whitespace(FILE *stream);

/* avoid having to check for NULL return values: program prints an error
 * message and exits */
FILE *xfopen(const char *path, const char *mode);
void *xrealloc(void *ptr, size_t size);
void *xmalloc(size_t size);
char *xstrdup(const char *str);

void *initialize_vector(void *dest, const void *src, size_t size, size_t nmemb);

/* finishes reading line. For use with scanf/fscanf */
#define empty_buffer(stream)	{\
	int __c__;\
	while((__c__ = getc(stream)) != EOF && __c__ != '\n');\
}

/* prevents newbies from complaining about how C is a horrible language
 * with weird and indecifferable function names :p */
#define equals(str1, str2)	(strcmp(str1, str2) == 0)


struct mempool {
	void *mem, **ptrs;
	size_t size, nmemb, index;
};

void mempool_create(struct mempool *mp, size_t size, size_t nmemb);
void *mempool_alloc(struct mempool *mp);
void mempool_free(struct mempool *mp, void *ptr);
void mempool_delete(struct mempool *mp);

#endif
