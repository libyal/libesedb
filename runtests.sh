#!/bin/sh
# Script that runs the tests
#
# Version: 20141218

EXIT_SUCCESS=0;
EXIT_FAILURE=1;

run_tests()
{
	CONFIGURE_OPTIONS=$1;

	./configure ${CONFIGURE_OPTIONS};

	if test $? -ne ${EXIT_SUCCESS};
	then
		echo "Running: './configure' failed";

		return ${EXIT_FAILURE};
	fi

	make clean > /dev/null;

	if test $? -ne ${EXIT_SUCCESS};
	then
		echo "Running: 'make clean' failed";

		return ${EXIT_FAILURE};
	fi

	make > /dev/null;

	if test $? -ne ${EXIT_SUCCESS};
	then
		echo "Running: 'make' failed";

		return ${EXIT_FAILURE};
	fi

	make check;

	if test $? -ne ${EXIT_SUCCESS};
	then
		echo "Running: 'make check' failed";

		if test -f tests/test-suite.log;
		then
			cat tests/test-suite.log;
		fi

		return ${EXIT_FAILURE};
	fi
	return ${EXIT_SUCCESS};
}

./configure --help | grep -- '--enable-python' > /dev/null;

HAVE_ENABLE_PYTHON=$?;

if test ${HAVE_ENABLE_PYTHON} -ne 0;
then
	if ! run_tests;
	then
		exit ${EXIT_FAILURE};
	fi
else
	# Test with Python 2.
	PYTHON2=`which python2 2> /dev/null`;

        # Note that "test -x" on Mac OS X will succeed if the argument is not set.
	if test ! -z ${PYTHON2} && test -x ${PYTHON2};
	then
		export PYTHON_VERSION=2;

		if ! run_tests "--enable-python";
		then
			exit ${EXIT_FAILURE};
		fi
		export PYTHON_VERSION=;
	fi

	# Test with Python 3.
	PYTHON3=`which python3 2> /dev/null`;

        # Note that "test -x" on Mac OS X will succeed if the argument is not set.
	if test ! -z ${PYTHON3} && test -x ${PYTHON3};
	then
		export PYTHON_VERSION=3;

		if ! run_tests "--enable-python";
		then
			exit ${EXIT_FAILURE};
		fi
		export PYTHON_VERSION=;
	fi

	# Test with the default Python version.
	if test -z ${PYTHON2} && test -z ${PYTHON3};
	then
		if ! run_tests "--enable-python";
		then
			exit ${EXIT_FAILURE};
		fi
	fi
fi

exit ${EXIT_SUCCESS};

