dnl Function to test if a certain feature was enabled
AC_DEFUN([LIBESEDB_TEST_ENABLE],
	[AC_ARG_ENABLE(
	 [$1],
	 [AS_HELP_STRING(
	  [--enable-$1],
	  [$3 (default is $4)])],
	 [ac_cv_libesedb_enable_$2=$enableval],
	 [ac_cv_libesedb_enable_$2=$4])dnl
	 AC_CACHE_CHECK(
	  [whether to enable $3],
	  [ac_cv_libesedb_enable_$2],
	  [ac_cv_libesedb_enable_$2=$4])dnl
])

dnl Function to detect whether nl_langinfo supports CODESET
AC_DEFUN([LIBESEDB_LANGINFO_CODESET],
	[AC_CHECK_FUNCS(
	 [nl_langinfo],
	 [AC_CACHE_CHECK(
	  [for nl_langinfo CODESET support],
	  [ac_cv_libesedb_langinfo_codeset],
	  [AC_LANG_PUSH(C)
	  AC_LINK_IFELSE(
	   [AC_LANG_PROGRAM(
	    [[#include <langinfo.h>]],
	    [[char* charset = nl_langinfo( CODESET );]] )],
	   [ac_cv_libesedb_langinfo_codeset=yes],
	   [ac_cv_libesedb_langinfo_codeset=no])
	  AC_LANG_POP(C) ]) ],
	 [ac_cv_libesedb_langinfo_codeset=no] )
	AS_IF(
	 [test "x$ac_cv_libesedb_langinfo_codeset" = xyes],
	 [AC_DEFINE(
	  [HAVE_LANGINFO_CODESET],
	  [1],
	  [Define if nl_langinfo has CODESET support.]) ])
])

dnl Function to detect whether printf conversion specifier "%jd" is available
AC_DEFUN([LIBESEDB_PRINTF_JD],
	[SAVE_CFLAGS="$CFLAGS"
	CFLAGS="$CFLAGS -Wall -Werror"
	AC_LANG_PUSH(C)
	AC_MSG_CHECKING(
	 [whether printf supports the conversion specifier "%jd"])
	dnl First try to see if compilation and linkage without a parameter succeeds
	AC_LINK_IFELSE(
		[AC_LANG_PROGRAM(
		 [[#include <stdio.h>]],
		 [[printf( "%jd" ); ]] )],
		[AC_MSG_RESULT(
		 [no])],
		dnl Second try to see if compilation and linkage with a parameter succeeds
	 	[AC_LINK_IFELSE(
			[AC_LANG_PROGRAM(
			 [[#include <sys/types.h>
#include <stdio.h>]],
		 	 [[printf( "%jd", (off_t) 10 ); ]] )],
			dnl Third try to see if the program runs correctly
	 		 [AC_RUN_IFELSE(
				[AC_LANG_PROGRAM(
				 [[#include <sys/types.h>
#include <stdio.h>]],
				 [[char string[ 3 ];
if( snprintf( string, 3, "%jd", (off_t) 10 ) < 0 ) return( 1 );
if( ( string[ 0 ] != '1' ) || ( string[ 1 ] != '0' ) ) return( 1 ); ]] )],
				[AC_MSG_RESULT(
				 [yes])
				AC_DEFINE(
				 [HAVE_PRINTF_JD],
				 [1],
				 [Define to 1 whether printf supports the conversion specifier "%jd".] )],
				[AC_MSG_RESULT(
				 [no])],
				[AC_MSG_RESULT(
				 [undetermined])] )],
			[AC_MSG_RESULT(
			 [no])] )] )
	AC_LANG_POP(C)
	CFLAGS="$SAVE_CFLAGS"])

dnl Function to detect whether printf conversion specifier "%zd" is available
AC_DEFUN([LIBESEDB_PRINTF_ZD],
	[SAVE_CFLAGS="$CFLAGS"
	 CFLAGS="$CFLAGS -Wall -Werror"
	 AC_LANG_PUSH(C)
	 AC_MSG_CHECKING(
	  [whether printf supports the conversion specifier "%zd"])
	dnl First try to see if compilation and linkage without a parameter succeeds
	AC_LINK_IFELSE(
		[AC_LANG_PROGRAM(
		 [[#include <stdio.h>]],
		 [[printf( "%zd" ); ]] )],
		[AC_MSG_RESULT(
		 [no])],
		dnl Second try to see if compilation and linkage with a parameter succeeds
	 	[AC_LINK_IFELSE(
			[AC_LANG_PROGRAM(
			 [[#include <sys/types.h>
#include <stdio.h>]],
		 	 [[printf( "%zd", (size_t) 10 ); ]] )],
			dnl Third try to see if the program runs correctly
	 		 [AC_RUN_IFELSE(
				[AC_LANG_PROGRAM(
				 [[#include <sys/types.h>
#include <stdio.h>]],
				 [[char string[ 3 ];
if( snprintf( string, 3, "%zd", (size_t) 10 ) < 0 ) return( 1 );
if( ( string[ 0 ] != '1' ) || ( string[ 1 ] != '0' ) ) return( 1 ); ]] )],
				[AC_MSG_RESULT(
				 [yes])
				AC_DEFINE(
				 [HAVE_PRINTF_ZD],
				 [1],
				 [Define to 1 whether printf supports the conversion specifier "%zd".] )],
				[AC_MSG_RESULT(
				 [no])],
				[AC_MSG_RESULT(
				 [undetermined])] )],
			[AC_MSG_RESULT(
			 [no])] )] )
	AC_LANG_POP(C)
	CFLAGS="$SAVE_CFLAGS"])

dnl Function to detect if ctime_r or ctime is available
dnl Also checks if ctime_r is defined according to the POSIX standard
AC_DEFUN([LIBESEDB_CHECK_FUNC_CTIME],
	[AC_CHECK_FUNC(
	 [ctime_r],
	 [AC_LANG_PUSH(C)
	 AC_MSG_CHECKING(
	  [if ctime_r is defined according to the POSIX definition])
	 AC_LINK_IFELSE(
		AC_LANG_PROGRAM(
		 [[#include <time.h>]],
		 [[ctime_r(NULL,NULL,0)]]),
		[AC_MSG_RESULT(
		 [ctime_r with additional size argument detected])
	 	AC_DEFINE(
		 [HAVE_CTIME_R],
		 [1],
		 [Define to 1 if you have the ctime_r function.] )
		AC_DEFINE(
		 [HAVE_CTIME_R_SIZE],
		 [1],
		 [Define to 1 if you have the ctime_r function with a third size argument.] )],
		[AC_LINK_IFELSE(
			AC_LANG_PROGRAM(
			 [[#include <time.h>]],
			 [[ctime_r(NULL,NULL)]]),
			 [AC_MSG_RESULT(
			  [yes])
	 		 AC_DEFINE(
			  [HAVE_CTIME_R],
			  [1],
			  [Define to 1 if you have the ctime_r function.] )],
			 [CPPFLAGS="$CPPFLAGS -D_POSIX_PTHREAD_SEMANTICS"
			 AC_LINK_IFELSE(
				AC_LANG_PROGRAM(
				 [[#include <time.h>]],
				 [[ctime_r(NULL,NULL)]] ),
				[AC_MSG_RESULT(
				 [ctime_r requires additional compile flags])
		 		 AC_DEFINE(
				  [HAVE_CTIME_R],
				  [1],
				  [Define to 1 if you have the ctime_r function.] )],
				[AC_MSG_WARN(
				 [Unable to determine how to compile ctime_r])
				AC_CHECK_FUNCS(
				 [ctime],
				 [],
				 [AC_MSG_FAILURE(
				  [Missing function: ctime_r and ctime],
				  [1]) ]) ])
			])
		])
	AC_LANG_POP(C) ],
	[AC_CHECK_FUNCS(
	 [ctime],
	 [],
	 [AC_MSG_FAILURE(
	  [Missing function: ctime_r and ctime],
	  [1]) ]) ])
])

