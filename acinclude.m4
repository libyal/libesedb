dnl Function to test if a certain feature was enabled
AC_DEFUN([COMMON_ARG_ENABLE],
 [AC_ARG_ENABLE(
  [$1],
  [AS_HELP_STRING(
   [--enable-$1],
   [$3 [default is $4]])],
  [ac_cv_enable_$2=$enableval],
  [ac_cv_enable_$2=$4])dnl
  AC_CACHE_CHECK(
   [whether to enable $3],
   [ac_cv_enable_$2],
   [ac_cv_enable_$2=$4])dnl
 ])

dnl Function to test if the location of a certain feature was provided
AC_DEFUN([COMMON_ARG_WITH],
 [AC_ARG_WITH(
  [$1],
  [AS_HELP_STRING(
   [--with-$1=[$5]],
   [$3 [default is $4]])],
  [ac_cv_with_$2=$withval],
  [ac_cv_with_$2=$4])dnl
  AC_CACHE_CHECK(
   [whether to use $3],
   [ac_cv_with_$2],
   [ac_cv_with_$2=$4])dnl
 ])

dnl Function to detect whether WINAPI support should be enabled
AC_DEFUN([AC_CHECK_WINAPI],
 [AS_IF(
  [test "x$ac_cv_enable_winapi" = xauto-detect],
  [ac_check_winapi_target_string="$target"

  AS_IF(
   [test "x$ac_check_winapi_target_string" = x],
   [ac_check_winapi_target_string="$host"])

  AS_CASE(
   [$ac_check_winapi_target_string],
   [*mingw*],[AC_MSG_NOTICE(
              [detected MinGW enabling WINAPI support for cross-compilation])
             ac_cv_enable_winapi=yes],
   [*],[ac_cv_enable_winapi=no])
  ])
 ])

dnl Function to detect whether printf conversion specifier "%jd" is available
AC_DEFUN([AC_CHECK_FUNC_PRINTF_JD],
 [AC_MSG_CHECKING(
  [whether printf supports the conversion specifier "%jd"])

 SAVE_CFLAGS="$CFLAGS"
 CFLAGS="$CFLAGS -Wall -Werror"
 AC_LANG_PUSH(C)

 dnl First try to see if compilation and linkage without a parameter succeeds
 AC_LINK_IFELSE(
  [AC_LANG_PROGRAM(
   [[#include <stdio.h>]],
   [[printf( "%jd" ); ]] )],
  [ac_cv_cv_have_printf_jd=no],
  [ac_cv_cv_have_printf_jd=yes])

 dnl Second try to see if compilation and linkage with a parameter succeeds
 AS_IF(
  [test "x$ac_cv_cv_have_printf_jd" = xyes],
  [AC_LINK_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <sys/types.h>
#include <stdio.h>]],
    [[printf( "%jd", (off_t) 10 ); ]] )],
    [ac_cv_cv_have_printf_jd=yes],
    [ac_cv_cv_have_printf_jd=no])
  ])

 dnl Third try to see if the program runs correctly
 AS_IF(
  [test "x$ac_cv_cv_have_printf_jd" = xyes],
  [AC_RUN_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <sys/types.h>
#include <stdio.h>]],
    [[char string[ 3 ];
if( snprintf( string, 3, "%jd", (off_t) 10 ) < 0 ) return( 1 );
if( ( string[ 0 ] != '1' ) || ( string[ 1 ] != '0' ) ) return( 1 ); ]] )],
    [ac_cv_cv_have_printf_jd=yes],
    [ac_cv_cv_have_printf_jd=no],
    [ac_cv_cv_have_printf_jd=undetermined])
   ])

 AC_LANG_POP(C)
 CFLAGS="$SAVE_CFLAGS"

 AS_IF(
  [test "x$ac_cv_cv_have_printf_jd" = xyes],
  [AC_MSG_RESULT(
   [yes])
  AC_DEFINE(
   [HAVE_PRINTF_JD],
   [1],
   [Define to 1 whether printf supports the conversion specifier "%jd".]) ],
  [AC_MSG_RESULT(
   [$ac_cv_cv_have_printf_jd]) ])
 ])

dnl Function to detect whether printf conversion specifier "%zd" is available
AC_DEFUN([AC_CHECK_FUNC_PRINTF_ZD],
 [AC_MSG_CHECKING(
  [whether printf supports the conversion specifier "%zd"])

 SAVE_CFLAGS="$CFLAGS"
 CFLAGS="$CFLAGS -Wall -Werror"
 AC_LANG_PUSH(C)

 dnl First try to see if compilation and linkage without a parameter succeeds
 AC_LINK_IFELSE(
  [AC_LANG_PROGRAM(
   [[#include <stdio.h>]],
   [[printf( "%zd" ); ]] )],
  [ac_cv_cv_have_printf_zd=no],
  [ac_cv_cv_have_printf_zd=yes])

 dnl Second try to see if compilation and linkage with a parameter succeeds
 AS_IF(
  [test "x$ac_cv_cv_have_printf_zd" = xyes],
  [AC_LINK_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <sys/types.h>
#include <stdio.h>]],
    [[printf( "%zd", (size_t) 10 ); ]] )],
    [ac_cv_cv_have_printf_zd=yes],
    [ac_cv_cv_have_printf_zd=no])
  ])

 dnl Third try to see if the program runs correctly
 AS_IF(
  [test "x$ac_cv_cv_have_printf_zd" = xyes],
  [AC_RUN_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <sys/types.h>
#include <stdio.h>]],
    [[char string[ 3 ];
if( snprintf( string, 3, "%zd", (size_t) 10 ) < 0 ) return( 1 );
if( ( string[ 0 ] != '1' ) || ( string[ 1 ] != '0' ) ) return( 1 ); ]] )],
    [ac_cv_cv_have_printf_zd=yes],
    [ac_cv_cv_have_printf_zd=no],
    [ac_cv_cv_have_printf_zd=undetermined])
   ])

 AC_LANG_POP(C)
 CFLAGS="$SAVE_CFLAGS"

 AS_IF(
  [test "x$ac_cv_cv_have_printf_zd" = xyes],
  [AC_MSG_RESULT(
   [yes])
  AC_DEFINE(
   [HAVE_PRINTF_ZD],
   [1],
   [Define to 1 whether printf supports the conversion specifier "%zd".]) ],
  [AC_MSG_RESULT(
   [$ac_cv_cv_have_printf_zd]) ])
 ])

