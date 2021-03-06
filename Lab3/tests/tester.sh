#!/bin/bash

#  Copyright (C) 2014 Julien Rabinow <jnr305@nyu.edu>
#
#  This file is part of Lab3-Mem-manager.
#
#  Lab3-Mem-manager is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Lab3-Mem-manager is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Lab3-Mem-manager. If not, see <http://www.gnu.org/licenses/>.

function contains()
{
	local n=$#
	local value=${!n}
	for((i = 1; i < $#; i++)) {
		if [ "${!i}" == "${value}" ]; then
			echo "1"
			return 0
		fi
	}
	echo "0"
	return 0
}

function run_diff_test()
{
	passed_tests=0
	out_format=OPFS
	for input in "${input_testdir}"/*
	do
		filename=$(basename $input)
		frames=${input_numframes[$filename]}
		for algo in ${algos}; do 
			testnum=${filename##*in}"_"${algo}"_"${frames}"_"${out_format}
			if ! test -f "${output_testdir}/out_in${testnum}"; then
				continue
			fi
			if $verbose; then
				echo "---------- TEST in$testnum ----------" >&2
			fi
			if $show_all || (! $show_all && [ $(contains ${suppress_diffs[@]} "${testnum}") != 1 ]); then
				./"${binary}" ${opts} -f"${frames}" -a"${algo}" -o"${out_format}" "${input}" "${rfile}" > "${tmpfile}"
				if [ $? != 0 ]; then
					echo $frames $input >&2
				fi
				res=$(diff -b -B -E "${tmpfile}" "${output_testdir}/out_in${testnum}")
				[[ "${res}"  == "" ]] && test -f ${tmpfile} && let passed_tests=$passed_tests+1 || ($verbose && echo "FAIL." >&2 )
			fi
		done
	done
	echo $passed_tests
}

function run_leak_test()
{
	out_format=
	frames=4
	input="${input_testdir}"/in18
	for algo in ${algos}; do 
		echo "---------- TEST "${input##*in}"_"${algo}"_"${frames}" ----------" >&2
		if $show_all || (! $show_all && [ $(contains ${suppress_leaks[@]} "${algo}") != 1 ]); then
			valgrind ./"${binary}" ${opts} -f"${frames}" -a"${algo}" -o"${out_format}" "${input}" "${rfile}" > /dev/null 2> "${tmpfile}"
			grep -A5 'LEAK SUMMARY' "${tmpfile}" |grep -v '0 bytes in 0 blocks'|cut -d' ' -f2-
			tail -1 "${tmpfile}"|grep -v '0 errors from 0 contexts'|cut -d' ' -f2-
		fi
	done

	frames=8
	input="${input_testdir}"/in60
	for algo in ${algos}; do 
		echo "---------- TEST "${input##*in}"_"${algo}"_"${frames}" ----------" >&2
		if $show_all || (! $show_all && [ $(contains ${suppress_leaks[@]} "${algo}") != 1 ]); then
			valgrind ./"${binary}" ${opts} -f"${frames}" -a"${algo}" -o"${out_format}" "${input}" "${rfile}" > /dev/null 2> "${tmpfile}"
			grep -A5 'LEAK SUMMARY' "${tmpfile}" |grep -v '0 bytes in 0 blocks'|cut -d' ' -f2-
			tail -1 "${tmpfile}"|grep -v '0 errors from 0 contexts'|cut -d' ' -f2-
		fi
	done

	frames=32
	for input in "${input_testdir}"/in1K4 "${input_testir}"/inrd1K "${input_testir}"/in10K3
	do
		for algo in ${algos}; do 
			echo "---------- TEST "${input##*in}"_"${algo}"_"${frames}" ----------" >&2
			if $show_all || (! $show_all && [ $(contains ${suppress_leaks[@]} "${algo}") != 1 ]); then
				valgrind ./"${binary}" ${opts} -f"${frames}" -a"${algo}" -o"${out_format}" "${input}" "${rfile}" > /dev/null 2> "${tmpfile}"
				grep -A5 'LEAK SUMMARY' "${tmpfile}" |grep -v '0 bytes in 0 blocks'|cut -d' ' -f2-
				tail -1 "${tmpfile}"|grep -v '0 errors from 0 contexts'|cut -d' ' -f2-
			fi
		done
	done
}

function run_benchmark()
{
	frames=4
	input="${input_testdir}"/in18
	for algo in ${algos}; do 
		./"${binary}" ${opts} -f"${frames}" -a"${algo}" "${input}" "${rfile}" > /dev/null
	done
	frames=8
	input="${input_testdir}"/in60
	for algo in ${algos}; do 
		./"${binary}" ${opts} -f"${frames}" -a"${algo}" "${input}" "${rfile}" > /dev/null
	done
	frames=32
	for input in "${input_testdir}"/in1K4 "${input_testir}"/inrd1K "${input_testir}"/in10K3
	do
		for algo in ${algos}; do 
			./"${binary}" ${opts} -f"${frames}" -a"${algo}" "${input}" "${rfile}" > /dev/null
		done
	done
}

function main()
{
	local binary=mmu				# binary file name
	local opts=					# options to pass to binary program
	local input_testdir=../tests/inputs		# directory containing sample inputs
	local output_testdir=../tests/references	# directory containing reference outputs
	local rfile="${input_testdir}"/rfile

	local diff_test=false				# perform output comparison tests
	local leak_test=false				# perform memory leak tests
	local benchmark=false				# perform benchmark
	local verbose=false				# increase verbosity
	local count=1					# default number of times to run benchmark test

	local suppress_diffs=( )			# don't show diff errors for tests in array
	local suppress_leaks=( )			# don't show memory leak errors for tests in array
	local show_all=true				# ignore suppress* arrays and print all errors
	local tmpfile=$(mktemp)

	# non-generic variables
	local algos="N l r f s c C a A"			# algorithm identifiers
	declare -A input_numframes			# associate to each input file its corresponding number of frames
	input_numframes=([in18]=4 [in60]=8 [in1K4]=32 [inrd1K]=32 [in10K3]=32 [in1M2]=32)

	while getopts "b:d:e:hmor:sv" opt; do
		case ${opt} in
			b)	# redefine binary
				binary="${OPTARG}"
				;;
			d)	# redefine input_testdir
				input_testdir="${OPTARG}"
				;;
			e)	# redefine output_testdir
				output_testdir="${OPTARG}"
				;;
			h)	# help message
				cat << EOF
Usage: $0 [OPTION]... output|memory|benchmark
Options: -bARG: set binary name to ARG
	-dARG: set directory containing test input files to ARG
	-eARG: set directory containing test output files to ARG
	-h: show this help dialog
	-rARG: number of runs over which to benchmark application
	-s: suppress test results from tests in hardcoded suppress* arrays
	-v: enable verbose mode for benchmarking
EOF
				exit 0
				;;
			r)	# number of benchmarking runs
				count="${OPTARG}"
				;;
			s)	show_all=false
				;;
			v)	# verbosity
				verbose=true
				;;
			?)
				echo "Unknown option, exiting now" >&2
				exit 1
		esac
	done
	while [ $# != 0 ]; do
		action=$1; shift
		if [ $action == 'output' ]; then
			diff_test=true
		fi
		if [ $action == 'memory' ]; then
			leak_test=true
		fi
		if [ $action == 'benchmark' ]; then
			benchmark=true
		fi
	done

	if [ ! -f "${binary}" ] || [ ! -x "${binary}" ]; then
		echo "Invalid program: '${binary}' does not exist or is not executable" >&2
		exit 1
	fi
	if [ ! -d "${input_testdir}" ] || [ ! -r "${input_testdir}" ]; then
		echo "Invalid input testdir: '${input_testdir}' does not exist or is not searchable" >&2
		exit 1
	fi
	if [ ! -d "${output_testdir}" ] || [ ! -r "${output_testdir}" ]; then
		echo "Invalid output testdir: '${output_testdir}' does not exist or is not searchable" >&2
		exit 1
	fi

	if ! $diff_test && ! $leak_test && ! $benchmark; then
		diff_test=true
		leak_test=true
		benchmark=true
	fi
	if $diff_test; then
		echo -e "\n\x1B[3mRunning diff tests...\x1B[0m\n"
		run_diff_test
	fi
	if $leak_test; then
		which valgrind >/dev/null
		if [ $? != 0 ]; then
			echo -e "\n\x1B[31;1mError: valgrind not found in path. Will not check for memory leaks\x1B[0m" >&2
		else
			echo -e "\n\x1B[3mRunning memory leak tests...\x1B[0m\n"
			run_leak_test
		fi
	fi

	if $benchmark; then
		echo -e "\n\x1B[3mRunning benchmark...\x1B[0m\n"
		let total=0
		for((i = 0; i < $count; i++)) {
			output=$((time run_benchmark) |& grep real)
			min=$(echo $output|cut -b5-|cut -dm -f1)
			sec=$(echo $output|cut -dm -f2|cut -d. -f1)
			milli=$(echo ${output##*m*.}|cut -ds -f1)
			let total=total+min*60000+sec*1000+milli
			if $verbose; then
				echo ${min}m$sec.${milli}s
			fi
		}
		let time_secs=total/1000
		let time_milli=total%1000
		let average=total/count
		let average_secs=average/1000
		let average_milli=average%1000
		echo -e "\n$count executions"
		echo -e "Total time:\t\t$time_secs.${time_milli}s"
		echo -e "Average runtime:\t$average_secs.${average_milli}s"
	fi

	if $diff_test || $leak_test; then
		echo "SUMMARY:"
		# !!! NON-GENERIC !!!
		let numtests=$(echo "${algos}"|tr -d ' \n'|wc -c)*5	# 5 test cases per algorithm
		if $diff_test; then
			echo "Passed diff tests: $passed_diffs/$numtests"
		fi
	#	if $leak_test; then
	#		echo "Passed diff tests: $passed_diffs/$numtests"
	#	fi
	fi

	rm -f $tmpfile
}

if [[ "$BASH_SOURCE" == "$0" ]]
then
	main "$@"
fi

