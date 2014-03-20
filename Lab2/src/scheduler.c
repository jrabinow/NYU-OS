#include <scheduler.h>

static Scheduler new(const Builder bld);
static void delete(Scheduler sched);
static Scheduler clone(const Scheduler sched);
static void run(Scheduler sched, bool trace);
static void reap(Scheduler this, Process p);

static struct Scheduler_LT lt = {
	NULL,
	false,
	&new,
	&delete,
	&clone,
	NULL,
	NULL,
	NULL,
	&run,
	&reap
};

const struct Builder __Scheduler__ = {
	&__Object__,
	(Lookup_Table) &lt,
	sizeof(struct Scheduler),
	"Scheduler"
};

/* See object.c for explanation on builders, destructors and the like */
static Scheduler new(const Builder bld)
{
	Scheduler this;

	/* we underride functions BEFORE calling the super constructor, as this
	 * allows each object to inherit the function most specific to itself */
	if( ! bld->lt->lt_initialized) {
		if(bld->lt->new == NULL)
			bld->lt->new = (void *(*)(const Builder, ...)) &new;
		if(bld->lt->delete == NULL)
			bld->lt->delete = (void (*)(void*)) &delete;
		if(bld->lt->clone == NULL)
			bld->lt->clone = (void *(*)(const void*)) &clone;
		if(((Scheduler_LT) bld->lt)->run == NULL)
			((Scheduler_LT) bld->lt)->run = &run;
		if(((Scheduler_LT) bld->lt)->reap == NULL)
			((Scheduler_LT) bld->lt)->reap = &reap;
	}
	this = __Scheduler__.super->lt->new(bld);

#define new(builder, ...)	((__##builder##__).lt->new((const Builder) &__##builder##__, ##__VA_ARGS__))
	if(bld == &__Scheduler__)
		bld->lt->lt_initialized = true;
	this->reaped = new(FIFO);
	this->IO_blocking = new(FIFO);
	this->last_event = 0;
	this->CPU_use = this->IO_use = this->turnaround = this->cpu_waiting = this->throughput = 0;

	return this;
}

static void delete(Scheduler this)
{
	this->reaped->lt->delete(this->reaped);
	this->IO_blocking->lt->delete(this->IO_blocking);
	/* call super destructor to finish the job */
	__Scheduler__.super->lt->delete(this);
}

static Scheduler clone(const Scheduler this)
{
	Scheduler s = __Scheduler__.super->lt->clone(this);

	s->reaped = this->reaped->lt->clone(this->reaped);
	s->IO_blocking = this->IO_blocking->lt->clone(this->IO_blocking);

	/* copying individual primitive values is taken care of
	 * in super's clone method */
	return s;
}

static void run(Scheduler this, bool trace)
{
/*	Process p;
	int runtime, quantum;

	if(instance_of(this, RR_Scheduler)) {
		quantum = this->get_quantum(this);
		while((p = this->get_event(this)) != NULL) {
			runtime = myrandom(p->CB);
		}
	} else {
		puts("lol");
	}
	*/
}

static void reap(Scheduler this, Process p)
{
	/*
	Elem *e = (Elem*) xmalloc(;

	p->state = REAPED;
	if(this->add_reaped != NULL)
		this->add_reaped->next = e;
	this->add_reaped = e;
	if(this->reaped == NULL)
		this->reaped = this->add_reaped;
	*/
}

