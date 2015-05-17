#!/bin/bash

inputdir=inputs

./test1.sh "${inputdir}"/in18 4 PFS
./test1.sh "${inputdir}"/in18 4 OPFS
./test1.sh "${inputdir}"/in18 4 pfOPFS

./test1.sh "${inputdir}"/in60 8 PFS
./test1.sh "${inputdir}"/in60 8 OPFS
./test1.sh "${inputdir}"/in60 8 pfOPFS

./test1.sh "${inputdir}"/in1K4 32 PFS
./test1.sh "${inputdir}"/in1K4 32 OPFS
./test1.sh "${inputdir}"/in1K4 32 pfOPFS

./test1.sh "${inputdir}"/inrd1K 32 PFS
./test1.sh "${inputdir}"/inrd1K 32 OPFS
./test1.sh "${inputdir}"/inrd1K 32 pfOPFS

./test1.sh "${inputdir}"/in10K3 32 PFS
./test1.sh "${inputdir}"/in10K3 32 OPFS

./test1.sh "${inputdir}"/in1M2 32 PFS
./test1.sh "${inputdir}"/in1M2 32 OPFS

