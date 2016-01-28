#!/bin/sh
# Script to generate ./configure using the autotools
#
# Version: 20160106

EXIT_SUCCESS=0;
EXIT_FAILURE=1;

BINDIR="/usr/bin";

if ! test -x "${BINDIR}/pkg-config";
then
	BINDIR="/usr/local/bin";
fi
if ! test -x "${BINDIR}/pkg-config";
then
	BINDIR="/usr/local/bin";
fi
if ! test -x "${BINDIR}/pkg-config";
then
	# Default location of MacPorts installed binaries.
	BINDIR="/opt/local/bin";
fi
if ! test -x "${BINDIR}/pkg-config";
then
	# Default location of MSYS-MinGW installed binaries.
	BINDIR="/mingw/bin";
fi

PKGCONFIG="${BINDIR}/pkg-config";

if ! test -x "${PKGCONFIG}";
then
	echo "Unable to find: pkg-config";

	exit ${EXIT_FAILURE};
fi

ACLOCAL="${BINDIR}/aclocal";
AUTOCONF="${BINDIR}/autoconf";
AUTOHEADER="${BINDIR}/autoheader";
AUTOMAKE="${BINDIR}/automake";
AUTOPOINT="${BINDIR}/autopoint";
AUTORECONF="${BINDIR}/autoreconf";
LIBTOOLIZE="${BINDIR}/libtoolize";

if test -x "${AUTORECONF}";
then
	${AUTORECONF} --force --install
	if test $? -ne 0;
	then
		exit $?;
	fi
else
	if ! test -x "${ACLOCAL}";
	then
		echo "Unable to find: aclocal";

		exit ${EXIT_FAILURE};
	fi

	if ! test -x "${AUTOCONF}";
		then
		echo "Unable to find: autoconf";

		exit ${EXIT_FAILURE};
	fi

	if ! test -x "${AUTOHEADER}";
	then
		echo "Unable to find: autoheader";

		exit ${EXIT_FAILURE};
	fi

	if ! test -x "${AUTOMAKE}";
	then
		echo "Unable to find: automake";

		exit ${EXIT_FAILURE};
	fi

	if ! test -x "${AUTOPOINT}";
	then
		echo "Unable to find: autopoint";

		exit ${EXIT_FAILURE};
	fi

	if ! test -x "${LIBTOOLIZE}";
	then
		echo "Unable to find: libtoolize";

		exit ${EXIT_FAILURE};
	fi

	${AUTOPOINT} --force;
	if test $? -ne 0;
	then
		exit $?;
	fi

	${ACLOCAL} --force -I m4;
	if test $? -ne 0;
	then
		exit $?;
	fi

	${LIBTOOLIZE} --force;
	if test $? -ne 0;
	then
		exit $?;
	fi

	${AUTOHEADER} --force;
	if test $? -ne 0;
	then
		exit $?;
	fi

	${AUTOCONF} --force;
	if test $? -ne 0;
	then
		exit $?;
	fi

	${AUTOMAKE} --force --add-missing;
	if test $? -ne 0;
	then
		exit $?;
	fi

fi

exit ${EXIT_SUCCESS};

