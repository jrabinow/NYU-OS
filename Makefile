all: linker mmu

debug: RECIPE = debug
debug: all

clean: RECIPE = clean
clean: all

distclean: RECIPE = distclean
distclean: all

check: RECIPE = check
check: all

test: RECIPE = test
test: all

memtest: RECIPE = memtest
memtest: all

show: RECIPE = show
show: all

memshow: RECIPE = memshow
memshow: all

compress: RECIPE = compress
compress: all

decompress: RECIPE = decompress
decompress: all

linker:
	$(MAKE) -C Lab1/build $(RECIPE)

scheduler:
	$(MAKE) -C Lab2/build $(RECIPE)

mmu:
	$(MAKE) -C Lab3/build $(RECIPE)

io_sched:
	$(MAKE) -C Lab4/build $(RECIPE)

.PHONY: all linker scheduler mmu io_sched clean distclean check test memtest\
	show memshow compress decompress

