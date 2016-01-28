#!/bin/sh
# Script that runs the tests
#
# Version: 20160124

EXIT_SUCCESS=0;
EXIT_FAILURE=1;

run_configure_make()
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
	return ${EXIT_SUCCESS};
}

run_configure_make_check()
{
	run_configure_make $1;

	if test $? -ne ${EXIT_SUCCESS};
	then
		return $?;
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

run_configure_make_check_python()
{
	run_configure_make $1;

	if test $? -ne ${EXIT_SUCCESS};
	then
		return $?;
	fi

	make check SKIP_LIBRARY_TESTS=1 SKIP_TOOLS_TESTS=1;

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

run_setup_py_tests()
{
	PYTHON=$1;

	${PYTHON} setup.py build

	if test $? -ne ${EXIT_SUCCESS};
	then
		echo "Running: 'setup.py build' failed";

		return ${EXIT_FAILURE};
	fi
	return ${EXIT_SUCCESS};
}

if ! run_configure_make_check;
then
	exit ${EXIT_FAILURE};
fi

./configure --help | grep -- '--with-zlib' > /dev/null;

HAVE_WITH_ZLIB=$?;

if test ${HAVE_WITH_ZLIB} -eq 0;
then
	if ! run_configure_make_check "--with-zlib=no";
	then
		exit ${EXIT_FAILURE};
	fi
fi

./configure --help | grep -- '--with-openssl' > /dev/null;

HAVE_WITH_OPENSSL=$?;

if test ${HAVE_WITH_OPENSSL} -eq 0;
then
	if ! run_configure_make_check "--with-openssl=no";
	then
		exit ${EXIT_FAILURE};
	fi

	if ! run_configure_make_check "--enable-openssl-evp-cipher=no --enable-openssl-evp-md=no";
	then
		exit ${EXIT_FAILURE};
	fi
fi

./configure --help | grep -- '--enable-python' > /dev/null;

HAVE_ENABLE_PYTHON=$?;

if test ${HAVE_ENABLE_PYTHON} -eq 0;
then
	# Test with Python 2.
	PYTHON2=`which python2 2> /dev/null`;

        # Note that "test -x" on Mac OS X will succeed if the argument is not set.
	if test ! -z ${PYTHON2} && test -x ${PYTHON2};
	then
		export PYTHON_VERSION=2;

		if ! run_configure_make_check_python "--enable-python";
		then
			exit ${EXIT_FAILURE};
		fi
		export PYTHON_VERSION=;

		if ! run_configure_make_check_python "--enable-python2";
		then
			exit ${EXIT_FAILURE};
		fi

		if test -f "setup.py" && ! run_setup_py_tests ${PYTHON2};
		then
			exit ${EXIT_FAILURE};
		fi
	fi

	# Test with Python 3.
	PYTHON3=`which python3 2> /dev/null`;

        # Note that "test -x" on Mac OS X will succeed if the argument is not set.
	if test ! -z ${PYTHON3} && test -x ${PYTHON3};
	then
		export PYTHON_VERSION=3;

		if ! run_configure_make_check_python "--enable-python";
		then
			exit ${EXIT_FAILURE};
		fi
		export PYTHON_VERSION=;

		if ! run_configure_make_check_python "--enable-python3";
		then
			exit ${EXIT_FAILURE};
		fi

		if test -f "setup.py" && ! run_setup_py_tests ${PYTHON3};
		then
			exit ${EXIT_FAILURE};
		fi
	fi

	# Test with the default Python version.
	if test -z ${PYTHON2} && test -z ${PYTHON3};
	then
		if ! run_configure_make_check_python "--enable-python";
		then
			exit ${EXIT_FAILURE};
		fi

		PYTHON=`which python 2> /dev/null`;

		if test -f "setup.py" && ! run_setup_py_tests ${PYTHON};
		then
			exit ${EXIT_FAILURE};
		fi
	fi
fi

exit ${EXIT_SUCCESS};

