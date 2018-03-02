#!/bin/bash
#=============================================================================
#
usage() {
	echo "Usage:"
	echo "     -a | --apply                 Apply any changes found"
	echo "     -c | --config                Config file to use"
	echo "     -h | --help                  Display this help message"
	exit 0
}


#=============================================================================
#
while [[ $# -gt 0 ]] ; do
	case "$1" in
		-a|--apply) cli_apply="yes" ;;
		-c|--config) cli_config=${2}; shift;;
		-h*|--help*|*) usage ;;
	esac
	shift
done


#=============================================================================
#
scan_source() {
	find "src" -name "*[.ch]" ! -type d >> .uncrustify-search
	find "tests" -name "*[.cpph]" ! -type d >> .uncrustify-search
}


#=============================================================================
#
display_failures() {
	uncrustify                                \
		-l C                                  \
		-c ${cli_config}                      \
		--check                               \
		-F .uncrustify-search                 \
		| grep FAIL
}


#=============================================================================
#
generate_test_changes() {
	uncrustify                                \
		-l C                                  \
		-c ${cli_config}                      \
		-F .uncrustify-search                 \
		--prefix out
}


#=============================================================================
#
apply_changes() {
	if [ "${cli_apply}" = "yes" ]; then
		echo "Applying changes"
			uncrustify                       \
			-l C                             \
			-c ${cli_config}                 \
			-F .uncrustify-search            \
			--no-backup
	fi
}


#=============================================================================
#
cleanup() {
	rm .uncrustify-search
	if [ "${cli_apply}" = "yes" ]; then
		rm -rf out/
	fi
}

scan_source
display_failures
generate_test_changes
apply_changes
cleanup
