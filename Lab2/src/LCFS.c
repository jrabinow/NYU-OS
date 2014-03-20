#include <LCFS.h>

static Scheduler new(const Builder bld);
static void delete(Scheduler sched);
static Scheduler clone(const Scheduler sched);
static void print(const Scheduler sched);
static void put_event(Scheduler sched, Process proc);
static Process get_event(Scheduler sched);

static struct Scheduler_LT LCFS_lt = {
	NULL,
	false,
	&new,
	&delete,
	&clone,
	&print,
	&get_event,
	&put_event,
	NULL,
	NULL
};

const struct Builder __LCFS_Scheduler__ = {
	&__Scheduler__,
	(Lookup_Table) &LCFS_lt,
	sizeof(struct LCFS_Scheduler),
	"LCFS"
};

static Scheduler new(const Builder bld)
{
	LCFS_Scheduler sched = __LCFS_Scheduler__.super->lt->new(bld);

#define new(builder, ...)	((__##builder##__).lt->new((const Builder) &__##builder##__, ##__VA_ARGS__))
	if(bld == &__LCFS_Scheduler__)
		bld->lt->lt_initialized = true;
	sched->ready_queue = new(Stack);
	return (Scheduler) sched;
}

static void delete(Scheduler sched)
{
	LCFS_Scheduler this = (LCFS_Scheduler) sched;

	this->ready_queue->lt->delete(this->ready_queue);
	__LCFS_Scheduler__.super->lt->delete(this);
}

static Scheduler clone(const Scheduler sched)
{
	LCFS_Scheduler s = __LCFS_Scheduler__.super->lt->clone(sched),
		       this = (LCFS_Scheduler) sched;

	s->ready_queue = this->ready_queue->lt->clone(this->ready_queue);
	return (Scheduler) s;
}

static void print(const Scheduler sched)
{
	LCFS_Scheduler this = (LCFS_Scheduler) sched;

	puts(this->lt->bld->name);
}

static void put_event(Scheduler sched, Process proc)
{
	LCFS_Scheduler this = (LCFS_Scheduler) sched;
	this->ready_queue->lt->push(this->ready_queue, (Object) proc);
}

static Process get_event(Scheduler sched)
{
	LCFS_Scheduler this = (LCFS_Scheduler) sched;

	return (Process) this->ready_queue->lt->pop(this->ready_queue);
}

