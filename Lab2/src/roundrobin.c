#include <roundrobin.h>

static Scheduler new(const Builder bld, int quantum);
static void delete(Scheduler sched);
static Scheduler clone(const Scheduler sched);
static void print(const Scheduler sched);
static void put_event(Scheduler sched, Process proc);
static Process get_event(Scheduler sched);
static int get_quantum(const Scheduler sched);
static void set_quantum(Scheduler sched, int quantum);

static struct RR_Scheduler_LT RR_lt = {
	NULL,
	false,
	&new,
	&delete,
	&clone,
	&print,
	&get_event,
	&put_event,
	NULL,
	NULL,
	&get_quantum,
	&set_quantum
};

const struct Builder __RR_Scheduler__ = {
	&__Scheduler__,
	(Lookup_Table) &RR_lt,
	sizeof(struct RR_Scheduler),
	"RR"
};


static Scheduler new(const Builder bld, int quantum)
{
	RR_Scheduler sched = __RR_Scheduler__.super->lt->new(bld);

#define new(builder, ...)	((__##builder##__).lt->new((const Builder) &__##builder##__, ##__VA_ARGS__))
	if(bld == &__RR_Scheduler__)
		bld->lt->lt_initialized = true;
	sched->ready_queue = new(FIFO);
	sched->quantum = quantum;

	return (Scheduler) sched;
}

static void delete(Scheduler sched)
{
	RR_Scheduler this = (RR_Scheduler) sched;

	this->ready_queue->lt->delete(this->ready_queue);
	__RR_Scheduler__.super->lt->delete(sched);
}

static Scheduler clone(const Scheduler sched)
{
	RR_Scheduler this = (RR_Scheduler) sched,
		     s = __RR_Scheduler__.super->lt->clone(sched);

	s->ready_queue = this->ready_queue->lt->clone(this->ready_queue);

	return (Scheduler) s;
}

static void print(const Scheduler sched)
{
	RR_Scheduler this = (RR_Scheduler) sched;

	printf("%s %d\n", this->lt->bld->name, this->quantum);
}

static Process get_event(Scheduler sched)
{
	RR_Scheduler this = (RR_Scheduler) sched;

	return (Process) this->ready_queue->lt->get(this->ready_queue);
}

static void put_event(Scheduler sched, Process proc)
{
	RR_Scheduler this = (RR_Scheduler) sched;

	this->ready_queue->lt->put(this->ready_queue, (Object) proc);
}

static int get_quantum(const Scheduler this)
{
	return ((RR_Scheduler) this)->quantum;
}

static void set_quantum(Scheduler this, int quantum)
{
	((RR_Scheduler) this)->quantum = quantum;
}
