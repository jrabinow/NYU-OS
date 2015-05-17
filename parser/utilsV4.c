/* utilsV4.c
 * Copyright (C) 2013 Julien Rabinow <jnr305@nyu.edu>
 *
 * utilsV4.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * utilsV4.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "utils.h"

#ifdef __MANAGE_MEM__
static void *heap_bottom = NULL, heap_top = NULL;
#ifdef __USING_VALGRIND__ || ifdef DEBUG
static void *__initial_alloc__ = NULL;

static void __clean_initial_alloc(void)
{
	free(__initial_alloc__);
}
#endif

void init_alloc(void)
{
	int count = 0;
	do {
		heap_bottom = heap_top = malloc(1);
		if(heap_bottom == NULL)
			switch(errno) {
				case ENOMEM:
					perror("Error allocating memory ");
					if(count++ < MAX_RETRIES_ALLOC) {
						fputs("Retrying in 100ms\n", stderr);
						usleep(100);
					} else {
						fputs(stderr, "Giving up after %d tries\n", MAX_RETRIES_ALLOC);
						exit(EXIT_FAILURE);
					}
					break;
				default:
					perror("Error allocating memory ");
					exit(EXIT_FAILURE);
			}
	} while(heap_bottom == NULL);
#ifdef __USING_VALGRIND__ || ifdef DEBUG
	__initial_alloc__ = heap_bottom;
	if(atexit(&__clean_initial_alloc)) {
		perror("Error registering cleanup function ");
		exit(EXIT_FAILURE);
	}
#endif
}

bool is_allocated(void *ptr)
{
	return heap_bottom <= ptr && ptr <= heap_top;
}

void xfree(void *ptr)
{
	if(heap_bottom <= ptr && ptr <= heap_top)
		free(ptr);
}
#endif

void *xmalloc(size_t size)
{
	void *ptr = NULL;
	int count = 0;

	do {
		ptr = malloc(size);
		if(ptr == NULL)
#ifdef __unix
			switch(errno) {
				case ENOMEM:
					perror("Error allocating memory ");
					if(count++ < MAX_RETRIES_ALLOC) {
						fputs("Retrying in 100ms\n", stderr);
						usleep(100);
					} else {
						fprintf(stderr, "Giving up after %d tries\n", MAX_RETRIES_ALLOC);
						exit(EXIT_FAILURE);
					}
					break;
				default:
#endif
					perror("Error allocating memory ");
					exit(EXIT_FAILURE);
#ifdef __unix
			}
#endif
	} while(ptr == NULL);
#ifdef __MANAGE_MEM__
	if(ptr < heap_bottom)
		heap_bottom = ptr;
	if(ptr > heap_top)
		heap_top = ptr;
#endif
	return ptr;
}

void *xcalloc(size_t nmemb, size_t size)
{
	void *ptr = NULL;
	int count = 0;

	do {
		ptr = calloc(nmemb, size);
		if(ptr == NULL)
#ifdef __unix
			switch(errno) {
				case ENOMEM:
					perror("Error allocating memory ");
					if(count++ < MAX_RETRIES_ALLOC) {
						fputs("Retrying in 100ms\n", stderr);
						usleep(100);
					} else {
						fprintf(stderr, "Giving up after %d tries\n", MAX_RETRIES_ALLOC);
						exit(EXIT_FAILURE);
					}
					break;
				default:
#endif
					perror("Error allocating memory ");
					exit(EXIT_FAILURE);
#ifdef __unix
			}
#endif
	} while(ptr == NULL);

#ifdef __MANAGE_MEM__
	if(ptr < heap_bottom)
		heap_bottom = ptr;
	if(ptr > heap_top)
		heap_top = ptr;
#endif
	return ptr;
}

char *xstrdup(const char *str)
{
	char *ptr = NULL;
	int count = 0;

	do {
		ptr = strdup(str);
		if(ptr == NULL)
#ifdef __unix
			switch(errno) {
				case ENOMEM:
					perror("Error allocating memory ");
					if(count++ < MAX_RETRIES_ALLOC) {
						fputs("Retrying in 100ms\n", stderr);
						usleep(100);
					} else {
						fprintf(stderr, "Giving up after %d tries\n", MAX_RETRIES_ALLOC);
						exit(EXIT_FAILURE);
					}
					break;
				default:
#endif
					perror("Error allocating memory ");
					exit(EXIT_FAILURE);
#ifdef __unix
			}
#endif
	} while(ptr == NULL);
#ifdef __MANAGE_MEM__
	if(ptr < heap_bottom)
		heap_bottom = ptr;
	if(ptr > heap_top)
		heap_top = ptr;
#endif
	return ptr;
}

void *xrealloc(void *ptr, size_t size)
{
	int count = 0;

	do {
		ptr = realloc(ptr, size);
		if(ptr == NULL)
#ifdef __unix
			switch(errno) {
				case ENOMEM:
					perror("Error allocating memory ");
					if(count++ < MAX_RETRIES_ALLOC) {
						fputs("Retrying in 100ms\n", stderr);
						usleep(100);
					} else {
						fprintf(stderr, "Giving up after %d tries\n", MAX_RETRIES_ALLOC);
						exit(EXIT_FAILURE);
					}
					break;
				default:
#endif
					perror("Error allocating memory ");
					exit(EXIT_FAILURE);
#ifdef __unix
			}
#endif
	} while(ptr == NULL);
#ifdef __MANAGE_MEM__
	if(ptr < heap_bottom)
		heap_bottom = ptr;
	if(ptr > heap_top)
		heap_top = ptr;
#endif
	return ptr;
}

#ifndef _WIN32
int xopen(const char *path, int flags)
{
	int fd, count = 0;

	do {
		fd = open(path, flags);
		if(fd == -1)
			switch(errno) {
				case ENOMEM:
					perror("Error opening file ");
					if(count++ < MAX_RETRIES_OPEN) {
						fputs("Retrying in 100ms\n", stderr);
						usleep(100);
					} else {
						fprintf(stderr, "Giving up after %d tries\n", MAX_RETRIES_OPEN);
						exit(EXIT_FAILURE);
					}
					break;
#ifdef _GNU_SOURCE
				case EINVAL:
					flags &= ~O_DIRECT;
#endif
				case EINTR:
					perror("Error opening file ");
					fputs("Retrying\n", stderr);
					count++;
					break;
				default:
					perror("Error opening file ");
					exit(EXIT_FAILURE);
			}
	} while(fd == -1);

	return fd;
}
#endif

FILE *xfopen(const char *path, const char *mode)
{
	FILE *f = NULL;
	int count = 0;

	do {
		f = fopen(path, mode);
		if(f == NULL)
#ifdef __unix
			switch(errno) {
				case ENOMEM:
					perror("Error opening file ");
					if(count++ < MAX_RETRIES_OPEN) {
						fputs("Retrying in 100ms\n", stderr);
						usleep(100);
					} else {
						fprintf(stderr, "Giving up after %d tries\n", MAX_RETRIES_OPEN);
						exit(EXIT_FAILURE);
					}
					break;
				case EINTR:
					perror("Error opening file ");
					fputs("Retrying\n", stderr);
					count++;
					break;
				default:
#endif
					perror("Error opening file ");
					exit(EXIT_FAILURE);
#ifdef __unix
			}
#endif
	} while(f == NULL);

	return f;
}

FILE *xfdopen(int fd, const char *mode)
{
	FILE *f = NULL;
	int count = 0;

	do {
		f = fdopen(fd, mode);
		if(f == NULL)
#ifdef __unix
			switch(errno) {
				case ENOMEM:
					perror("Error opening file ");
					if(count++ < MAX_RETRIES_OPEN) {
						fputs("Retrying in 100ms\n", stderr);
						usleep(100);
					} else {
						fprintf(stderr, "Giving up after %d tries\n", MAX_RETRIES_OPEN);
						exit(EXIT_FAILURE);
					}
					break;
				case EINTR:
					perror("Error opening file ");
					fputs("Retrying\n", stderr);
					count++;
					break;
				default:
#endif
					perror("Error opening file ");
					exit(EXIT_FAILURE);
#ifdef __unix
			}
#endif
	} while(f == NULL);

	return f;
}

/* ----- MATH ----- */

