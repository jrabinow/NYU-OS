CC=gcc
CFLAGS=-Iincludes -Wall -Wextra
ifeq ($(CC), gcc)
	CFLAGS += --short-enums
endif

BINARY=scheduler

all: CFLAGS += -Ofast
all: LDFLAGS += -s
all: depend $(BINARY)

debug: CFLAGS += -g -DDEBUG
debug: depend $(BINARY)

$(BINARY): main.o utils.o random.o object.o FIFO.o linkedlist.o\
	stack.o heap.o process.o FCFS.o LCFS.o SJF.o roundrobin.o

depend:
	$(CC) -Iincludes -MM *.c > .depend

-include .depend

check: $(BINARY)
	./tester.sh

test: $(BINARY)
	./tester.sh -oi

memtest: $(BINARY)
	./tester.sh -mi

show: $(BINARY)
	./tester.sh -o

memshow: $(BINARY)
	./tester.sh -m

compress: $(BINARY)
	gzexe $(BINARY) && $(RM) $(BINARY)~

decompress:
	test -f $(BINARY) && gzexe -d $(BINARY) && $(RM) $(BINARY)~ || $(MAKE)

.PHONY: clean distclean depend

clean:
	$(RM) *.o

distclean: clean
	$(RM) $(BINARY)
