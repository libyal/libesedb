#!/bin/bash
#
# Library open close testing script
#
# Copyright (c) 2009-2014, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.
#

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

list_contains()
{
	LIST=$1;
	SEARCH=$2;

	for LINE in $LIST;
	do
		if test $LINE = $SEARCH;
		then
			return ${EXIT_SUCCESS};
		fi
	done

	return ${EXIT_FAILURE};
}

test_open_close()
{ 
	INPUT_FILE=$1;

	rm -rf tmp;
	mkdir tmp;

	echo "Testing open close of input: ${INPUT_FILE}";

	${TEST_RUNNER} ./${ESEDB_TEST_OPEN_CLOSE} ${INPUT_FILE};

	RESULT=$?;

	rm -rf tmp;

	echo "";

	return ${RESULT};
}

ESEDB_TEST_OPEN_CLOSE="esedb_test_open_close";

if ! test -x ${ESEDB_TEST_OPEN_CLOSE};
then
	ESEDB_TEST_OPEN_CLOSE="esedb_test_open_close.exe";
fi

if ! test -x ${ESEDB_TEST_OPEN_CLOSE};
then
	echo "Missing executable: ${ESEDB_TEST_OPEN_CLOSE}";

	exit ${EXIT_FAILURE};
fi

TEST_RUNNER="tests/test_runner.sh";

if ! test -x ${TEST_RUNNER};
then
	TEST_RUNNER="./test_runner.sh";
fi

if ! test -x ${TEST_RUNNER};
then
	echo "Missing test runner: ${TEST_RUNNER}";

	exit ${EXIT_FAILURE};
fi

if ! test -d "input";
then
	echo "No input directory found.";

	exit ${EXIT_IGNORE};
fi

OLDIFS=${IFS};
IFS="
";

RESULT=`ls input/* | tr ' ' '\n' | wc -l`;

if test ${RESULT} -eq 0;
then
	echo "No files or directories found in the input directory.";

	EXIT_RESULT=${EXIT_IGNORE};
else
	IGNORELIST="";

	if test -f "input/.libesedb/ignore";
	then
		IGNORELIST=`cat input/.libesedb/ignore | sed '/^#/d'`;
	fi
	for TESTDIR in input/*;
	do
		if test -d "${TESTDIR}";
		then
			DIRNAME=`basename ${TESTDIR}`;

			if ! list_contains "${IGNORELIST}" "${DIRNAME}";
			then
				if test -f "input/.libesedb/${DIRNAME}/files";
				then
					TEST_FILES=`cat input/.libesedb/${DIRNAME}/files | sed "s?^?${TESTDIR}/?"`;
				else
					TEST_FILES=`ls -1 ${TESTDIR}/* 2> /dev/null`;
				fi
				for TEST_FILE in ${TEST_FILES};
				do
					if ! test_open_close "${TEST_FILE}";
					then
						exit ${EXIT_FAILURE};
					fi
				done
			fi
		fi
	done

	EXIT_RESULT=${EXIT_SUCCESS};
fi

IFS=${OLDIFS};

exit ${EXIT_RESULT};

