#!/bin/bash

DIR1=$1
DIR2=$2
DARGS=$3

############################################################################
# run with RFILE1
############################################################################

for F in ${DIR1}/out*_PFS; do
      B=`basename ${F}`
      X=`diff -b ${DARGS} ${DIR1}/${B} ${DIR2}/${B}`
      [[ "${X}"  != "" ]] && echo "${B} diff"
done
  
