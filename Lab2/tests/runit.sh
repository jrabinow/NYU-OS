#!/bin/bash

RFILE=./rfile
SCHED=./scheduler
INS="0 1 2 3 4 5"
ALGS="F L S R2 R5 R20"

for f in ${INS}; do
	for s in ${ALGS}; do 
		echo "${SCHED} -s${s} input${f} ${RFILE}"
		${SCHED} -s${s} input${f} ${RFILE} > output${f}_${s}
	done
done

#for f in ${INS}; do
#	for s in F L S R2 R5 R20; do 
#		echo "${SCHED} -v -s${s} input${f} ${RFILE}"
#		${SCHED} -v -s${s} input${f} ${RFILE} > output${f}_${s}_t
#	done
#done
	 
