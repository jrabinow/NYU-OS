#include "token.h"
#include "parser.h"

void init_file(File *f, const char *path, const char *mode)
{
	f->stream = xfopen(path, mode);
	f->path = path;
	f->mode = mode;
	f->linenum = f->lineoffset = 1;
}

int main(int argc, char **argv)
{
	Token t;
	File f;
	init_file(&f, argv[1], "r");

	while((t = whitespace_delim_token(&f)).size != 0) {
		printf("%d,%d: %s\n", t.linenum, t.lineoffset, t.token);
		delete_token(&t);
	}
	fclose(f.stream);
	return 0;
}
