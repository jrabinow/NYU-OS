#include <stack.h>

typedef struct Elem {
	Object obj;
	struct Elem *next;
} Elem;

static Stack new(const Builder);
static void delete(Stack);
static Stack clone(Stack);
static void push(Stack, Object);
static Object pop(Stack);
static Object peek(Stack);

static struct Stack_LT lt = {
	NULL,
	false,
	&new,
	&delete,
	&clone,
	NULL,
	&push,
	&pop,
	&peek
};

const struct Builder __Stack__ = {
	&__Object__,
	(Lookup_Table) &lt,
	sizeof(struct Stack),
	"Stack"
};

static Stack new(const Builder bld)
{
	Stack s = __Stack__.super->lt->new(bld);

	if(bld == &__Stack__)
		bld->lt->lt_initialized = true;
	s->sp = NULL;
	return s;
}

static void delete(Stack this)
{
	Elem *tmp;
	while((tmp = this->sp) != NULL) {
		this->sp = this->sp->next;
		if(tmp->obj != NULL)
			tmp->obj->lt->delete(tmp->obj);
		free(tmp);
	}
	__Stack__.super->lt->delete(this);
}

static Stack clone(Stack this)
{
	Elem *tmp, *iterator = this->sp;
	Stack s = __Stack__.super->lt->clone(this);

	if(iterator != NULL) {
		s->sp = tmp = (Elem*) xmalloc(sizeof(Elem));
		tmp->obj = iterator->obj != NULL ? iterator->obj->lt->clone(iterator->obj) : NULL;

		while((iterator = iterator->next) != NULL) {
			tmp->next = (Elem*) xmalloc(sizeof(Elem));
			tmp->next->obj = iterator->obj != NULL ?
				iterator->obj->lt->clone(iterator->obj) : NULL;
			tmp = tmp->next;
		}
		tmp->next = NULL;
	}
	return s;
}

static void push(Stack this, Object obj)
{
	Elem *e = (Elem*) xmalloc(sizeof(Elem));

	e->obj = obj;
	e->next = this->sp;
	this->sp = e;
}

static Object pop(Stack this)
{
	Elem *tmp = this->sp;
	Object ret = NULL;

	if(tmp != NULL) {
		ret = tmp->obj;
		this->sp = tmp->next;
		free(tmp);
	}
	return ret;
}

static Object peek(Stack this)
{
	return this->sp == NULL ? NULL : this->sp->obj;
}

