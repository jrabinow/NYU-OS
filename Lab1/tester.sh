#!/bin/bash

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
	# we pipe the directory listing through sort to ensure all tests are done in numerical order
	for input in $(ls "${testdir}"/input-*|sort -nk2 -t'-')
	do
		for suffix in $(ls ${input}|cut -d- -f2)
		do
			echo "---------- TEST $suffix ----------"
			if $show_all || (! $show_all && [ $(contains ${suppress_diffs[@]} $suffix) != 1 ]); then
				./"${binary}" ${opts} "${input}" >$tmpfile
				diff -b -B -E $tmpfile ${testdir}/out-$suffix
			fi
		done
	done
}

function run_leak_test()
{
	# we pipe the directory listing through sort to ensure all tests are done in numerical order
	for input in $(ls "${testdir}"/input-*|sort -nk2 -t'-')
	do
		for suffix in $(ls ${input}|cut -d- -f2)
		do
			echo "---------- TEST $suffix ----------"
			if $show_all || (! $show_all && [ $(contains ${suppress_leaks[@]} $suffix) != 1 ]); then
				valgrind ./"${binary}" ${opts} "${input}" 2>$tmpfile >/dev/null
				grep -A5 'LEAK SUMMARY' $tmpfile|grep -v '0 bytes in 0 blocks'|cut -d' ' -f2-
				tail -1 $tmpfile|grep -v '0 errors from 0 contexts'|cut -d' ' -f2-
			fi
		done
	done
}

function run_benchmark()
{
	for input in ${testdir}/input-*
	do
		for suffix in $(ls ${input}|cut -d- -f2)
		do
			./"${binary}" ${opts} "${input}" > /dev/null
		done
	done
}

function main()
{
	local binary=linker				# binary file name
	local opts=-c					# options to pass to binary program
	local testdir=labsamples			# directory containing sample inputs and outputs

	local diff_test=false				# perform output comparison tests
	local leak_test=false				# perform memory leak tests
	local benchmark=false				# perform benchmark
	local count=1

	local suppress_diffs=( 9 )			# don't show diff errors for tests in array
	local suppress_leaks=(11 12 13 14 15 16 17 18)	# don't show memory leak errors for tests in array
	local show_all=true				# ignore suppress* arrays and print all errors
	local tmpfile=$(mktemp)

	while getopts "b:d:himot" opt; do
		case ${opt} in
			b)	# redefine binary
				binary="${OPTARG}";
				;;
			d)	# redefine testdir
				testdir="${OPTARG}"
				;;
			i)	# ignore
				show_all=false
				;;
			h)	# help message
				cat << EOF
Usage: $0 [OPTS]...
Options: -dARG: set directory containing test input-output values to ARG
	-h: show this help dialog
	-i: ignore tests included in suppress* arrays (hardcoded)
	-m: run leak-check tests (do not run output comparison tests)
	-o: run output comparison tests (do not run leak-check tests)
	-t[ARG]: benchmark application. Optional argument specifies number of runs
EOF
				exit 0
				;;
			m)	# leak check test
				leak_test=true
				;;
			o)	# output comparison test
				diff_test=true
				;;
			t)	# benchmarking
				benchmark=true
				if [ ! -z ${!OPTIND} ] && [[ "${!OPTIND#-t}" =~ ^[0-9]+$ ]]; then
					let count="${!OPTIND#-t}"
					let OPTIND++
				fi
				;;
			?)
				echo "Unknown option, exiting now" >&2
				exit 1
		esac
	done

	if [ ! -f "${binary}" ] || [ ! -x "${binary}" ]; then
		echo "Invalid program: '${binary}' does not exist or is not executable" >&2
		exit 1
	fi
	if [ ! -d "${testdir}" ] || [ ! -r "${testdir}" ]; then
		echo "Invalid testdir: '${testdir}' does not exist or is not searchable" >&2
		exit 1
	fi

	if $benchmark; then
		let totalmin=0
		let totalsec=0
		for((i = 0; i < $count; i++)) {
			output=$((time run_benchmark) |& grep real)
			parsestr=${output##real}
			parsestr=${parsestr%s}
			min=${parsestr%%m*}
			sec=${parsestr##*m}
			let totalmin=$(echo $min + $totalmin | bc)
			totalsec=$(echo $sec+$totalsec | bc)
			#echo $min $sec
		}
		echo $totalmin/$count|bc
		exit 0
	fi

	if ! $diff_test && ! $leak_test; then
		diff_test=true
		leak_test=true
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

	rm -f $tmpfile
}

if [[ "$BASH_SOURCE" == "$0" ]]
then
	main $*
fi

