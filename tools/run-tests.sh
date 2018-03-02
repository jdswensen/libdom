build_dir=. # Default to this directory

usage() {
	echo "Usage:"
	echo "     -c | --coverage             Run code coverage tests (implies -t)"
	echo "     -d | --dir                  Build directory"
	echo "     -l | --lcov-cobertura       Location of the lcov-cobertura tool"
	echo "     -t | --test                 Run unit and coverage tests"
	echo "     -h | --help                 Display this help message"
	exit 0
}

while [[ $# -gt 0 ]] ; do
	case "$1" in
		-c|--coverage) cli_coverage="yes"; cli_test="yes";;
		-d|--dir) cli_build_dir=$2; shift ;;
		-l|--lcov-cobertura) cli_lcov_cobertura=$2; shift;;
		-t|--test) cli_test="yes";;
		-h*|--help*|*) usage ;;
	esac
	shift
done

set -e

#=============================================================================
# Overwrite config values with anything passed in from the CLI
if [[ -n ${cli_build_dir} ]]; then
	build_dir=${cli_build_dir};
fi

run_test() {
	if [ "${cli_test}" = "yes" ]; then
		cd ${build_dir}
		mkdir -p results/

		if [ "${cli_coverage}" = "yes" ]; then
			lcov --rc lcov_branch_coverage=1 --zerocounters --directory .
			lcov --rc lcov_branch_coverage=1 --capture --initial --directory . --output-file results/coverage-initial

			# Check if this is actually a coverage build by checking if the
			# coverage-initial file is empty
			echo "Checking for coverage data"
			if [[ -s results/coverage-initial ]]; then
				echo "Coverage data exits"
			else
				echo "coverage-initial is empty, skipping code coverage"
				cli_coverage="no"
			fi

		fi

		echo "Running ctest"
		ctest

		if [ "${cli_coverage}" = "yes" ]; then
			lcov --rc lcov_branch_coverage=1 --directory . --capture --output-file results/coverage.info
			lcov --rc lcov_branch_coverage=1 --remove results/coverage.info '*/build/*' '*/tests/*' '/usr/*' '*.hunter/*' --output-file results/coverage.info.clean
			genhtml --branch-coverage --highlight --legend -o results/coverage results/coverage.info.clean

			if [ "${cli_lcov_cobertura}" ]; then
				python ${cli_lcov_cobertura} results/coverage.info.clean --output results/COVERAGE-mmmiot.cobertura.xml --base-dir .
			else
				echo "lcov_cobertura.py location not defined (define it with '-l')"
			fi
		fi
	fi
}

echo "PWD: ${PWD}"
run_test
