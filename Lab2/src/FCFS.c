#include <FCFS.h>

static Scheduler new(const Builder bld);
static void delete(Scheduler sched);
static Scheduler clone(const Scheduler sched);
static void print(const Scheduler sched);
static void put_event(Scheduler sched, Process proc);
static Process get_event(Scheduler sched);

static struct Scheduler_LT FCFS_lt = {
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

const struct Builder __FCFS_Scheduler__ = {
	&__Scheduler__ ,
	(Lookup_Table) &FCFS_lt,
	sizeof(struct FCFS_Scheduler),
	"FCFS"
};

static Scheduler new(const Builder bld)
{
	FCFS_Scheduler this =  __FCFS_Scheduler__.super->lt->new(bld);

#define new(builder, ...)	((__##builder##__).lt->new((const Builder) &__##builder##__, ##__VA_ARGS__))
	if(bld == &__FCFS_Scheduler__)
		bld->lt->lt_initialized = true;
	this->ready_queue = new(FIFO);
	return (Scheduler) this;
}

static void delete(Scheduler sched)
{
	FCFS_Scheduler this = (FCFS_Scheduler) sched;

	this->ready_queue->lt->delete(this->ready_queue);
	__FCFS_Scheduler__.super->lt->delete(sched);
}

static Scheduler clone(const Scheduler sched)
{
	FCFS_Scheduler this = (FCFS_Scheduler) sched,
		       s = __FCFS_Scheduler__.super->lt->clone(sched);

	s->ready_queue = this->ready_queue->lt->clone(this->ready_queue);

	return (Scheduler) s;
}

static void print(const Scheduler sched)
{
	FCFS_Scheduler this = (FCFS_Scheduler) sched;

	puts(this->lt->bld->name);
}

static Process get_event(Scheduler sched)
{
	FCFS_Scheduler this = (FCFS_Scheduler) sched;

	return (Process) this->ready_queue->lt->get(this->ready_queue);
}

static void put_event(Scheduler sched, Process proc)
{
	FCFS_Scheduler this = (FCFS_Scheduler) sched;

	this->ready_queue->lt->put(this->ready_queue, (Object) proc);
}

