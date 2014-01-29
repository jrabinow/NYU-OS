#include "token.h"

/* read token into allocated buffer. No limit on token size
 * Bizarre code behaviour for EOF: getchar() will always read a newline character
 * right before newline. A lot of code here is just to make sure lineoffset and linenum
 * are updated properly anyways */
Token *read_token(FILE *stream)
{
	int c, i = 0, mem_size = 16;
	Token *t = (Token*) xmalloc(sizeof(Token));

	t->token = (char*) xmalloc(mem_size);

	/* swallow whitespace */
	while((c = fgetc(stream)) == ' ' || c == '\t' || c == '\n')
		switch(c) {
			case ' ':
			case '\t':
				lineoffset++;
				break;
			case '\n':
				if((c = fgetc(stream)) != EOF) {
					linenum++;
					lineoffset = 1;
					ungetc(c, stream);
				}
		}
	t->linenum = linenum;
	t->lineoffset = lineoffset;
	
	if(c == EOF) {
		free(t->token);
		t->token = NULL;
		t->size = 0;
		return t;
	}

	t->linenum = linenum;
	t->lineoffset = lineoffset;
	do {
		t->token[i++] = c;
		if(i == mem_size)
			t->token = (char*) xrealloc(t->token, mem_size <<= 1);
	} while((c = fgetc(stream)) != EOF && c != ' ' && c != '\t' && c != '\n');

	ungetc(c, stream);
	lineoffset += i;
	t->token[i] = '\0';
	t->size = i;
	t->token = (char*) realloc(t->token, i+1);	/* adjust allocated memory to correct size */
	return t;
}

bool is_valid_token(Token *t)
{
	return t != NULL && t->token != NULL;
}

bool is_valid_int(Token *t)
{
	int i;

	if( ! is_valid_token(t))
		return false;

	for(i = 0; t->token[i] != '\0'; i++)
		if( ! isdigit(t->token[i]))
			return false;
	return true;
}

void delete_token(Token *t)
{
	if(t != NULL) {
		if(t->token != NULL)
			free(t->token);
		free(t);
	}
}

