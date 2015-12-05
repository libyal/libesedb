#!/bin/sh
# Script that synchronizes the local library dependencies
#
# Version: 20150507

GIT_URL_PREFIX="https://github.com/libyal";
LOCAL_LIBS="libbfio libcdata libcerror libcfile libclocale libcnotify libcpath libcsplit libcstring libcsystem libcthreads libfcache libfdata libfdatetime libfguid libfmapi libfvalue libfwnt libmapidb libuna";

OLDIFS=$IFS;
IFS=" ";

for LOCAL_LIB in ${LOCAL_LIBS};
do
	git clone ${GIT_URL_PREFIX}/${LOCAL_LIB}.git ${LOCAL_LIB}-$$;

	if test -d ${LOCAL_LIB}-$$;
	then
		LOCAL_LIB_UPPER=`echo "${LOCAL_LIB}" | tr "[a-z]" "[A-Z]"`;
		LOCAL_LIB_VERSION=`grep -A 2 AC_INIT ${LOCAL_LIB}-$$/configure.ac | tail -n 1 | sed 's/^\s*\[\([0-9]*\)\],\s*$/\1/'`;

		rm -rf ${LOCAL_LIB};
		mkdir ${LOCAL_LIB};

		if test -d ${LOCAL_LIB};
		then
			cp ${LOCAL_LIB}-$$/${LOCAL_LIB}/*.[ch] ${LOCAL_LIB};
			cp ${LOCAL_LIB}-$$/${LOCAL_LIB}/Makefile.am ${LOCAL_LIB}/Makefile.am;

SED_SCRIPT="/AM_CPPFLAGS = / {
	i\\
if HAVE_LOCAL_${LOCAL_LIB_UPPER}
}

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
		i\\
endif
		d
	}
	/${LOCAL_LIB}_la_LDFLAGS/ !{
		N
		b loop1
	}
}

/${LOCAL_LIB}_la_LDFLAGS/ {
	N
	i\\
endif
	d
}

/distclean: clean/ {
	n
	N
	d
}";
			echo "${SED_SCRIPT}" >> ${LOCAL_LIB}-$$.sed;
			sed -i'~' -f ${LOCAL_LIB}-$$.sed ${LOCAL_LIB}/Makefile.am;
			rm -f ${LOCAL_LIB}-$$.sed;

			sed -i'~' "/${LOCAL_LIB}_definitions.h.in/d" ${LOCAL_LIB}/Makefile.am;
			sed -i'~' "/${LOCAL_LIB}.rc/d" ${LOCAL_LIB}/Makefile.am;
			sed -i'~' '/EXTRA_DIST = /d' ${LOCAL_LIB}/Makefile.am;

SED_SCRIPT="/^$/ {
	x
	N
	/endif$/ {
		a\\

		D
	}
}";
			echo "${SED_SCRIPT}" >> ${LOCAL_LIB}-$$.sed;
			sed -i'~' -f ${LOCAL_LIB}-$$.sed ${LOCAL_LIB}/Makefile.am;
			rm -f ${LOCAL_LIB}-$$.sed;

			rm -f ${LOCAL_LIB}/${LOCAL_LIB}.c;

			cp ${LOCAL_LIB}-$$/${LOCAL_LIB}/${LOCAL_LIB}_definitions.h.in ${LOCAL_LIB}/${LOCAL_LIB}_definitions.h;
			sed -i'~' "s/@VERSION@/${LOCAL_LIB_VERSION}/" ${LOCAL_LIB}/${LOCAL_LIB}_definitions.h;
		fi
		rm -rf ${LOCAL_LIB}-$$;
	fi
done

IFS=$OLDIFS;

