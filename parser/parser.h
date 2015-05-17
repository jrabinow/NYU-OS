#ifndef PARSER_H
#define PARSER_H

#define MAX_VAR_NAME_LEN	32

#include "utils.h"
/*#include <sys/mman.h>
#include <unistd.h> */

typedef struct {
	const char *classname;
	const char *super;
} ClassDef;

typedef struct {
	const char *path, *mode;
	FILE *stream;
	int linenum, lineoffset;
} File;

#include "fsm.h"

ClassDef get_class_def(File *f);
void init_file(File *f, const char *path, const char *mode);
bool is_valid_classname(char *name);

#endif	/* ifndef PARSER_H */
