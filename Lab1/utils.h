/*
 *  Copyright (C) 2014 Julien Rabinow <jnr305@nyu.edu>
 *
 *  This file is part of Lab1-Linker.
 *
 *  Lab1-Linker is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Lab1-Linker is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Lab1-Linker.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UTILS
#define UTILS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* https://lkml.org/lkml/2013/8/31/138
 * No problems so far, but will probably take Torvalds advice and stop
 * using this */
#ifdef C99
# include <stdbool.h>
#else
 typedef enum { false = 0, true = 1 } bool;
#endif

void usage(char *progname);

FILE *xfopen(const char *path, const char *mode);
void *xrealloc(void *ptr, size_t size);
void *xmalloc(size_t size);
char *xstrdup(const char *str);

typedef enum {
	NUM_EXPECTED,
	SYM_EXPECTED,
	ADDR_EXPECTED,
	SYM_TOLONG,
	TOO_MANY_DEF_IN_MODULE,
	TOO_MANY_USE_IN_MODULE,
	TOO_MANY_SYMBOLS,
	TOO_MANY_INSTR
} Error_Codes;

void __parseerror(int errcode, int linenum, int lineoffset);

extern const char *current_workfile;

#endif
