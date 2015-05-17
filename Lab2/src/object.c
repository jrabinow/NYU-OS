/*
 *  Copyright (C) 2014 Julien Rabinow <jnr305@nyu.edu>
 *
 *  This file is part of Lab2-Scheduler.
 *
 *  Lab2-Scheduler is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Lab2-Scheduler is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Lab2-Scheduler. If not, see <http://www.gnu.org/licenses/>.
 */

#include <object.h>

static void *new(const Builder bld, ...);
static void *clone(const void *obj);
static char *to_string(const void *obj);

struct Lookup_Table lt = {
	NULL,		/* pointer to builder. We define the Object builder
			 * later on in the code, so we set it to NULL for now */
	true,		/* lt_initialized boolean. Lookup table methods need
			 * underwriting only once, so once an object of a class
			 * has been initialized a first time, we set this to
			 * true to indicate that there is no need to go through
			 * the entire underwriting process again */
	&new,
	&free,		/* Destructor. In this case, new(Object) only allocates
			 * some memory and initializes some values => we can
			 * just call free on the object and it will be destroyed
			 * without any memory leak */
	&clone,
	&to_string
};

const struct Builder __Object__ = {
	NULL,		/* pointer to "super" builder. Everything descends from
			 * objects (objects don't descend from anything so in
			 * this case it's NULL */
	&lt,		/* lookup table */
	sizeof(Object),	/* size of object. */
	"Object"	/* string of type */
};

static void *new(const Builder bld, ...)
{
	Object o = (Object) xmalloc(bld->obj_size);

	o->lt = (Lookup_Table) bld->lt;			/* lookup table */
	/* "formally" sets object type (can now be recognized by instance_of(),
	 * see below) */

	if( ! bld->lt->lt_initialized) {
		o->lt->bld = bld;
		/* functions are not automatically inherited between types, we
		 * have to manually set pointer values. However we must make
		 * sure there is no other function pointer before overwriting
		 * pointers in the lookup table */
		if(bld->lt->new == NULL)		
			bld->lt->new = &new;	
		if(bld->lt->delete == NULL)
			bld->lt->delete = &free;
		if(bld->lt->clone == NULL)
			bld->lt->clone = (void *(*)(const void*)) &clone;
		if(bld->lt->to_string == NULL)
			bld->lt->to_string = (char *(*)(const void*)) &to_string;
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

static char *to_string(const void *obj)
{
	char *str = NULL;

	if(asprintf(&str, "%s: %p", ((Object) obj)->lt->bld->name, obj) == -1) {
		perror("Error allocating memory ");
		exit(EXIT_FAILURE);
	} else
		return str;
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
