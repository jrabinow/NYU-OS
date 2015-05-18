CC=gcc
SRCDIR=../src
TESTDIR=../tests
INCLUDES=$(addprefix $(SRCDIR), /includes)
CFLAGS=$(addprefix -I, $(INCLUDES)) -Wall -Wextra -Wpedantic -ansi -march=native
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

BINARY=linker

all: LDFLAGS += -s
all: depend $(BINARY)

debug: CFLAGS += -g -DDEBUG -ggdb
debug: depend $(BINARY)

$(BINARY): utils.o token.o symbol.o instruction.o module.o

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
