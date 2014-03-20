#include <linkedlist.h>

typedef struct Elem {
	Object obj;
	struct Elem *next;
} Elem;

static LinkedList new(const Builder);
static void delete(LinkedList);
static LinkedList clone(LinkedList);
static void put(LinkedList, Object);
static Object get(LinkedList);
static Object peek(LinkedList);
static void *apply(LinkedList this, void *accu, void *(*func)(Object, void*));

static struct LinkedList_LT lt = {
	NULL,
	false,
	&new,
	&delete,
	&clone,
	NULL,
	&put,
	&get,
	&peek,
	&apply
};

const struct Builder __LinkedList__ = {
	&__Object__,
	(Lookup_Table) &lt,
	sizeof(struct LinkedList),
	"LinkedList"
};

static LinkedList new(const Builder bld)
{
	LinkedList l = __LinkedList__.super->lt->new(bld);

	if(bld == &__LinkedList__)
		bld->lt->lt_initialized = true;
	l->head = NULL;
	return l;
}

static void delete(LinkedList this)
{
	Elem *tmp;
	while((tmp = this->head) != NULL) {
		this->head = this->head->next;
		if(tmp->obj != NULL)
			tmp->obj->lt->delete(tmp->obj);
		free(tmp);
	}
	__LinkedList__.super->lt->delete(this);
}

static LinkedList clone(LinkedList this)
{
	Elem *tmp, *iterator = this->head;
	LinkedList l = __LinkedList__.super->lt->clone(this);

	if(this->head != NULL) {
		l->head = tmp = (Elem*) xmalloc(sizeof(Elem));
		tmp->obj = iterator->obj != NULL ? iterator->obj->lt->clone(iterator->obj) : NULL;

		while((iterator = iterator->next) != NULL) {
			tmp->next = (Elem*) xmalloc(sizeof(Elem));
			tmp->next->obj = iterator->obj != NULL ?
				iterator->obj->lt->clone(iterator->obj) : NULL;
			tmp = tmp->next;
		}
		tmp->next = NULL;
	}
	return l;
}

static void put(LinkedList this, Object obj)
{
	Elem *e = (Elem*) xmalloc(sizeof(Elem));
	e->obj = obj;
	e->next = this->head;
	this->head = e;
}

static Object get(LinkedList this)
{
	Elem *tmp = this->head;
	Object ret = tmp->obj;

	this->head = tmp->next;
	free(tmp);
	return ret;
}

static Object peek(LinkedList this)
{
	return this->head->obj;
}

static void *apply(LinkedList this, void *accu, void *(*func)(Object, void*))
{
	Elem *tmp = this->head;

	while(tmp != NULL) {
		accu = func(tmp->obj, accu);
		tmp = tmp->next;
	}

	return accu;
}
