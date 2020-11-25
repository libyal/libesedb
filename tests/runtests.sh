#!/bin/sh
# Script to run tests
#
# Version: 20201121

if test -f ${PWD}/libesedb/.libs/libesedb.1.dylib && test -f ./pyesedb/.libs/pyesedb.so;
then
	install_name_tool -change /usr/local/lib/libesedb.1.dylib ${PWD}/libesedb/.libs/libesedb.1.dylib ./pyesedb/.libs/pyesedb.so;
fi

make check CHECK_WITH_STDERR=1;
RESULT=$?;

if test ${RESULT} -ne 0 && test -f tests/test-suite.log;
then
	cat tests/test-suite.log;
fi
exit ${RESULT};

