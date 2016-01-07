#!/bin/bash
#
# Script to run an executable for testing.
#
# When CHECK_WITH_VALGRIND is set to a non-empty value the executable
# is run with valgrind, otherwise it is run without.
#
# When CHECK_WITH_GDB is set to a non-empty value the executable
# is run with gdb, otherwise it is run without.
#
# Copyright (C) 2009-2016, Joachim Metz <joachim.metz@gmail.com>
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

if test $# -le 0;
then
	echo "Missing arguments";

	exit ${EXIT_FAILURE};
fi

EXECUTABLE=$1;

shift

if ! test -x ${EXECUTABLE};
then
	echo "Invalid executable: ${EXECUTABLE}";

	exit ${EXIT_FAILURE};
fi

if ! test -d "tmp";
then
	echo "Missing tmp directory.";

	exit ${EXIT_FAILURE};
fi

# Check if the executable needs to run with valgrind or gdb
if ! test -z ${CHECK_WITH_VALGRIND} || ! test -z ${CHECK_WITH_GDB};
then
	# Check if the executable is a libtool shell script or a binary.
	# Use the binary in combination with valgrind otherwise the shell binary is also tested.
	file -bi ${EXECUTABLE} | sed 's/;.*$//' | grep "text/x-shellscript" > /dev/null 2>&1;

	echo ${EXECUTABLE} | grep 'tools' > /dev/null 2>&1;

	if test $? -eq 0;
	then
		LIBRARY=`dirname ${EXECUTABLE} | sed 's?.*/\(.*\)tools$?lib\1?'`;
	else
		LIBRARY=`basename ${EXECUTABLE} | sed 's/^\(.*\)_test_.*$/lib\1/'`;
	fi

	if test $? -eq 0;
	then
		DIRNAME=`dirname ${EXECUTABLE}`;
		BASENAME=`basename ${EXECUTABLE}`;
		EXECUTABLE="${DIRNAME}/.libs/${BASENAME}";

		if ! test -x ${EXECUTABLE};
		then
			echo "Unable to find executable: ${EXECUTABLE}";

			exit ${EXIT_FAILURE};
		fi

		file -bi ${EXECUTABLE} | sed 's/;.*$//' | grep "application/x-executable" > /dev/null 2>&1;

		if test $? -ne 0;
		then
			echo "Invalid executable: ${EXECUTABLE}";

			exit ${EXIT_FAILURE};
		fi
	fi

	if ! test -z ${CHECK_WITH_VALGRIND};
	then
		IFS="
"; LD_LIBRARY_PATH="../${LIBRARY}/.libs/" valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-reachable=yes --log-file=tmp/valgrind.log ${EXECUTABLE} $*;
	else
		IFS="
"; LD_LIBRARY_PATH="../${LIBRARY}/.libs/" gdb -ex r --args ${EXECUTABLE} $*;
	fi

	EXIT_RESULT=$?;

	if ! test -z ${CHECK_WITH_VALGRIND};
	then
		if test ${EXIT_RESULT} -eq 0;
		then
			grep "All heap blocks were freed -- no leaks are possible" tmp/valgrind.log > /dev/null 2>&1;

			if test $? -ne 0;
			then
				echo "Memory leakage detected.";

				cat tmp/valgrind.log;

				EXIT_RESULT=${EXIT_FAILURE};
			fi
		fi
		rm -f tmp/valgrind.log;
	fi
else
	IFS="
"; ${EXECUTABLE} $*;

	EXIT_RESULT=$?;
fi

exit ${EXIT_RESULT};

