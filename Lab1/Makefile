CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -ansi -DC99
ifeq ($(CC), gcc)
	CFLAGS += --short-enums
endif

BINARY=linker

all: CFLAGS += -Ofast
all: LDFLAGS += -s
all: depend $(BINARY)

debug: CFLAGS += -g -DDEBUG
debug: depend $(BINARY)

$(BINARY): utils.o token.o symbol.o instruction.o module.o

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
