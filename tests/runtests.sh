#!/bin/sh
# Script to run tests
#
# Version: 20260714

if [ -f "${PWD}/libesedb/.libs/libesedb.1.dylib" ] && [ -f ./pyesedb/.libs/pyesedb.so ]
then
    install_name_tool -change /usr/local/lib/libesedb.1.dylib "${PWD}/libesedb/.libs/libesedb.1.dylib" ./pyesedb/.libs/pyesedb.so
fi

make check-build > /dev/null

# shellcheck disable=SC2068
make check $@
RESULT=$?

if [ ${RESULT} -ne 0 ]
then
    find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

