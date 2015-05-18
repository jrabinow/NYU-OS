CC=gcc
SRCDIR=../src
TESTDIR=../tests
INCLUDES=$(addprefix $(SRCDIR), /includes)
CFLAGS=$(addprefix -I, $(INCLUDES)) -Wall -Wextra -march=native
ifeq ($(CC), gcc)
	GCCVERSION := $(shell $(CC) -dumpversion|cut -d. -f1,2)
	HAS_OFAST := $(shell echo '$(GCCVERSION) >= 4.6' | bc)
	CFLAGS += --short-enums
debug:  CFLAGS += -Og
endif
ifeq ($(CC), clang)
	HAS_OFAST := 1
endif

ifeq ($(HAS_OFAST), 1)
all:	CFLAGS += -Ofast
else
all:	CFLAGS += -O3
endif

BINARY=mmu

all: LDFLAGS += -s
all: depend $(BINARY)

debug: CFLAGS += -g -DDEBUG -ggdb
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
