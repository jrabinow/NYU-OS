#include <process.h>

static Process new(const Builder bld, pid_t pid, int arrival_time, int cpu_time, int cpu_burst, int io_burst);
static void print(const Process p);

static struct Lookup_Table lt = {
	NULL,
	false,
	(void *(*)(const Builder, ...)) &new,
	NULL,
	NULL,
	(void (*)(const void*)) &print
};

const struct Builder __Process__ = {
	&__Object__,			/* Process implements Comparable, but Comparable is a virtual class
					 * Therefore, Process descends from Object */
	&lt,
	sizeof(struct Process),
	"Process"
};

static Process new(const Builder bld, pid_t pid, int arrival_time, int cpu_time, int cpu_burst, int io_burst)
{
	Process p = __Process__.super->lt->new(bld);

	if(bld == &__Process__)
		bld->lt->lt_initialized = true;
	p->pid = pid;
	p->AT = arrival_time;
	p->TC = cpu_time;
	p->CB = cpu_burst;
	p->IO = io_burst;
	p->FT = p->IT = p->CW = 0;
	p->state = READY;

	return p;
}

static void print(const Process p)
{
	printf("%04d:\t%d\t%d\t%d\t%d | %d\t%d\t%d\t%d\n", p->pid, p->AT, p->TC, p->CB, p->IO, p->FT, p->FT - p->AT, p->IT, p->CW);
}

