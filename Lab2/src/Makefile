CC=gcc
CFLAGS=-Iincludes -Wall -Wextra
ifeq ($(CC), gcc)
	CFLAGS += --short-enums
endif

all: CFLAGS += -Ofast
all: LDFLAGS +=-s
all: depend scheduler

debug: CFLAGS += -g -DDEBUG
debug: depend scheduler

scheduler: main.o utils.o random.o object.o FIFO.o linkedlist.o\
	stack.o heap.o process.o FCFS.o LCFS.o SJF.o roundrobin.o

depend:
	$(CC) -Iincludes -MM *.c > .depend

-include .depend

test: scheduler
	./tester.sh

vgtest: scheduler
	./tester.sh -v

show: scheduler
	./tester.sh -s

vgshow: scheduler
	./tester.sh -sv

compress: scheduler
	test -f scheduler~ || gzexe scheduler

decompress:
	test -f scheduler~ && gzexe -d scheduler~ && $(RM) scheduler~ || $(MAKE)

.PHONY: clean distclean depend

clean:
	rm -f *.o

distclean: clean
	rm -f scheduler scheduler~
