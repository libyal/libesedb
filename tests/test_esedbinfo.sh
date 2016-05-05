#!/bin/bash
# Info tool testing script
#
# Version: 20160328

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

TEST_PREFIX=`dirname ${PWD}`;
TEST_PREFIX=`basename ${TEST_PREFIX} | sed 's/^lib\([^-]*\).*$/\1/'`;
TEST_SUFFIX="info";

TEST_PROFILE="${TEST_PREFIX}${TEST_SUFFIX}";
TEST_DESCRIPTION="${TEST_PREFIX}${TEST_SUFFIX}";
OPTION_SETS="";

TEST_TOOL_DIRECTORY="../${TEST_PREFIX}tools";
TEST_TOOL="${TEST_PREFIX}${TEST_SUFFIX}";
INPUT_DIRECTORY="input";
INPUT_GLOB="*";

if ! test -z ${SKIP_TOOLS_TESTS};
then
	exit ${EXIT_IGNORE};
fi

TEST_EXECUTABLE="${TEST_TOOL_DIRECTORY}/${TEST_TOOL}";

if ! test -x "${TEST_EXECUTABLE}";
then
	TEST_EXECUTABLE="${TEST_TOOL_DIRECTORY}/${TEST_TOOL}.exe";
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

run_test_on_input_directory "${TEST_PROFILE}" "${TEST_DESCRIPTION}" "with_stdout_reference" "${OPTION_SETS}" "${TEST_EXECUTABLE}" "${INPUT_DIRECTORY}" "${INPUT_GLOB}";
RESULT=$?;

exit ${RESULT};

