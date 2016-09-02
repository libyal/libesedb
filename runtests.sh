#!/bin/bash
# Script that runs the tests
#
# Version: 20160824

EXIT_SUCCESS=0;
EXIT_FAILURE=1;

run_configure_make()
{
	local CONFIGURE_OPTIONS=$@;

	./configure ${CONFIGURE_OPTIONS[@]};
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo "Running: './configure' failed";

		return ${RESULT};
	fi

	make clean > /dev/null;
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo "Running: 'make clean' failed";

		return ${RESULT};
	fi

	make > /dev/null;
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo "Running: 'make' failed";

		return ${RESULT};
	fi
	return ${EXIT_SUCCESS};
}

run_configure_make_check()
{
	run_configure_make $@;
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		return ${RESULT};
	fi

	make check;
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo "Running: 'make check' failed";

		if test -f tests/test-suite.log;
		then
			cat tests/test-suite.log;
		fi

		return ${RESULT};
	fi
	return ${EXIT_SUCCESS};
}

run_configure_make_check_with_coverage()
{
	# Disable optimization so we can hook malloc and realloc.
	export CFLAGS="--coverage -O0";
	export LDFLAGS="--coverage";

	# Disable creating a shared library so we can hook memset.
	run_configure_make_check "--enable-shared=no --enable-wide-character-type";
	RESULT=$?;

	export CFLAGS=;
	export LDFLAGS=;

	return ${RESULT};
}

run_configure_make_check_python()
{
	run_configure_make $@;
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		return ${RESULT};
	fi

	make check SKIP_LIBRARY_TESTS=1 SKIP_TOOLS_TESTS=1;
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo "Running: 'make check' failed";

		if test -f tests/test-suite.log;
		then
			cat tests/test-suite.log;
		fi

		return ${RESULT};
	fi
	return ${EXIT_SUCCESS};
}

run_setup_py_tests()
{
	PYTHON=$1;

	${PYTHON} setup.py build;
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo "Running: 'setup.py build' failed";

		return ${RESULT};
	fi
	return ${EXIT_SUCCESS};
}

run_configure_make_check;
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

./configure --help | grep -- '--with-zlib' > /dev/null;

HAVE_WITH_ZLIB=$?;

if test ${HAVE_WITH_ZLIB} -eq 0;
then
	run_configure_make_check "--with-zlib=no";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		exit ${EXIT_FAILURE};
	fi
fi

./configure --help | grep -- '--with-openssl' > /dev/null;

HAVE_WITH_OPENSSL=$?;

if test ${HAVE_WITH_OPENSSL} -eq 0;
then
	run_configure_make_check "--with-openssl=no";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		exit ${EXIT_FAILURE};
	fi

	run_configure_make_check "--enable-openssl-evp-cipher=no --enable-openssl-evp-md=no";
	RESULT=$?;

	if test ${RESULT} -ne ${EXIT_SUCCESS};
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

		run_configure_make_check_python "--enable-python";
		RESULT=$?;

		if test ${RESULT} -ne ${EXIT_SUCCESS};
		then
			exit ${EXIT_FAILURE};
		fi
		export PYTHON_VERSION=;

		run_configure_make "--enable-python2";
		RESULT=$?;

		if test ${RESULT} -ne ${EXIT_SUCCESS};
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

		run_configure_make_check_python "--enable-python";
		RESULT=$?;

		if test ${RESULT} -ne ${EXIT_SUCCESS};
		then
			exit ${EXIT_FAILURE};
		fi
		export PYTHON_VERSION=;

		run_configure_make "--enable-python3";
		RESULT=$?;

		if test ${RESULT} -ne ${EXIT_SUCCESS};
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
		run_configure_make_check_python "--enable-python";
		RESULT=$?;

		if test ${RESULT} -ne ${EXIT_SUCCESS};
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

run_configure_make_check_with_coverage;
RESULT=$?;

if test ${RESULT} -ne ${EXIT_SUCCESS};
then
	exit ${EXIT_FAILURE};
fi

exit ${EXIT_SUCCESS};