unsigned int gcd(unsigned int u, unsigned int v)
{
	int shift;

	if (u == 0)
		return v;
	if (v == 0)
		return u;
	/* Let shift := lg K, where K is the greatest power of 2
	   dividing both u and v. */
	for (shift = 0; ((u | v) & 1) == 0; shift++) {
		u >>= 1;
		v >>= 1;
	}

	/* while u is even */
	while ((u & 1) == 0)
		u >>= 1;
	do {
		while ((v & 1) == 0) 
			v >>= 1;
		/* Now u and v are both odd. Swap if necessary so u <= v,
		   then set v = v - u (which is even). */
		if (u > v)
			SWAP(u, v, unsigned int);
		v -= u;
	} while (v != 0);

	/* restore common factors of 2 */
	return u << shift;
}

/* ---------- STRING MANAGEMENT ---------- */

int hexatoi(const char *hex)
{
	register size_t size;
	register unsigned res, pow;

	for(size = 0; hex[size] != '\0'; size++);

	for(pow = 1, res = 0, size--; size > 0; pow <<= 4, size--)
		res += pow * (hex[size] >= 'a' ? hex[size] - 87 : (hex[size] >= 'A' ? hex[size] - 55 : hex[size] - '0'));

	if(hex[0] == '-')
		res = 0 - res;
	else
		res += pow * (hex[0] >= 'a' ? hex[0] - 87 : (hex[0] >= 'A' ? hex[0] - 55 : hex[0] - '0'));
	return res;
}

