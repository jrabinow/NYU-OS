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

function main()
{
	local suppress_leaks=(11 12 13 14 15 16 17 18)	# don't show memory leak errors for tests in array
	local suppress_errors=( 9 )			# don't show diff errors for tests in array
	local testdir=labsamples	# directory containing sample inputs and outputs
	local binary=linker		# binary file name
	local using_valgrind=false	# perform memory leak tests by default?
	local show_all=false		# ignore suppress* arrays and print all errors
	local tmpfile=$(mktemp)

	while getopts "sv" opt; do
		case ${opt} in
			s)
				show_all=true
				;;
			v)
				which valgrind >/dev/null
				if [ $? != 0 ]; then
					echo "Error: valgrind not found in path. Ignoring option -v" >&2
				else
					using_valgrind=true
				fi
				;;
			?)
				echo "Unknown option, exiting now" >&2
				exit 1
		esac
	done

	for input in $(ls "${testdir}"/input-*|sort -nk2 -t'-')	# we pipe the directory listing through sort to ensure all tests are done in numerical order
	do
		for suffix in $(ls ${input}|cut -d- -f2)
		do
			echo "---------- TEST $suffix ----------"
			if $show_all || (! $show_all && [ $(contains ${suppress_errors[@]} $suffix) != 1 ]); then
				./"${binary}" -c "${input}" >$tmpfile
				diff -b -B -E $tmpfile ${testdir}/out-$suffix
			fi
			if ($using_valgrind && $show_all) || ($using_valgrind && ! $show_all && [ $(contains ${suppress_leaks[@]} $suffix) != 1 ]); then
				valgrind ./"${binary}" -c "${input}" 2>$tmpfile >/dev/null
				grep -A5 'LEAK SUMMARY' $tmpfile|grep -v '0 bytes in 0 blocks'|cut -d' ' -f2-
				tail -1 $tmpfile|grep -v '0 errors from 0 contexts'|cut -d' ' -f2-
			fi
		done
	done
	rm -f $tmpfile
}

main $*
