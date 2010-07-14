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
AC_DEFUN([LIBESEDB_CHECK_FUNC_LANGINFO_CODESET],
 [AC_CHECK_FUNCS([nl_langinfo])

 AS_IF(
  [test "x$ac_cv_func_nl_langinfo" = xyes],
  [AC_CACHE_CHECK(
   [for nl_langinfo CODESET support],
   [ac_cv_libesedb_langinfo_codeset],
   [AC_LANG_PUSH(C)
   AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
     [[#include <langinfo.h>]],
     [[char* charset = nl_langinfo( CODESET );]]) ],
    [ac_cv_libesedb_langinfo_codeset=yes],
    [ac_cv_libesedb_langinfo_codeset=no])
   AC_LANG_POP(C) ]) ],
  [ac_cv_libesedb_langinfo_codeset=no])

 AS_IF(
  [test "x$ac_cv_libesedb_langinfo_codeset" = xyes],
  [AC_DEFINE(
   [HAVE_LANGINFO_CODESET],
   [1],
   [Define if nl_langinfo has CODESET support.])
  ])
 ])

dnl Function to detect whether printf conversion specifier "%jd" is available
AC_DEFUN([LIBESEDB_CHECK_FUNC_PRINTF_JD],
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
  [ac_cv_libesedb_have_printf_jd=no],
  [ac_cv_libesedb_have_printf_jd=yes])

 dnl Second try to see if compilation and linkage with a parameter succeeds
 AS_IF(
  [test "x$ac_cv_libesedb_have_printf_jd" = xyes],
  [AC_LINK_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <sys/types.h>
#include <stdio.h>]],
    [[printf( "%jd", (off_t) 10 ); ]] )],
    [ac_cv_libesedb_have_printf_jd=yes],
    [ac_cv_libesedb_have_printf_jd=no])
  ])

 dnl Third try to see if the program runs correctly
 AS_IF(
  [test "x$ac_cv_libesedb_have_printf_jd" = xyes],
  [AC_RUN_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <sys/types.h>
#include <stdio.h>]],
    [[char string[ 3 ];
if( snprintf( string, 3, "%jd", (off_t) 10 ) < 0 ) return( 1 );
if( ( string[ 0 ] != '1' ) || ( string[ 1 ] != '0' ) ) return( 1 ); ]] )],
    [ac_cv_libesedb_have_printf_jd=yes],
    [ac_cv_libesedb_have_printf_jd=no],
    [ac_cv_libesedb_have_printf_jd=undetermined])
   ])

 AC_LANG_POP(C)
 CFLAGS="$SAVE_CFLAGS"

 AS_IF(
  [test "x$ac_cv_libesedb_have_printf_jd" = xyes],
  [AC_MSG_RESULT(
   [yes])
  AC_DEFINE(
   [HAVE_PRINTF_JD],
   [1],
   [Define to 1 whether printf supports the conversion specifier "%jd".]) ],
  [AC_MSG_RESULT(
   [$ac_cv_libesedb_have_printf_jd]) ])
 ])

dnl Function to detect whether printf conversion specifier "%zd" is available
AC_DEFUN([LIBESEDB_CHECK_FUNC_PRINTF_ZD],
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
  [ac_cv_libesedb_have_printf_zd=no],
  [ac_cv_libesedb_have_printf_zd=yes])

 dnl Second try to see if compilation and linkage with a parameter succeeds
 AS_IF(
  [test "x$ac_cv_libesedb_have_printf_zd" = xyes],
  [AC_LINK_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <sys/types.h>
#include <stdio.h>]],
    [[printf( "%zd", (size_t) 10 ); ]] )],
    [ac_cv_libesedb_have_printf_zd=yes],
    [ac_cv_libesedb_have_printf_zd=no])
  ])

 dnl Third try to see if the program runs correctly
 AS_IF(
  [test "x$ac_cv_libesedb_have_printf_zd" = xyes],
  [AC_RUN_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <sys/types.h>
#include <stdio.h>]],
    [[char string[ 3 ];
if( snprintf( string, 3, "%zd", (size_t) 10 ) < 0 ) return( 1 );
if( ( string[ 0 ] != '1' ) || ( string[ 1 ] != '0' ) ) return( 1 ); ]] )],
    [ac_cv_libesedb_have_printf_zd=yes],
    [ac_cv_libesedb_have_printf_zd=no],
    [ac_cv_libesedb_have_printf_zd=undetermined])
   ])

 AC_LANG_POP(C)
 CFLAGS="$SAVE_CFLAGS"

 AS_IF(
  [test "x$ac_cv_libesedb_have_printf_zd" = xyes],
  [AC_MSG_RESULT(
   [yes])
  AC_DEFINE(
   [HAVE_PRINTF_ZD],
   [1],
   [Define to 1 whether printf supports the conversion specifier "%zd".]) ],
  [AC_MSG_RESULT(
   [$ac_cv_libesedb_have_printf_zd]) ])
 ])

