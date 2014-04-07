all: linker

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
	cd Lab1/ && $(MAKE) $(RECIPE)

scheduler:
	cd Lab2/src/ && $(MAKE) $(RECIPE)

mmu:
	cd Lab3/src/ && $(MAKE) $(RECIPE)

io_sched:
	cd Lab4/src/ && $(MAKE) $(RECIPE)

.PHONY: linker scheduler mmu io_sched clean distclean check test memtest show\
	memshow compress decompress

