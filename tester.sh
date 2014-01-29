#!/bin/bash

function main()
{
	permissible_leaks=(11 12 13 14 15 16 17 18)
	permissible_errors=( 9 )
	testdir=labsamples
	binary=linker
	using_valgrind=false
	show_all=false

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

	for input in $(ls "${testdir}"/input-*|sort -nk2 -t'-')
	do
		suffix=${input:${#testdir}+7}
		echo "---------- TEST $suffix ----------"
		if ! $show_all; then
			./"${binary}" "${input}" >out
			diff -b -B -E out ${testdir}/out-$suffix
		fi
		if $using_valgrind && ! $show_all ; then
			valgrind ./"${binary}" "${input}" 2>out >/dev/null
			grep -A5 'LEAK SUMMARY' out|grep -v '0 bytes in 0 blocks'|cut -d' ' -f2-
			tail -1 out|grep -v '0 errors from 0 contexts'|cut -d' ' -f2-
		fi
	done
	rm -f out
}

main $*
