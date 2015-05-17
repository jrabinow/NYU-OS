#!/bin/sh

(cd ../src/ && make distclean && make)
RFILE=./rfile
SCHED=./scheduler
DIR1=prog_outputs
DIR2=reference_outputs
INS="0 1 2 3 4 5"
DARGS="-b -B -E"
INS="0 1 2 3 4 5"

for f in ${INS}; do
	for s in F L S R2 R5 R20; do 
		#echo "${SCHED} -s${s} input${f} ${RFILE}"
		${SCHED} -s${s} input${f} ${RFILE} > ${DIR1}/output${f}_${s}
	done
done

for f in ${INS}; do
	for s in F L S R2 R5 R20; do 
		diff ${DARGS} ${DIR1}/output${f}_${s} ${DIR2}/output${f}_${s}
	done
done

#for f in ${INS}; do
#	for s in F L S R2 R5 R20; do 
#		echo "${SCHED} -v -s${s} input${f} ${RFILE}"
#		${SCHED} -v -s${s} input${f} ${RFILE} > output${f}_${s}_t
#	done
#done
	 
