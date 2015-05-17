#include "parser.h"

int main(int argc, char *argv[])
{
	FILE *header_out, *source_out;
	File header, source;
	ClassDef classdef;
	
	if(argc != 3) {
		fprintf(stderr, "Usage %s header-file.h source-file.c\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	init_file(&header, argv[1], "r");
	init_file(&source, argv[2], "r");

	classdef = get_class_def(&source);
	printf("%s implements %s\n", classdef.classname, classdef.super);

	/* cleanup */
	free((void*) classdef.classname);
	free((void*)classdef.super);
	fclose(header.stream);
	fclose(source.stream);

	return 0;
}

ClassDef get_class_def(File *f)
{
	bool got_semicolon = false;
	ClassDef classdef;
	Token t = read_token(f);

	check_token(&t, f, "class", "class declaration");
	delete_token(&t);

	t = read_token(f);
	if(! is_valid_classname(t.token)) {
		fprintf(stderr, "%s:%d:%d: error: expected class declaration before '%s'\n",
				f->path, t.linenum, t.lineoffset, t.token == NULL ? "EOF" : t.token);
		exit(EXIT_FAILURE);
	}
	classdef.classname = t.token;
	
	t = read_token(f);
	check_token(&t, f, "implements", "'implements'");
	delete_token(&t);

	t = read_token(f);
	if(t.token[t.size - 1] == ';') {
		t.token[t.size - 1] = '\0';
		got_semicolon = true;
	}
	if(! is_valid_classname(t.token)) {
		fprintf(stderr, "%s:%d:%d: error: expected class declaration before '%s'\n",
				f->path, t.linenum, t.lineoffset, t.token == NULL ? "EOF" : t.token);
		exit(EXIT_FAILURE);
	}
	classdef.super = t.token;

	if( ! got_semicolon)
		if((t = get_statement(f)).token != NULL)
			free(t.token);

	return classdef;
}

void check_token(Token *t, File *f, char *expected_data, char *err_msg)
{
	if(t->token == NULL) {
		fprintf(stderr, "%s: error: reached end-of-file\n", f->path);
		exit(EXIT_FAILURE);
	}
	if(! equals(t->token, expected_data)) {
		fprintf(stderr, "%s:%d:%d: error: expected %s before '%s' \n",
				f->path, t->linenum, t->lineoffset, err_msg, t->token);
		exit(EXIT_FAILURE);
	}
}

void init_file(File *f, const char *path, const char *mode)
{
	f->stream = xfopen(path, mode);
	f->path = path;
	f->mode = mode;
	f->linenum = f->lineoffset = 1;
}

bool is_valid_classname(char *name)
{
	size_t len;

	if(name == NULL)
		return false;
	if( ! isalpha(name[0]) && name[0] != '_')
		return false;
	for(len = 1; name[len] != '\0'; len++)
		if(len > MAX_VAR_NAME_LEN)
			return false;
		else if( ! isalnum(name[len]) && name[len] != '_')
			return false;
	return true;
}
