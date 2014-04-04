CC=gcc
CFLAGS=-Iincludes -Wall -Wextra
ifeq ($(CC), gcc)
	CFLAGS += --short-enums
endif

BINARY=scheduler

all: CFLAGS += -Ofast
all: LDFLAGS +=-s
all: depend $(BINARY)

debug: CFLAGS += -g -DDEBUG
debug: depend $(BINARY)

$(BINARY): main.o utils.o random.o object.o FIFO.o linkedlist.o\
	stack.o heap.o process.o FCFS.o LCFS.o SJF.o roundrobin.o

depend:
	$(CC) -Iincludes -MM *.c > .depend

-include .depend

test: $(BINARY)
	./tester.sh

vgtest: $(BINARY)
	./tester.sh -v

show: $(BINARY)
	./tester.sh -s

vgshow: $(BINARY)
	./tester.sh -sv

compress: $(BINARY)
	gzexe $(BINARY) && $(RM) $(BINARY)~

decompress:
	test -f $(BINARY) && gzexe -d $(BINARY) && $(RM) $(BINARY)~ || $(MAKE)

.PHONY: clean distclean depend

clean:
	$(RM) *.o

distclean: clean
	$(RM) $(BINARY)
