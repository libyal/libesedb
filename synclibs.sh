#!/bin/sh
# Script that synchronizes the local library dependencies
#
# Version: 20141001

GIT_URL_PREFIX="https://github.com/libyal";
LOCAL_LIBS="libbfio libcdata libcerror libcfile libclocale libcnotify libcpath libcsplit libcstring libcsystem libcthreads libfcache libfdata libfdatetime libfguid libfmapi libfvalue libfwnt libmapdb libuna";

OLDIFS=$IFS;
IFS=" ";

for LOCAL_LIB in ${LOCAL_LIBS};
do
	git clone ${GIT_URL_PREFIX}/${LOCAL_LIB}.git ${LOCAL_LIB}-$$;

	if [ -d ${LOCAL_LIB}-$$ ];
	then
		LOCAL_LIB_UPPER=`echo "${LOCAL_LIB}" | tr "[a-z]" "[A-Z]"`;
		LOCAL_LIB_VERSION=`grep -A 2 AC_INIT ${LOCAL_LIB}-$$/configure.ac | tail -n 1 | sed 's/^\s*\[\([0-9]*\)\],\s*$/\1/'`;

		rm -rf ${LOCAL_LIB};
		mkdir ${LOCAL_LIB};

		if [ -d ${LOCAL_LIB} ];
		then
			cp ${LOCAL_LIB}-$$/${LOCAL_LIB}/*.[ch] ${LOCAL_LIB};
			cp ${LOCAL_LIB}-$$/${LOCAL_LIB}/Makefile.am ${LOCAL_LIB}/Makefile.am;

SED_SCRIPT="
1i if HAVE_LOCAL_${LOCAL_LIB_UPPER}

/lib_LTLIBRARIES/ {
	s/lib_LTLIBRARIES/noinst_LTLIBRARIES/
}

/${LOCAL_LIB}\.c/ {
	d
}

/${LOCAL_LIB}_la_LIBADD/ {
:loop1
	/${LOCAL_LIB}_la_LDFLAGS/ {
		N
		i endif
		d
	}
	/${LOCAL_LIB}_la_LDFLAGS/ !{
		N
		b loop1
	}
}

/${LOCAL_LIB}_la_LDFLAGS/ {
	N
	i endif
	d
}

/EXTRA_DIST = / {
	N
	N
	N
	N
	d
}

/distclean: clean/ {
	n
	N
	d
}";
			sed "${SED_SCRIPT}" -i ${LOCAL_LIB}/Makefile.am;

SED_SCRIPT="
/^$/ {
	x
	N
	/endif$/ {
		a \

		D
	}
}";
			sed "${SED_SCRIPT}" -i ${LOCAL_LIB}/Makefile.am;

			rm -f ${LOCAL_LIB}/${LOCAL_LIB}.c;

			cp ${LOCAL_LIB}-$$/${LOCAL_LIB}/${LOCAL_LIB}_definitions.h.in ${LOCAL_LIB}/${LOCAL_LIB}_definitions.h;
			sed 's/@VERSION@/${LOCAL_LIB_VERSION}/' -i ${LOCAL_LIB}/${LOCAL_LIB}_definitions.h;
		fi
		rm -rf ${LOCAL_LIB}-$$;
	fi
done

IFS=$OLDIFS;

