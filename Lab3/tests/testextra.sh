#!/bin/bash -v

inputdir=inputs

./mmu -aa -f4 -oapfOPFS "${inputdir}"/in18 rfile > out_in18_a_4_apfOPFS
./mmu -aA -f4 -oapfOPFS "${inputdir}"/in18 rfile > out_in18_A_4_apfOPFS
./mmu -aa -f8 -oapfOPFS "${inputdir}"/in60 rfile > out_in60_a_8_apfOPFS
./mmu -aA -f8 -oapfOPFS "${inputdir}"/in60 rfile > out_in60_A_8_apfOPFS

./mmu -aN -f4 -oapfOPFS "${inputdir}"/in18 rfile > out_in18_N_4_apfOPFS
./mmu -aN -f8 -oapfOPFS "${inputdir}"/in60 rfile > out_in60_N_8_apfOPFS

./mmu -aN -f32 -oapfOPFS "${inputdir}"/in1K4 rfile > out_in1K4_N_32_apfOPFS

