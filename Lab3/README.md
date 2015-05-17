Lab3-Memory-Manager
===================

Memory-management unit lab for Professor Hubertus Franke's OS course

Compile:
-----------
$ make

Usage:
-----------
$ ./mmu [OPTION]... INPUT-FILE [RANDOM FILE]
Options:	-a=ARG	algorithm used by the Virtual Memory Manager
			ARG has the following specification: [ NlrfscCaA ]
			The default algorithm is LRU
		-f=ARG	Number of frames. The default number of frames is 32
		-h	print this help message
		-o=ARG	output format
			ARG has the following specification: [OPFS]{1,4}
