CC=gcc
CFLAGS=-Iincludes -Wall -Wextra
ifeq ($(CC), gcc)
	CFLAGS += --short-enums
endif

BINARY=vmm

all: CFLAGS += -Ofast
all: LDFLAGS += -s
all: depend $(BINARY)

debug: CFLAGS += -g -DDEBUG
debug: depend $(BINARY)

$(BINARY): main.o random.o linkedlist.o FIFO.o token.o utils.o object.o\
	NRU_VMM.o FIFO_VMM.o Random_VMM.o LRU_VMM.o VAging_VMM.o VClock_VMM.o\
	PAging_VMM.o PClock_VMM.o SecChance_VMM.o

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