bool is_valid_int(const char *str)
{
	if( ! isdigit(*str) && *str != '-')
		return false;

	for(str++; *str != '\0'; str++)
		if( ! isdigit(*str))
			return false;
	return true;
}

bool is_valid_float(const char *str)
{
	bool period = false;

	if(*str == '.')
		period = true;
	else if( ! isdigit(*str) && *str != '-')
		return false;

	for(str++; *str != '\0'; str++)
		if(*str == '.' && period == false)
			period = true;
		else if( ! isdigit(*str))
			return false;
	return true;
}

bool is_valid_hex(const char *str)
{
	size_t i;

	for(i = 0; str[i] != '\0'; i++)
		if( ! isxdigit(str[i]))
			return false;
	return true;
}

bool startswith(const char *str, const char *prefix)
{
	while(*str == *prefix) {
		str++, prefix++;
		if(*str == '\0')
			break;
	}
	return *prefix == '\0';
}

bool endswith(const char *str, const char *suffix)
{
	int i, j;
	for(i = 0; str[i] != '\0'; i++);
	for(j = 0; suffix[j] != '\0'; j++);

	if(i < j)
		return false;

	for(; j >= 0; i--, j--)
		if(str[i] != suffix[j])
			return false;
	return true;
}

/* xmalloc */
char *trim(const char *str)
{
	const char *start_str, *end_str;
	char *ptr;
	size_t len;

	for(start_str = str; isspace(*start_str); start_str++);
	for(end_str = start_str; ! isspace(*end_str) && *end_str != '\0'; end_str++);

	for(ptr = (char*) end_str; *ptr != '\0'; ptr++)
		if(isspace(*ptr) && ! isspace(*(ptr - 1)))
			end_str = ptr;
	len = (size_t) (end_str - start_str);
	ptr = (char*) xmalloc(len + 1);
	memcpy(ptr, start_str, len);
	ptr[len] = '\0';

	return ptr;
}

/* xmalloc */
char *extract(const char *str, char start, char end)
{
	char *extracted = NULL;
	unsigned i;

	for(; *str != start && *str != '\0'; str++);

	if(*str != '\0') {
		for(str++, i = 0; str[i] != end && str[i] != '\0'; i++);
		if(str[i] != '\0' || end == '\0') {
			extracted = (char*) xmalloc(i + 1);
			memcpy(extracted, str, i);
			extracted[i] = '\0';
		}
	}
	return extracted;
}

/* xmalloc */
char *insert(const char *str, char c, size_t pos)
{
	char *new_str = NULL;
	size_t len = strlen(str);

	if(pos <= len) {
		new_str = (char*) xmalloc(len + 2);
		memcpy(new_str, str, pos);
		new_str[pos] = c;
		memcpy(new_str + pos + 1, str + pos, len - pos + 1);
	}

	return new_str;
}

/* xmalloc */
char *insert_str(const char *str, const char *ins, size_t pos)
{
	size_t len1, len2;
	char *new_str = NULL;

	len1 = strlen(str);
	if(pos <= len1) {
		len2 = strlen(ins);
		new_str = xmalloc(len1 + len2 + 1);
		memcpy(new_str, str, pos);
		memcpy(new_str + pos, ins, len2);
		memcpy(new_str + pos + len2, str + pos, len1 - pos + 1);
	}
	return new_str;
}

