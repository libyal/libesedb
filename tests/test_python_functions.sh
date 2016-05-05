#!/bin/bash
# Python module functions testing script
#
# Version: 20160420

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

TEST_PREFIX=`dirname ${PWD}`;
TEST_PREFIX=`basename ${TEST_PREFIX} | sed 's/^lib\([^-]*\).*$/\1/'`;

TEST_PROFILE="py${TEST_PREFIX}";
TEST_FUNCTIONS="get_version";
TEST_FUNCTIONS_WITH_INPUT="open_close";
OPTION_SETS="";

TEST_TOOL_DIRECTORY=".";
INPUT_DIRECTORY="input";
INPUT_GLOB="*";

test_python_function()
{
	local TEST_PROFILE=$1;
	local TEST_FUNCTION=$2;
	local OPTION_SETS=$3;

	local TEST_SCRIPT="${TEST_TOOL_DIRECTORY}/py${TEST_PREFIX}_test_${TEST_FUNCTION}.py";

	echo -n -e "Testing Python-bindings function: py${TEST_PREFIX}.${TEST_FUNCTION}\t";

	run_test_with_arguments "${TEST_SCRIPT}";
	local RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo "(FAIL)";
	else
		echo "(PASS)";
	fi
	return ${RESULT};
}

test_python_function_with_input()
{
	local TEST_PROFILE=$1;
	local TEST_FUNCTION=$2;
	local OPTION_SETS=$3;
	local INPUT_DIRECTORY=$4;
	local INPUT_GLOB=$5;

	local TEST_SCRIPT="${TEST_TOOL_DIRECTORY}/py${TEST_PREFIX}_test_${TEST_FUNCTION}.py";

	run_test_on_input_directory "${TEST_PROFILE}" "${TEST_FUNCTION}" "default" "${OPTION_SETS}" "${TEST_SCRIPT}" "${INPUT_DIRECTORY}" "${INPUT_GLOB}";
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
	test_python_function "${TEST_PROFILE}" "${TEST_FUNCTION}" "${OPTION_SETS}";
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
	test_python_function_with_input "${TEST_PROFILE}" "${TEST_FUNCTION}" "${OPTION_SETS}" "${INPUT_DIRECTORY}" "${INPUT_GLOB}";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		break;
	fi
done

exit ${RESULT};

