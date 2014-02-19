CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -O2 -DC99
ifeq ($(CC), gcc)
	CFLAGS += --short-enums
endif
LDFLAGS=-s

linker: utils.o token.o symbol.o instruction.o module.o

test: linker
	./tester.sh

vgtest: linker
	./tester.sh -v

show: linker
	./tester.sh -s

vgshow: linker
	./tester.sh -sv

compress: linker
	test -f linker~ || gzexe linker

decompress:
	test -f linker~ && gzexe -d linker~ linker || make

.PHONY: clean distclean

clean:
	rm -f *.o linker~

distclean: clean
	rm -f linker
