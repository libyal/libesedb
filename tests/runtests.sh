#!/bin/sh
# Script to run tests
#
# Version: 20260609

if test -f ${PWD}/libesedb/.libs/libesedb.1.dylib && test -f ./pyesedb/.libs/pyesedb.so
then
	install_name_tool -change /usr/local/lib/libesedb.1.dylib ${PWD}/libesedb/.libs/libesedb.1.dylib ./pyesedb/.libs/pyesedb.so
fi

make check-build > /dev/null

make check $@
RESULT=$?

if test ${RESULT} -ne 0
then
	find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

