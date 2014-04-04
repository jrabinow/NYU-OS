CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -Ofast -ansi -DC99
ifeq ($(CC), gcc)
	CFLAGS += --short-enums
endif
LDFLAGS=-s

all: depend linker

linker: utils.o token.o symbol.o instruction.o module.o

depend:
	gcc -MM *.c > .depend

-include .depend

check: linker
	./tester.sh

test: linker
	./tester.sh -oi

memtest: linker
	./tester.sh -mi

show: linker
	./tester.sh -o

memshow: linker
	./tester.sh -m

compress: linker
	test -f linker~ || gzexe linker

decompress:
	test -f linker~ && gzexe -d linker~ linker || make

.PHONY: clean distclean depend

clean:
	rm -f *.o linker~

distclean: clean
	rm -f linker
