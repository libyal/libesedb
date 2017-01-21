#!/bin/bash
# Tests Python module functions and types.
#
# Version: 20170115

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

TEST_PREFIX=`dirname ${PWD}`;
TEST_PREFIX=`basename ${TEST_PREFIX} | sed 's/^lib\([^-]*\).*$/\1/'`;

TEST_PROFILE="py${TEST_PREFIX}";
TEST_FUNCTIONS="support";
TEST_FUNCTIONS_WITH_INPUT="file";
OPTION_SETS="";

TEST_TOOL_DIRECTORY=".";
INPUT_DIRECTORY="input";
INPUT_GLOB="*";

test_python_function()
{
	local TEST_FUNCTION=$1;

	local TEST_DESCRIPTION="Testing Python-bindings functions: ${TEST_FUNCTION}";
	local TEST_SCRIPT="${TEST_TOOL_DIRECTORY}/py${TEST_PREFIX}_test_${TEST_FUNCTION}.py";

	run_test_with_arguments "${TEST_DESCRIPTION}" "${TEST_SCRIPT}";
	local RESULT=$?;

	return ${RESULT};
}

test_python_function_with_input()
{
	local TEST_FUNCTION=$1;

	local TEST_DESCRIPTION="Testing Python-bindings functions: ${TEST_FUNCTION}";
	local TEST_SCRIPT="${TEST_TOOL_DIRECTORY}/py${TEST_PREFIX}_test_${TEST_FUNCTION}.py";

	run_test_on_input_directory "${TEST_PROFILE}" "${TEST_DESCRIPTION}" "default" "${OPTION_SETS}" "${TEST_SCRIPT}" "${INPUT_DIRECTORY}" "${INPUT_GLOB}";
	local RESULT=$?;

	return ${RESULT};
}

if ! test -z ${SKIP_PYTHON_TESTS};
then
	exit ${EXIT_IGNORE};
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

RESULT=${EXIT_IGNORE};

for TEST_FUNCTION in ${TEST_FUNCTIONS};
do
	test_python_function "${TEST_FUNCTION}";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		break;
	fi
done

if test ${RESULT} -ne ${EXIT_SUCCESS} && test ${RESULT} -ne ${EXIT_IGNORE};
then
	exit ${RESULT};
fi

for TEST_FUNCTION in ${TEST_FUNCTIONS_WITH_INPUT};
do
	test_python_function_with_input "${TEST_FUNCTION}";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		break;
	fi
done

exit ${RESULT};

