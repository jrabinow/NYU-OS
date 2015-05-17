#!/bin/bash

#example ./test1.sh in60 8 options

INFILE=$1
FRAMES=$2
shift 2
OPTIONS=$*     

ALGOS="N l r f s c C a A"
for X in ${ALGOS}; do 
	OUTF="./out_$(basename ${INFILE})_${X}_${FRAMES}_${OPTIONS}"
	echo "./mmu -a${X} -f${FRAMES} -o${OPTIONS} ${PARGS} ${INFILE} rfile > ${OUTF}"
	./mmu -a${X} -f${FRAMES} -o${OPTIONS} ${PARGS} ${INFILE} rfile > ${OUTF}
	OUTPUT=`egrep "^SUM" ${OUTF}`
	echo "${X}: ${OUTPUT}"
done
