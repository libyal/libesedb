#!/bin/bash
# Export tool testing script
#
# Version: 20170802

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

OPTION_SETS="";

INPUT_GLOB="*";

if ! test -z ${SKIP_TOOLS_TESTS};
then
	exit ${EXIT_IGNORE};
fi

TEST_EXECUTABLE="../esedbtools/esedbexport";

if ! test -x "${TEST_EXECUTABLE}";
then
	TEST_EXECUTABLE="../esedbtools/esedbexport.exe";
fi

if ! test -x "${TEST_EXECUTABLE}";
then
	echo "Missing test executable: ${TEST_EXECUTABLE}";

	exit ${EXIT_FAILURE};
fi

TEST_RUNNER="tests/test_runner.sh";

if ! test -f "${TEST_RUNNER}";
then
	TEST_RUNNER="./test_runner.sh";
fi

if ! test -f "${TEST_RUNNER}";
then
	echo "Missing test runner: ${TEST_RUNNER}";

	exit ${EXIT_FAILURE};
fi

source ${TEST_RUNNER};

run_test_on_input_directory "esedbexport" "esedbexport" "with_stdout_reference" "${OPTION_SETS}" "${TEST_EXECUTABLE}" "input" "${INPUT_GLOB}";
RESULT=$?;

exit ${RESULT};

