/* utils.h
 * Copyright (C) Julien Rabinow <jnr305@nyu.edu>
 *
 * utils.h is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * utils.h is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _UTILS_H
#define _UTILS_H
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* Currently, __MANAGE_MEM__ enables only a very limited amount of features. Although 
 * the possibilities may very well expand soon, there is a tiny overhead associated
 * with turning on the features. They are therefore left off by default */
//#define __MANAGE_MEM__
#define __USING_VALGRIND__	/* If you leave __MANAGE_MEM__ disabled, there is absolutely
				 *  no penalty (both performance and memory wise) with
				 *  leaving this on. */

extern int errno;

typedef enum { false = 0, true } bool;

#define SWAP(a, b, TYPE)	{\
	TYPE __tmp__ = a;\
	a = b;\
	b = __tmp__;\
}

/* ----- AUTOMATIC ERROR MANAGEMENT ----- */

/* exit program with failed status if malloc and consorts fail
 * -> no more error checking necessary
 * free() when done */

#define MAX_RETRIES	5
#define MAX_RETRIES_ALLOC	MAX_RETRIES

void *xmalloc(size_t size);
void *xcalloc(size_t nmemb, size_t size);
char *xstrdup(const char *str);
void *xrealloc(void *ptr, size_t size);

#define MAX_RETRIES_OPEN	MAX_RETRIES

FILE *xfopen(const char *path, const char *mode);
FILE *xfdopen(int fd, const char *mode);

#ifndef _WIN32
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
int xopen(const char *path, int flags);
#define xpipe(pipefd)\
	if(pipe(pipefd) != 0) {\
		perror("Error creating pipe ");\
		exit(EXIT_FAILURE);\
	}
#define xdup2(oldfd, newfd)\
	if(dup2(oldfd, newfd) == -1) {\
		perror("Error redirecting output ");\
		exit(EXIT_FAILURE);\
	}
void register_signal_handler(int signum, void (*sighandler)(int));
#endif

/* ----- MEMORY MANAGEMENT ----- */

#ifdef __MANAGE_MEM__
/* To use the following functions, you must call init_alloc from your program before allocating any memory 
 * WARNING: DO NOT MIX calls to malloc() with calls to xmalloc(). The functions will only work correctly 
 * for memory allocated through xmalloc.
 * Finally, remember to delete memory structs allocated by other functions (getaddrinfo for example) */

/* TODO: get rid of all these limitations by exporting a hook to malloc */
void init_alloc(void);

/* returns true if ptr points to dynamically allocated memory, false otherwise. You can now do things like
 * if(is_allocated(myptr))
 * 	free(myptr);
 */
bool is_allocated(void *ptr);

/* if you're too lazy to do like in the previous example, this function does it for you */
void xfree(void *ptr);
#endif

/* ----- STRING MANAGEMENT ----- */

/* returns true if str starts with prefix */
bool startswith(const char *str, const char *prefix);
/* returns true if str ends with suffix */
bool endswith(const char *str, const char *suffix);
/* returns true if isdigit() returns true for all chars in str, false otherwise */
bool is_valid_int(const char *str);
/* Same idea as is_valid_int, except checks for float (allows for a single '.' in str) */
bool is_valid_float(const char *str);

/* Creates a new string composed of str2 appended to str1
 * free() when done */
char *const_append(const char *str1, const char *str2);

/* Appends str2 to str1. str1 MUST be dynamically allocated.
 * free() when done */
char *append(char *str1, const char *str2);

/* sets str to lower case */
void str_tolower(char *str);
/* sets str to upper case */
void str_toupper(char *str);

/* extracts portion of str contained between 1st occurence of start and first occurence of end. 
 * If str does not contain start or end char, or there is no end character after start, 
 * returns NULL.
 * Example:
 * extract("foo(bar)", '(', ')') will return a pointer to the string "bar"
 * free after usage */
char *extract(const char *str, char start, char end);