char *erase(const char *str, size_t pos)
{
	char *new_str = NULL;
	size_t len = strlen(str);

	if(pos < len) {
		new_str = (char*) xmalloc(len);
		memcpy(new_str, str, pos);
		memcpy(new_str + pos, str + pos + 1, len - pos);
	}
	return new_str;
}

char *erase_str(const char *str, size_t pos, size_t len)
{
	char *new_str = NULL;
	size_t len2 = strlen(str), new_len = len2 - len + 1;

	if(pos + len <= len2) {
		new_str = (char*) xmalloc(new_len);
		memcpy(new_str, str, pos);
		memcpy(new_str + pos, str + pos + len, new_len - pos);
	}
	return new_str;
}

/* xmalloc */
char *const_append(const char *str1, const char *str2)
{
	char *newStr = (char*) NULL;
	size_t len1, len2;

	len1 = strlen(str1);
	len2 = strlen(str2);
	newStr = (char*) xmalloc(len1 + len2 + 1);
	memcpy(newStr, str1, len1);
	memcpy(newStr + len1, str2, len2 + 1);
	return newStr;
}

/* xrealloc */
char *append(char *str1, const char *str2)
{
	size_t len1, len2;
#ifdef __MANAGE_MEM__
	if( ! is_allocated(str1))
		return const_append(str1, str2);
#endif
	len1 = strlen(str1);
	len2 = strlen(str2);
	str1 = (char*) xrealloc(str1, len1 + len2 + 1);
	memcpy(str1 + len1, str2, len2 + 1);
	return str1;
}

/* strchr */
ssize_t rev_strcspn(const char *str, const char *needle)
{
	ssize_t i, save = (ssize_t) -1;

	for(i = 0; str[i] != '\0'; i++)
		if(strchr(needle, str[i]))
			save = i;
	return save;
}

char *neg_strchr(const char *s, int c)
{
	while(*s++ == c);
	if(*s == '\0')
		return (char*) NULL;
	else
		return (char*) s;
}

void str_tolower(char *str)
{
	for(; *str != '\0'; str++)
		if(*str >= 'A' && *str <= 'Z')
			*str = *str + 32;	/* - 'A' + 'a' */
}

void str_toupper(char *str)
{
	for(; *str != '\0'; str++)
		if(*str >= 'a' && *str <= 'z')
			*str = *str - 32;	/* + 'A' - 'a' */
}

char *itoa(int n, char *buffer)
{
	char *ptr = buffer;
	int log;

	if(n < 0) {
		*ptr++ = '-';
		n = 0 - n;
	}

	for(log = n; log != 0; log /= 10)
		ptr++;

	for(*ptr = '\0'; n != 0; n /= 10)
		*--ptr = n % 10 + '0';

	return buffer;
}

void *initialize_vector(void *dest, const void *src, size_t size, size_t nmemb)
{
	int i;

	memcpy(dest, src, size);
	for(i = 1; i << 1 <= nmemb; i <<= 1)
		memcpy(dest + i * size, dest, i * size);
	memcpy(dest + i * size, dest, (nmemb - i) * size);
	return dest;
}

/* xmalloc, xrealloc */
char *readLine(FILE *stream)
{
	register unsigned i = 0;
	unsigned currentSize = 32;
	register char c;
	char *str = (char*) xmalloc(32);

#ifdef _POSIX_THREAD_SAFE_FUNCTIONS		/* locking and getc_unlocked functions */
	flockfile(stream);
	while((c = getc_unlocked(stream)) != EOF && c != '\n') {
		if(i == currentSize)
			str = (char*) xrealloc(str, currentSize <<= 1);
		str[i++] = c;
	}
	funlockfile(stream);
#else
	while((c = getc(stream)) != EOF && c != '\n') {
		if(i == currentSize)
			str = (char*) xrealloc(str, currentSize <<= 1);
		str[i++] = c;
	}
#endif
	if(i == currentSize)
		str = (char*) xrealloc(str, currentSize += 1);
	else if(c == EOF && i == 0) {
		free(str);
		return (char*) NULL;
	} else
		str = xrealloc(str, i+1);
	str[i] = '\0';
	return str;
}

