#!/bin/bash

DIR1=$1
DIR2=$2
DARGS="-b -B -E"

INS="0 1 2 3 4 5"

for f in ${INS}; do
	for s in F L S R2 R5 R20; do 
		diff ${DARGS} ${DIR1}/output${f}_${s} ${DIR2}/output${f}_${s}
	done
done

#for f in ${INS}; do
#	for s in F L S R2 R5 R20; do 
#		diff ${DARGS} ${DIR1}/output${f}_${s}_t ${DIR2}/output${f}_${s}_t
#	done
#done
	 
