CC=gcc
SRCDIR=../src
TESTDIR=../tests
INCLUDES=$(addprefix $(SRCDIR), /includes)
CFLAGS=$(addprefix -I, $(INCLUDES)) -Wall -Wextra -DC99
ifeq ($(CC), gcc)
	CFLAGS += --short-enums
endif

BINARY=scheduler

all: CFLAGS += -Ofast -fomit-frame-pointer
all: LDFLAGS += -s
all: depend $(BINARY)

debug: CFLAGS += -g -DDEBUG
debug: depend $(BINARY)

$(BINARY): main.o utils.o random.o object.o FIFO.o linkedlist.o\
	stack.o heap.o process.o FCFS.o LCFS.o SJF.o roundrobin.o

.SUFFIXES: .h

.h:
	$(CC) $(CFLAGS) $^

depend:
	$(CC) $(CFLAGS) -MM $(SRCDIR)/*.c > .depend

-include .depend

check: $(BINARY)
	$(TESTDIR)/tester.sh

test: $(BINARY)
	$(TESTDIR)/tester.sh output

memtest: $(BINARY)
	$(TESTDIR)/tester.sh memory

test-suppress: $(BINARY)
	$(TESTDIR)/tester.sh -s output

memtest-suppress: $(BINARY)
	$(TESTDIR)/tester.sh -s memory

compress: $(BINARY)
	gzexe $(BINARY) && $(RM) $(BINARY)~

decompress:
	test -f $(BINARY) && gzexe -d $(BINARY) && $(RM) $(BINARY)~ || $(MAKE)

.PHONY: clean distclean depend check test memtest test-suppress memtest-suppress compress\
	decompress

clean:
	$(RM) *.o $(addsuffix /*.gch, $(INCLUDES)) $(BINARY)~

distclean: clean
	$(RM) $(BINARY)

vpath %.o $(SRCDIR)
vpath %.c $(SRCDIR)
