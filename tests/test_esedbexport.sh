#!/bin/bash
# Export tool testing script
#
# Version: 20170901

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

OPTION_SETS="";
OPTIONS="";

INPUT_GLOB="*";

test_callback()
{
	local TMPDIR=$1;
	local TEST_SET_DIRECTORY=$2;
	local TEST_OUTPUT=$3;
	local TEST_EXECUTABLE=$4;
	local TEST_INPUT=$5;
	shift 5;
	local ARGUMENTS=$@;

	TEST_EXECUTABLE=$( readlink_f "${TEST_EXECUTABLE}" );
	INPUT_FILE_FULL_PATH=$( readlink_f "${INPUT_FILE}" );

	(cd ${TMPDIR} && run_test_with_input_and_arguments "${TEST_EXECUTABLE}" "${INPUT_FILE_FULL_PATH}" ${ARGUMENTS[@]} ${OPTIONS[@]});
	local RESULT=$?;

	local TEST_LOG="${TEST_OUTPUT}.log";

	if test "${PLATFORM}" = "Darwin";
	then
		(cd ${TMPDIR} && find "'${INPUT_NAME}.export'" -type f -exec md5 {} \; | sort -k 2 > "'${TEST_LOG}'");
	else
		(cd ${TMPDIR} && find "'${INPUT_NAME}.export'" -type f -exec md5sum {} \; | sort -k 2 > "'${TEST_LOG}'");
	fi

	local TEST_RESULTS="${TMPDIR}/${TEST_LOG}";
	local STORED_TEST_RESULTS="${TEST_SET_DIRECTORY}/${TEST_LOG}.gz";

	if test -f "'${STORED_TEST_RESULTS}'";
	then
		# Using zcat here since zdiff has issues on Mac OS X.
		# Note that zcat on Mac OS X requires the input from stdin.
		zcat < "'${STORED_TEST_RESULTS}'" | diff "'${TEST_RESULTS}'" -;
		RESULT=$?;
	else
		gzip "'${TEST_RESULTS}'";

		mv "'${TEST_RESULTS}.gz'" ${TEST_SET_DIRECTORY};
	fi
	return ${RESULT};
}

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

PLATFORM=`uname -s`;

source ${TEST_RUNNER};

assert_availability_binary find;

if test "${PLATFORM}" = "Darwin";
then
	assert_availability_binary md5;
else
	assert_availability_binary md5sum;
fi

run_test_on_input_directory "esedbexport" "esedbexport" "with_callback" "${OPTION_SETS}" "${TEST_EXECUTABLE}" "input" "${INPUT_GLOB}" "${OPTIONS}";
RESULT=$?;

exit ${RESULT};