/* xmalloc, xrealloc */
char *read_file_descriptor(int fd)
{
	unsigned currentSize = 32;
	ssize_t i = 0;
	char *str = (char*) xmalloc(32);

	/* read (currentSize - i) chars at a time, double currentSize and increment
	 * i by the number of characters read and repeat until no more characters
	 * are available */
	do {
		i += read(fd, str + i, currentSize - i);	
		if(i == currentSize)
			str = xrealloc(str, currentSize <<= 1);
	} while(i << 1 == currentSize);

	if(i == -1 || str[0] == 4) {	/* 4 == EOT (End Of Transmission, for sockets),
					 * see 'ascii' manpage for more details */
		free(str);
		return (char*) NULL;
	} else {
		/* remove all non-printable characters from end of string
		 * see 'isprint' manpage for more details */
		while(! isprint(str[i-1]))
			i--;
		/* allocate precisely as much memory (not a single byte more)
		 * as is needed to contain the data */
		if(i == currentSize)
			str = (char*) xrealloc(str, currentSize += 1);
		else	/* freeing space -> no need to check for NULL */
			str = xrealloc(str, i+1);
		str[i] = '\0';
	}
	return str;
}

/* xmalloc
 * returnArray is set to NULL if all chars in str are separator
 * returnArray and all elements in returnArray are dynamically allocated -> free them all when done */
size_t split_str(const char *str, const char separator, char ***returnArray)
{
	register int i;
	size_t count = 1;

	/* COMMENT NEXT 6 LINES TO NOT SKIP CONSECUTIVE separator
	 * CHARS AT START OF str */
	while(*str == separator)
		str++;
	if(*str == '\0') {
		*returnArray = (char**) NULL;
		return 0;
	}

	for(i = 0; str[i] != '\0'; i++)
		//count += (str[i] == separator);
		count += (str[i] == separator && str[i+1] != separator && str[i+1] != '\0');
	/* REPLACE PREVIOUS LINE WITH ABOVE COMMENTED LINE
	 * TO NOT SKIP OVER CONSECUTIVE SEPARATORS */

	*returnArray = (char**) xmalloc(count * sizeof(char*));

	for(count = i = 0; str[i] != '\0'; i++) {
		if(str[i] == separator) {
			/* COMMENT NEXT 3 LINES TO NOT SKIP OVER CONSECUTIVE SEPARATORS */
			if(i == 0)
				str++;
			else {
				(*returnArray)[count] = (char*) xmalloc(i+1);
				memcpy((*returnArray)[count], str, i);
				(*returnArray)[count++][i] = '\0';
				str += i+1;
			}	/* COMMENT THIS LINE TO NOT SKIP OVER CONSECUTIVE SEPARATORS */
			i = -1;
		}
	}
	if(i != 0) {
		(*returnArray)[count] = (char*) xmalloc(i+1);
		strcpy((*returnArray)[count++], str);
	}
	return count;
}

/* xmalloc
 * returnArray is set to NULL if all chars in str are separator
 * returnArray is dynamically allocated -> free when done */
size_t split_str_lite(char *str, const char separator, char ***returnArray)
{
	register int i;
	size_t count = 1;

	/* COMMENT NEXT 6 LINES TO NOT SKIP CONSECUTIVE separator
	 * CHARS AT START OF str */
	while(*str == separator)
		str++;
	if(*str == '\0') {
		*returnArray = (char**) NULL;
		return 0;
	}

	for(i = 0; str[i] != '\0'; i++)
		//count += (str[i] == separator);
		count += (str[i] == separator && str[i+1] != separator && str[i+1] != '\0');
	/* REPLACE PREVIOUS LINE WITH ABOVE COMMENTED LINE
	 * TO NOT SKIP OVER CONSECUTIVE SEPARATORS */

	*returnArray = (char**) xmalloc(count * sizeof(char*));

	for(count = i = 0; str[i] != '\0'; i++) {
		if(str[i] == separator) {
			/* COMMENT NEXT 3 LINES TO NOT SKIP OVER CONSECUTIVE SEPARATORS */
			if(i == 0)
				str++;
			else {
				(*returnArray)[count] = str;
				(*returnArray)[count++][i] = '\0';
				str += i+1;
			}	/* COMMENT THIS LINE TO NOT SKIP OVER CONSECUTIVE SEPARATORS */
			i = -1;
		}
	}
	if(i != 0)
		(*returnArray)[count] = str;
	return count;
}

#include <sys/stat.h>
int is_dir(char *path)
{
	struct stat buf;

	if(lstat(path, &buf) != 0) {
		perror("Error statting file ");
		return -1;
	} else
		return S_ISDIR(buf.st_mode);
}

