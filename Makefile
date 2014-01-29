CFLAGS=-Wall -Wextra -Ofast --short-enums
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

clean:
	rm -f *.o

distclean: clean
	rm -f linker
