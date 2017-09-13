#!/bin/sh
# Script that synchronizes the shared library dependencies
#
# Version: 20170903

GIT_URL_PREFIX="https://github.com/libyal";
SHARED_LIBS="libbfio libcdata libcerror libcfile libclocale libcnotify libcpath libcsplit libcthreads libfcache libfdata libfdatetime libfguid libfmapi libfvalue libfwnt libmapidb libuna";

if test ${TRAVIS_OS_NAME} != "linux";
then
	echo "ERROR: This script is intended to be run on Travis CI.";

	exit 1;
fi

OLDIFS=$IFS;
IFS=" ";

for SHARED_LIB in ${SHARED_LIBS};
do
	git clone ${GIT_URL_PREFIX}/${SHARED_LIB}.git ${SHARED_LIB}-$$;

	if ! test -d ${SHARED_LIB}-$$;
	then
		continue
	fi

	(cd ${SHARED_LIB}-$$ && ./synclibs.sh && ./autogen.sh && ./configure --prefix=/usr && make && sudo make install);

	rm -rf ${SHARED_LIB}-$$;
done

IFS=$OLDIFS;

