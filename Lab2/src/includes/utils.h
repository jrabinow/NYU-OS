#ifndef UTILS_H
#define UTILS_H

/* generic utils, useful for any program */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum { false = 0, true } bool;

void usage(char *progname);

/* Forall c in str, isdigit(c) must return true.
 * In this particular case, negative values (strings beginning with '-') will
 * be considered invalid */
bool is_valid_int(const char *str);

/* avoid having to check for NULL return values: program prints an error
 * message and exits */
FILE *xfopen(const char *path, const char *mode);
void *xrealloc(void *ptr, size_t size);
void *xmalloc(size_t size);
char *xstrdup(const char *str);

/* finishes reading line. For use with scanf/fscanf */
#define empty_buffer(stream)	{\
	int __c__;\
	while((__c__ = getc(stream)) != EOF && __c__ != '\n');\
}

/* prevents newbies from complaining about how C is a horrible language
 * with weird and indecifferable function names :p */
#define equals(str1, str2)	(strcmp(str1, str2) == 0)

#endif
