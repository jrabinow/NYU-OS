CC=gcc
SRCDIR=../src
TESTDIR=../tests
INCLUDES=$(addprefix $(SRCDIR), /includes)
CFLAGS=$(addprefix -I, $(INCLUDES)) -Wall -Wextra
ifeq ($(CC), gcc)
	CFLAGS += --short-enums
endif

BINARY=mmu

all: CFLAGS += -Ofast -fomit-frame-pointer
all: LDFLAGS += -s
all: depend $(BINARY)

debug: CFLAGS += -g -DDEBUG
debug: depend $(BINARY)

$(BINARY): main.o random.o FIFO.o utils.o object.o vmm.o NRU_VMM.o FIFO_VMM.o\
	Random_VMM.o LRU_VMM.o VAging_VMM.o VClock_VMM.o PAging_VMM.o\
	PClock_VMM.o SecChance_VMM.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

.SUFFIXES: .h

.h:
	$(CC) $(CFLAGS) $^

depend:
	$(CC) $(CFLAGS) -MM $(SRCDIR)/*.c > .depend

-include .depend

check: $(BINARY)
	$(TESTDIR)/tester.sh

test: $(BINARY)
	$(TESTDIR)/tester.sh -oi

memtest: $(BINARY)
	$(TESTDIR)/tester.sh -mi

show: $(BINARY)
	$(TESTDIR)/tester.sh -o

memshow: $(BINARY)
	$(TESTDIR)/tester.sh -m

compress: $(BINARY)
	gzexe $(BINARY) && $(RM) $(BINARY)~

decompress:
	test -f $(BINARY) && gzexe -d $(BINARY) && $(RM) $(BINARY)~ || $(MAKE)

.PHONY: clean distclean depend test memtest show memshow check compress\
	decompress

clean:
	$(RM) *.o $(addsuffix /*.gch, $(INCLUDES))

distclean: clean
	$(RM) $(BINARY)

vpath %.o $(SRCDIR)
vpath %.c $(SRCDIR)