char *trim(const char *str);
char *insert(const char *str, char c, size_t pos);
char *insert_str(const char *str, const char *ins, size_t pos);
char *erase(const char *str, size_t pos);
char *erase_str(const char *str, size_t pos, size_t len);

/* convert from hexadecimal format string to integer */
int hexatoi(const char *hex);

/* return pointer to first occurence of char in s not equal to c. If str is made up entirely
 * of c characters, returns NULL */
char *neg_strchr(const char *s, int c);

/* returns pointer to buffer (must be preallocated and sufficiently large) containing base 10
 * string representation of val buffer passed as second argument must be sufficiently large. */
char *itoa(int val, char *buffer);

/* Efficiently fill dest with contents of src. src is a single element of size size. dest is a
 * memory buffer of size size * nmemb
 * Example:
 * long unsigned int dest[100], value = 42;
 * initialize_vector(dest, &value, sizeof value, 100); fills all 100 elements of dest with the
 * value 42 */
void *initialize_vector(void *dest, const void *src, size_t size, size_t nmemb);

/* ----- READING DATA ----- */

/* reads a complete line (no length limit) from file.
 * free() buffer when done */
char *readLine(FILE *stream);

/* read as much as possible from file descriptor.
 * free() buffer when done */
#include <ctype.h>
char *read_file_descriptor(int fd);

/* separates str along separator chars into non-empty tokens. If str is composed only
 * of separator chars, returnArray will point to NULL.
 * Otherwise, returnArray will point to dynamically allocated array with one string token
 * per array element.
 * return value is size of array.
 * free() *returnArray, (*returnArray)[0], (*returnArray)[1] ... when done */
size_t split_str(const char *str, const char separator, char ***returnArray);

/* Same thing as split_str, except that elements in returnArray are pointers to locations
 * in the original string. Use only if you have no more use for original data as it is
 * modified (each separator char is replaced by a '\0')
 * This function is more efficient as there is no data copying involved, and the only 
 * allocated memory is *returnArray
 */
size_t split_str_lite(char *str, const char separator, char ***returnArray);

/* returns true if str1 and str2 are 2 the same strings. Helps make code more readable*/
#define equals(str1, str2)	(strcmp(str1, str2) == 0)

/* Empties buffer till nothing left to read or hits end of line. Useful with scanf/fscanf */
#define empty_buffer(stream)	{\
	int __c__;\
	while((__c__ = getc(stream)) != EOF && __c__ != '\n');\
}

/* ----- FILESYSTEM ----- */

/* returns 1 if path is dir, -1 in case of error and 0 otherwise */
#include <sys/stat.h>
int is_dir(char *path);

/* creates a new dynamically allocated string of the form "path/filename"
 * (or "path\filename" if on windows)
 * free() when done */
char *make_path(const char *path, const char *filename);

/* iterates over every file in filesystem under path */
#include <dirent.h>
void *dirwalk(const char *path, void* (*func)(void* arg, char* path), void *arg);

/* ----- TERMINAL FUN ----- */
#ifndef _WIN32

#define clear_screen()			printf("\x1B[2J\x1B[0;0H");

typedef enum { BLACK, RED, GREEN, YELLOW, BLUE, PINK, CYAN, WHITE } Color;
typedef enum { NORMAL, BOLD, DARK, ITALIC, UNDERLINED } Style;

void set_color_env(Color c, Color bgc, Style s);
void print_string(char *str, Color c, Color bgc, Style s);
#define reset_color_profile()		printf("\x1B[0m")

#include <termios.h>
/* Do not display keyboard input on terminal */
void turn_echoing_off(void);
/* Display keyboard input on terminal */
void turn_echoing_on(void);

/* program reads input without user having to press enter */
void instant_getchar(void);
/* Restore default behaviour */
void normal_getchar(void);

#endif	/* ifndef _WIN32 */
#endif	/* ifndef _UTILS_H */