#ifdef	_WIN32
#define FILE_SEPARATOR	'\\'
#else
#define FILE_SEPARATOR	'/'
#endif
/* xmalloc */
char *make_path(const char *old_path, const char *dir_name)
{
	char *new_path = (char*) NULL;
	int len1, len2;

	len1 = strlen(old_path);
	len2 = strlen(dir_name);
	new_path = (char*) xmalloc(len1 + len2 + 2);
	memcpy(new_path, old_path, len1);
	new_path[len1] = FILE_SEPARATOR;
	memcpy(new_path + len1 + 1, dir_name, len2 + 1);
	return new_path;
}
#undef FILE_SEPARATOR

#include <dirent.h>
void *dirwalk(const char *path, void* (*func)(void*, char*), void *arg)
{
	DIR *dir;
	struct dirent *entry;
	char *new_path;

	dir = opendir(path);
	if(dir == (DIR*) NULL)
		fprintf(stderr, "Error opening directory '%s' : %s.\n", path, strerror(errno));
	else {
		/* iterate over linked list. When we have examined all files in directory, readdir() returns NULL */
		while((entry = readdir(dir)) != (struct dirent*)NULL) {
			if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
				new_path = make_path(path, entry->d_name);
#ifdef _WIN32
				if(is_dir(new_path))
#else
				if(entry->d_type == DT_DIR)	/* windows sucks, this line works on unix-based OSs only */
#endif
					dirwalk(new_path, func, arg);
				else
					arg = func(arg, new_path);
				free(new_path);
			}
		}
		closedir(dir);
	}
	return arg;
}

void register_signal_handler(int signum, void (*sighandler)(int))
{
	struct sigaction new_sigaction;

	memset(&new_sigaction, 0, sizeof(struct sigaction));
	new_sigaction.sa_handler = sighandler;
	if(sigaction(signum, &new_sigaction, NULL) != 0) {
		perror("Error registering signal handler ");
		exit(EXIT_FAILURE);
	}
}

/* Don't bother trying to run the following on windows. You're wasting your time */
#ifndef _WIN32

/* Color management */

typedef struct {
	Color c;
	Color bgc;
	Style s;
} ColorEnv;

static ColorEnv __session__;

void set_style(Color c, Color bgc, Style s)
{
	printf("\x1B[%d;%dm", c + 30, s);
	__session__.c = c;
	__session__.s = s;
	printf("\x1B[%dm", bgc + 40);
	__session__.bgc = bgc;
}

void stylish_print(char *str, Color c, Color bgc, Style s)
{
	printf("\x1B[%d;%dm", c + 30, s);
	printf("\x1B[%dm%s\x1B[%dm", bgc + 40, str, __session__.bgc);
	printf("\x1B[%d;%dm", __session__.c + 30, __session__.s);
	fflush(stdout);
}

/* input character display */
void turn_echoing_off(void)
{
	struct termios term;

	if(tcgetattr(STDIN_FILENO, &term) != 0) {
		perror("Error manipulating terminal ");
		exit(EXIT_FAILURE);
	}
	term.c_lflag &= ~ECHO;
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) != 0) {
		perror("Error manipulating terminal ");
		exit(EXIT_FAILURE);
	}
}

void turn_echoing_on(void)
{
	struct termios term;

	if(tcgetattr(STDIN_FILENO, &term) != 0) {
		perror("Error manipulating terminal ");
		exit(EXIT_FAILURE);
	}
	term.c_lflag |= ECHO;
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) != 0) {
		perror("Error manipulating terminal ");
		exit(EXIT_FAILURE);
	}
}

/* program reads input without user having to press enter */
void instant_getchar(void)
{
	struct termios term;

	if(tcgetattr(STDIN_FILENO, &term) != 0) {
		perror("Error manipulating terminal ");
		exit(EXIT_FAILURE);
	}
	term.c_lflag &= ~ICANON;
	term.c_cc[VMIN] = 1;	
	term.c_cc[VTIME] = 0;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) != 0) {
		perror("Error manipulating terminal ");
		exit(EXIT_FAILURE);
	}
}

void normal_getchar(void)
{
	struct termios term;

	if(tcgetattr(STDIN_FILENO, &term) != 0) {
		perror("Error manipulating terminal ");
		exit(EXIT_FAILURE);
	}
	term.c_lflag |= ICANON;
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) != 0) {
		perror("Error manipulating terminal ");
		exit(EXIT_FAILURE);
	}
}

#endif

