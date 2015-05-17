#include "fsm.h"

/* read token into allocated buffer. No limit on token size */

static void skip_multiline_comment(File *in)
{
	bool in_comment = true;
	int c;

	while( in_comment ) {
		switch(fgetc(in->stream)) {
			case EOF:
				in_comment = false;
				break;
			case '\r':
				in->lineoffset = 1;
				break;
			case '\v':
			case '\f':
			case '\n':
				if((c = fgetc(in->stream)) != EOF) {
					in->linenum++;
					in->lineoffset = 1;
					ungetc(c, in->stream);
				}
				break;
			case '*':
				if((c = fgetc(in->stream)) == '/') {
					in_comment = false;
					in->lineoffset += 2;
				} else {
					ungetc(c, in->stream);
					in->lineoffset++;
				}
				break;
			default:
				in->lineoffset++;
		}
	}

}

static void skip_eol_comment(File *in)
{
	int c;
	bool in_comment = true;

	while(in_comment) {
		switch(c = fgetc(in->stream)) {
			case EOF:
				in_comment = false;
				break;
			case '\v':
			case '\f':
			case '\n':
				in_comment = false;
				if((c = fgetc(in->stream)) != EOF) {
					in->linenum++;
					in->lineoffset = 1;
					ungetc(c, in->stream);
				}
				break;
			default:
				in->lineoffset++;
		}
	}
}

static void swallow_whitespace(File *in)
{
	bool in_whitespace = true;
	int c;

	while(in_whitespace) {
		switch(c = fgetc(in->stream)) {
			case ' ':
			case '\t':
				in->lineoffset++;
				break;
			case '\r':
				in->lineoffset = 1;
				break;
			case '\v':
			case '\f':
			case '\n':
				if((c = fgetc(in->stream)) != EOF) {
					in->linenum++;
					in->lineoffset = 1;
					ungetc(c, in->stream);
				}
				break;
			default:
				in_whitespace = false;
				ungetc(c, in->stream);
		}
	}
}

static void read_string(Token *t, File *in)
{
	int c;
	bool in_string = true;

	while(in_string) {
		c = fgetc(in->stream);
		if(c != EOF && needs_more_mem(t))
			add_mem(t);
		switch(c) {
			case '"':
				t->token[t->size++] = '"';
				in->lineoffset++;
				in_string = false;
				break;
			case '\\':
				t->token[t->size++] = '\\';
				if((c = fgetc(in->stream)) == '"') {
					in->lineoffset += 2;
					t->token[t->size++] = '"';
				} else {
					in->lineoffset++;
					ungetc(c, in->stream);
				}
				break;
			case '\v':
			case '\f':
			case '\n':
				t->token[t->size++] = c;
				if((c = fgetc(in->stream)) != EOF) {
					in->lineoffset = 1;
					in->linenum++;
					ungetc(c, in->stream);
				}
				break;
			case EOF:
				in_string = false;
				break;
			default:
				t->token[t->size++] = c;
		}
	}
}

static void read_char(Token *t, File *in)
{
	int c;
	bool in_char = true;

	while(in_char) {
		c = fgetc(in->stream);
		if(c != EOF && needs_more_mem(t))
			add_mem(t);
		switch(c) {
			case '"':
				t->token[t->size++] = '"';
				in->lineoffset++;
				in_string = false;
				break;
			case '\\':
				t->token[t->size++] = '\\';
				if((c = fgetc(in->stream)) == '"') {
					in->lineoffset += 2;
					t->token[t->size++] = '"';
				} else {
					in->lineoffset++;
					ungetc(c, in->stream);
				}
				break;
			case '\n':
				t->token[t->size++] = '\n';
				if((c = fgetc(in->stream)) != EOF) {
					in->lineoffset = 1;
					in->linenum++;
					ungetc(c, in->stream);
				}
				break;
			case EOF:
				in_string = false;
				break;
			default:
				t->token[t->size++] = c;
		}
	}
}

static void read_identifier(Token *t, File *in)
{

}

static void read_number(Token *t

		Token read_token(File *in)
		{
		int c, tmp;
		bool reading_token = true;
		size_t i = 0;
		Token t;

		t.token = (char*) xmalloc(__TOKEN_MEM_SIZE__);

		while( reading_token ) {
		c = fgetc(in->stream);
		if(c != EOF && needs_more_mem(&t))
		add_mem(&t);
		switch(c) {
		case ' ':
		case '\t':
		case '\v':
		case '\r':
		case '\f':
		case '\n':
			ungetc(c, in->stream);
			swallow_whitespace(in);
			break;
		case '"':
			in->lineoffset++;
			t.token[t.size++] = '"';
			read_string(&t, in);
			reading_token = false;
			break;
		case '\'':
			in->lineoffset++;
			t.token[t.size++] = '\'';
			read_char(&t, in);
			reading_token = false;
		case '/':
			switch(c = fgetc(in->stream)) {
				case '/':
					in->lineoffset += 2;
					skip_eol_comment(in);
					break;
				case '*':
					in->lineoffset += 2;
					skip_multiline_comment(in);
					break;
				case '=':
					in->lineoffset += 2;
					t.token[t.size++] = '/';
					if(needs_more_mem(&t))
						add_mem(&t);
					t.token[t.size++] = '=';
				case EOF:

				default:
					ungetc(c, in->stream);
			}
			reading_token = false;
			break;
		case '*':
		case '+':
		case '-':
			switch(c = fgetc(in->stream)) {
				case '-':
				case '=':
				case '>':
				default:
					tmp = c;
					if(isdigit(c) || c == '.' && isdigit((c = fgetc(in->stream)))
							}
							break;
							case '%':
							case '&':
							switch(c = fgetc(in->stream)) {

							}
							case '|':
							case '^':
							case '!':
							case '<':
							case '>':
							case '.':
							default:
							reading_token = false;
							if(c == '_' || isalpha(c)) {
							ree
							} else if(isalnum(c) ) {

							} else if(c != EOF) {

							}
		}
		}

		if(c == EOF) {
			free(t.token);
			t.token = NULL;
			t.size = 0;
		} else {
			if(needs_more_mem(&t))
				add_mem(&t);
			t.token[t.size] = '\0';
		}
		return t;
		}

Token get_statement(File *in)
{
	Token t = read_token(in);

}

bool is_valid_token(Token *t)
{
	return t != NULL && t->token != NULL;
}

void delete_token(Token *t)
{
	if(t != NULL && t->token != NULL)
		free(t->token);
}

