#!/bin/sh
# Script that synchronizes the local test data
#
# Version: 20161009

TEST_SET="public";
TEST_INPUT_DIRECTORY="tests/input";
TEST_FILES="Windows.edb";

mkdir -p "${TEST_INPUT_DIRECTORY}/${TEST_SET}";

for TEST_FILE in ${TEST_FILES};
do
	URL="https://github.com/log2timeline/plaso/blob/main/test_data/${TEST_FILE}?raw=true";

	curl -L -o "${TEST_INPUT_DIRECTORY}/${TEST_SET}/${TEST_FILE}" ${URL};
done

