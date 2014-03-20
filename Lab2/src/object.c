#include <object.h>

static void *new(const Builder bld, ...);
static void *clone(const void *obj);
static void print(const void *obj);

struct Lookup_Table lt = {
	NULL,					/* pointer to builder. We define the Object builder
						 * later on in the code, so we set it to NULL for now */
	true,					/* lt_initialized boolean. Lookup table methods need underwriting
						 * only once, so once an object of a class has been initialized
						 * a first time, we set this to true to indicate that there is no
						 * need to go through the entire underwriting process again */
	&new,
	&free,					/* Destructor. In this case, new(Object) only allocates some
						 * memory and initializes some values => we can just call free
						 * on the object and it will be destroyed without any memory leak */
	&clone,
	&print
};

const struct Builder __Object__ = {
	NULL,		/* pointer to "super" builder. Everything descends from objects
			 * (objects don't descend from anything so in this case it's NULL */
	&lt,		/* lookup table */
	sizeof(Object),	/* size of object. */
	"Object"	/* string of type */
};

static void *new(const Builder bld, ...)
{
	Object o = (Object) xmalloc(bld->obj_size);

	o->lt = (Lookup_Table) bld->lt;			/* lookup table */
	/* "formally" sets object type (can now be recognized by instance_of(), see below) */

	if( ! bld->lt->lt_initialized) {
		o->lt->bld = bld;
		if(bld->lt->new == NULL)		/* functions are not automatically inherited between types, we have to manually set pointer values */
			bld->lt->new = &new; 		/* However we must make sure there is no other function pointer before overwriting */
		if(bld->lt->delete == NULL)		/* pointers in the lookup table */
			bld->lt->delete = &free;
		if(bld->lt->clone == NULL)
			bld->lt->clone = (void *(*)(const void*)) &clone;
		if(bld->lt->print == NULL)
			bld->lt->print = (void (*)(const void*)) &print;
	}
	return o;
}

static void *clone(const void *obj)
{
	Object this = (Object) obj,
	       new_obj = (Object) xmalloc(this->lt->bld->obj_size);
	memcpy(new_obj, this, this->lt->bld->obj_size);
	return new_obj;
}

static void print(const void *obj)
{
	printf("%p\n", obj);
}

#undef instance_of
bool instance_of(const Object obj, const Builder bld)
{
	const struct Builder *obj_bld;

	if(obj == NULL)
		return false;

	obj_bld = obj->lt->bld;
	while(obj_bld != NULL) {
		if(obj_bld == bld)
			return true;
		else
			obj_bld = obj_bld->super;
	}
	return false;
}
