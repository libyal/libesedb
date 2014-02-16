#!/bin/sh
# Script to generate ./configure using the autotools
#
# Version: 20130509

EXIT_SUCCESS=0;
EXIT_FAILURE=1;

ACLOCAL="/usr/bin/aclocal";
AUTOCONF="/usr/bin/autoconf";
AUTOHEADER="/usr/bin/autoheader";
AUTOMAKE="/usr/bin/automake";
AUTOPOINT="/usr/bin/autopoint";
AUTORECONF="/usr/bin/autoreconf";
LIBTOOLIZE="/usr/bin/libtoolize";

if [ -x "${AUTORECONF}" ];
then
	${AUTORECONF} --force --install
else
	if [ ! -x "${ACLOCAL}" ];
	then
		echo "Unable to find: aclocal";

		echo ${EXIT_FAILURE};
	fi

	if [ ! -x "${AUTOCONF}" ];
		then
		echo "Unable to find: autoconf";

		echo ${EXIT_FAILURE};
	fi

	if [ ! -x "${AUTOHEADER}" ];
	then
		echo "Unable to find: autoheader";

		echo ${EXIT_FAILURE};
	fi

	if [ ! -x "${AUTOMAKE}" ];
	then
		echo "Unable to find: automake";

		echo ${EXIT_FAILURE};
	fi

	if [ ! -x "${AUTOPOINT}" ];
	then
		echo "Unable to find: autopoint";

		echo ${EXIT_FAILURE};
	fi

	if [ ! -x "${LIBTOOLIZE}" ];
	then
		echo "Unable to find: libtoolize";

		echo ${EXIT_FAILURE};
	fi

	${AUTOPOINT} --force
	${ACLOCAL} --force -I m4
	${LIBTOOLIZE} --force
	${AUTOHEADER} --force
	${AUTOCONF} --force
	${AUTOMAKE} --force --add-missing
fi

exit ${EXIT_SUCCESS};