dnl Function to detect if ctime_r or ctime is available
dnl Also checks how to use ctime_r
AC_DEFUN([LIBESEDB_CHECK_FUNC_CTIME],
 [AC_CHECK_FUNCS([ctime_r])

 AS_IF(
  [test "x$ac_cv_func_ctime_r" = xyes],
  [AC_MSG_CHECKING(
   [how to use ctime_r])

  AC_LANG_PUSH(C)

  AC_LINK_IFELSE(
   AC_LANG_PROGRAM(
    [[#include <time.h>]],
    [[ctime_r( NULL, NULL, 0 )]]),
    [AC_MSG_RESULT(
     [with additional size argument])
    ac_cv_libesedb_ctime_r_size=yes],
    [ac_cv_libesedb_ctime_r_size=no])

  AS_IF(
   [test "x$ac_cv_libesedb_ctime_r_size" = xno],
   [AC_LINK_IFELSE(
    AC_LANG_PROGRAM(
     [[#include <time.h>]],
     [[ctime_r( NULL, NULL )]]),
    [AC_MSG_RESULT(
     [with two arguments])
    ac_cv_libesedb_ctime_r_posix=yes],
    [ac_cv_libesedb_ctime_r_posix=no])
   ])

  AS_IF(
   [test "x$ac_cv_libesedb_ctime_r_posix" = xno],
   [CPPFLAGS="$CPPFLAGS -D_POSIX_PTHREAD_SEMANTICS"
   AC_LINK_IFELSE(
    AC_LANG_PROGRAM(
     [[#include <time.h>]],
     [[ctime_r( NULL, NULL )]]),
    [AC_MSG_RESULT(
     [with two arguments and definition _POSIX_PTHREAD_SEMANTICS])
     ac_cv_libesedb_ctime_r_posix=yes],
    [ac_cv_libesedb_ctime_r_posix=no])
   ])

  AC_LANG_POP(C)

  AS_IF(
   [test "x$ac_cv_libesedb_ctime_r_size" = xno && test "x$ac_cv_libesedb_ctime_r_posix" = xno],
   [AC_MSG_WARN(
    [unknown])
   ac_cv_func_ctime_r=no])

  AS_IF(
   [test "x$ac_cv_func_ctime_r" = xyes],
   [AC_DEFINE(
    [HAVE_CTIME_R],
    [1],
    [Define to 1 if you have the ctime_r function.])
   ])

  AS_IF(
   [test "x$ac_cv_libesedb_ctime_r_size" = xyes],
   [AC_DEFINE(
    [HAVE_CTIME_R_SIZE],
    [1],
    [Define to 1 if you have the ctime_r function with a third size argument.])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_func_ctime_r" = xno],
  [AC_CHECK_FUNCS([ctime])

  AS_IF(
   [test "x$ac_cv_func_ctime" = xno],
   [AC_MSG_FAILURE(
    [Missing function: ctime_r and ctime],
    [1])
   ])
 ])
])

dnl Function to detect if mkdir is available
dnl Also checks how to use mkdir
AC_DEFUN([LIBESEDB_CHECK_FUNC_MKDIR],
 [AC_CHECK_FUNCS([mkdir])

 AS_IF(
  [test "x$ac_cv_func_mkdir" = xyes],
  [AC_MSG_CHECKING(
   [how to use mkdir])

  SAVE_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS -Wall -Werror"
  AC_LANG_PUSH(C)

  AC_LINK_IFELSE(
   AC_LANG_PROGRAM(
    [[#include <sys/stat.h>
#include <sys/types.h>]],
    [[mkdir( "", 0 )]]),
    [AC_MSG_RESULT(
     [with additional mode argument])
    ac_cv_libesedb_mkdir_mode=yes],
    [ac_cv_libesedb_mkdir_mode=no])

  AS_IF(
   [test "x$ac_cv_libesedb_mkdir_mode" = xno],
   [AC_LINK_IFELSE(
    AC_LANG_PROGRAM(
     [[#include <io.h>]],
     [[mkdir( "" )]]),
    [AC_MSG_RESULT(
     [with single argument])
    ac_cv_libesedb_mkdir=yes],
    [ac_cv_libesedb_mkdir=no])
   ])

  AC_LANG_POP(C)
  CFLAGS="$SAVE_CFLAGS"

  AS_IF(
   [test "x$ac_cv_libesedb_mkdir_mode" = xno && test "x$ac_cv_libesedb_mkdir" = xno],
   [AC_MSG_WARN(
    [unknown])
   ac_cv_func_mkdir=no])

  AS_IF(
   [test "x$ac_cv_func_mkdir" = xyes],
   [AC_DEFINE(
    [HAVE_MKDIR],
    [1],
    [Define to 1 if you have the mkdir function.])
   ])

  AS_IF(
   [test "x$ac_cv_libesedb_mkdir_mode" = xyes],
   [AC_DEFINE(
    [HAVE_MKDIR_MODE],
    [1],
    [Define to 1 if you have the mkdir function with a second mode argument.])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_func_mkdir" = xno],
  [AC_MSG_FAILURE(
   [Missing function: mkdir],
   [1])
 ])
])

dnl Function to detect if libuna available
AC_DEFUN([LIBESEDB_CHECK_LIBUNA],
 [AC_CHECK_HEADERS([libuna.h])

 AS_IF(
  [test "x$ac_cv_header_libuna_h" = xno],
  [ac_libesedb_have_libuna=no],
  [ac_libesedb_have_libuna=yes
  AC_CHECK_LIB(
   una,
   libuna_get_version,
   [],
   [ac_libesedb_have_libuna=no])
 
  dnl Byte stream functions
  AC_CHECK_LIB(
   una,
   libuna_byte_stream_size_from_utf16,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_byte_stream_copy_from_utf16,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_byte_stream_size_from_utf32,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_byte_stream_copy_from_utf32,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_byte_stream_size_from_utf8,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_byte_stream_copy_from_utf8,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])

  dnl UTF-16 string functions
  AC_CHECK_LIB(
   una,
   libuna_utf16_string_size_from_byte_stream,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf16_string_copy_from_byte_stream,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf16_string_size_from_utf16_stream,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf16_string_copy_from_utf16_stream,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf16_string_size_from_utf8,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf16_string_copy_from_utf8,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])

  dnl UTF-32 string functions
  AC_CHECK_LIB(
   una,
   libuna_utf32_string_size_from_byte_stream,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf32_string_copy_from_byte_stream,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf16_string_compare_with_byte_stream,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf32_string_size_from_utf8_stream,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf32_string_copy_from_utf8_stream,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf32_string_size_from_utf16_stream,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf32_string_copy_from_utf16_stream,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf32_string_size_from_utf8,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf32_string_copy_from_utf8,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])

  dnl UTF-8 string functions
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_size_from_byte_stream,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_copy_from_byte_stream,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_compare_with_byte_stream,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_size_from_utf16,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_copy_from_utf16,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_size_from_utf16_stream,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_copy_from_utf16_stream,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_size_from_utf32,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_copy_from_utf32,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_size_from_utf8_stream,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  AC_CHECK_LIB(
   una,
   libuna_utf8_string_copy_from_utf8_stream,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libuna=no])
  ])
 ])

dnl Function to detect if libbfio available
AC_DEFUN([LIBESEDB_CHECK_LIBBFIO],
 [AC_CHECK_HEADERS([libbfio.h])

 AS_IF(
  [test "x$ac_cv_header_libbfio_h" = xno],
  [ac_libesedb_have_libbfio=no],
  [ac_libesedb_have_libbfio=yes
  AC_CHECK_LIB(
   bfio,
   libbfio_get_version,
   [],
   [ac_libesedb_have_libbfio=no])
 
  dnl Handle functions
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_free,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_clone,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_open,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_close,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_read,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_seek_offset,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_is_open,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_set_open_on_demand,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_set_track_offsets_read,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libbfio=no])

  AC_CHECK_LIB(
   bfio,
   libbfio_handle_set_track_offsets_read,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_get_offset_read,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_handle_get_number_of_offsets_read,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libbfio=no])

  dnl File functions
  AC_CHECK_LIB(
   bfio,
   libbfio_file_initialize,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libbfio=no])
  AC_CHECK_LIB(
   bfio,
   libbfio_file_set_name,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libbfio=no])
 
  AS_IF(
   [test "x$ac_cv_libesedb_enable_wide_character_type" != xno],
   [AC_CHECK_LIB(
    bfio,
    libbfio_file_set_name_wide,
    [ac_libesedb_dummy=yes],
    [ac_libesedb_have_libbfio=no]) ])
  ])
 ])

dnl Function to detect if libfdatetime available
AC_DEFUN([LIBESEDB_CHECK_LIBFDATETIME],
 [AC_CHECK_HEADERS([libfdatetime.h])

 AS_IF(
  [test "x$ac_cv_header_libfdatetime_h" = xno],
  [ac_libesedb_have_libfdatetime=no],
  [ac_libesedb_have_libfdatetime=yes
  AC_CHECK_LIB(
   fdatetime,
   libfdatetime_get_version,
   [],
   [ac_libesedb_have_libfdatetime=no])
 
  dnl Filetime functions
  AC_CHECK_LIB(
   fdatetime,
   libfdatetime_filetime_initialize,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libfdatetime=no])
  AC_CHECK_LIB(
   fdatetime,
   libfdatetime_filetime_free,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libfdatetime=no])
  AC_CHECK_LIB(
   fdatetime,
   libfdatetime_filetime_copy_from_byte_stream,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libfdatetime=no])
  AC_CHECK_LIB(
   fdatetime,
   libfdatetime_filetime_copy_to_utf8_string,
   [ac_libesedb_dummy=yes],
   [ac_libesedb_have_libfdatetime=no])
  ])
 ])

dnl Function to detect if libfmapi available
AC_DEFUN([LIBESEDB_CHECK_LIBFMAPI],
 [AC_CHECK_HEADERS([libfmapi.h])

 AS_IF(
  [test "x$ac_cv_header_libfmapi_h" = xno],
  [ac_libesedb_have_libfmapi=no],
  [ac_libesedb_have_libfmapi=yes
  AC_CHECK_LIB(
   fmapi,
   libfmapi_get_version,
   [],
   [ac_libesedb_have_libfmapi=no])
 
  ])
 ])

