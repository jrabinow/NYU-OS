#include <SJF.h>

typedef struct SJF_Scheduler* SJF_Scheduler;

static Scheduler new(const Builder bld);
static void delete(Scheduler sched);
static Scheduler clone(const Scheduler sched);
static void print(const Scheduler sched);
static void put_event(Scheduler sched, Process proc);
static Process get_event(Scheduler sched);

static struct Scheduler_LT SJF_lt = {
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

const struct Builder __SJF_Scheduler__ = {
	&__Scheduler__,
	(Lookup_Table) &SJF_lt,
	sizeof(struct SJF_Scheduler),
	"SJF"
};

static Scheduler new(const Builder bld)
{
	SJF_Scheduler sched = __SJF_Scheduler__.super->lt->new(bld);

#define new(builder, ...)	((__##builder##__).lt->new((const Builder) &__##builder##__, ##__VA_ARGS__))
	if(bld == &__SJF_Scheduler__)
		bld->lt->lt_initialized = true;
	sched->heap = new(Heap);

	return (Scheduler) sched;
}

static void delete(Scheduler sched)
{
	SJF_Scheduler this = (SJF_Scheduler) sched;

	this->heap->lt->delete(this->heap);
	__SJF_Scheduler__.super->lt->delete(sched);
}

static Scheduler clone(const Scheduler sched)
{
	SJF_Scheduler this = (SJF_Scheduler) sched,
		      new_sched = __SJF_Scheduler__.super->lt->clone(sched);

	new_sched->heap = this->heap->lt->clone(this->heap);

	return (Scheduler) new_sched;
}

static void print(const Scheduler sched)
{
	SJF_Scheduler this = (SJF_Scheduler) sched;
	
	puts(this->lt->bld->name);
}

static Process get_event(Scheduler sched)
{
	SJF_Scheduler this = (SJF_Scheduler) sched;

	return (Process) this->heap->lt->get(this->heap);
}

static void put_event(Scheduler sched, Process proc)
{
	SJF_Scheduler this = (SJF_Scheduler) sched;

	this->heap->lt->put(this->heap, (Comparable) proc);
}

