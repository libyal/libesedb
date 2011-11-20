dnl Functions for libsystem
dnl
dnl Version: 20111106

dnl Function to detect if ctime_r or ctime is available
dnl Also checks how to use ctime_r
AC_DEFUN([AX_LIBSYSTEM_CHECK_FUNC_CTIME],
 [AC_CHECK_FUNCS([ctime_r])

 AS_IF(
  [test "x$ac_cv_func_ctime_r" = xyes],
  [AC_MSG_CHECKING(
   [how to use ctime_r])

  AC_LANG_PUSH(C)

  AC_LINK_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <time.h>]],
    [[ctime_r( NULL, NULL, 0 )]] )],
    [AC_MSG_RESULT(
     [with additional size argument])
    ac_cv_cv_ctime_r_size=yes],
    [ac_cv_cv_ctime_r_size=no])

  AS_IF(
   [test "x$ac_cv_cv_ctime_r_size" = xno],
   [AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
     [[#include <time.h>]],
     [[ctime_r( NULL, NULL )]] )],
    [AC_MSG_RESULT(
     [with two arguments])
    ac_cv_cv_ctime_r_posix=yes],
    [ac_cv_cv_ctime_r_posix=no])
   ])

  AS_IF(
   [test "x$ac_cv_cv_ctime_r_posix" = xno],
   [CPPFLAGS="$CPPFLAGS -D_POSIX_PTHREAD_SEMANTICS"
   AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
     [[#include <time.h>]],
     [[ctime_r( NULL, NULL )]] )],
    [AC_MSG_RESULT(
     [with two arguments and definition _POSIX_PTHREAD_SEMANTICS])
     ac_cv_cv_ctime_r_posix=yes],
    [ac_cv_cv_ctime_r_posix=no])
   ])

  AC_LANG_POP(C)

  AS_IF(
   [test "x$ac_cv_cv_ctime_r_size" = xno && test "x$ac_cv_cv_ctime_r_posix" = xno],
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
   [test "x$ac_cv_cv_ctime_r_size" = xyes],
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
AC_DEFUN([AX_LIBSYSTEM_CHECK_FUNC_MKDIR],
 [AC_CHECK_FUNCS([mkdir])

 AS_IF(
  [test "x$ac_cv_func_mkdir" = xyes],
  [AC_MSG_CHECKING(
   [how to use mkdir])

  SAVE_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS -Wall -Werror"
  AC_LANG_PUSH(C)

  AC_LINK_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <sys/stat.h>
#include <sys/types.h>]],
    [[mkdir( "", 0 )]] )],
    [AC_MSG_RESULT(
     [with additional mode argument])
    ac_cv_cv_mkdir_mode=yes],
    [ac_cv_cv_mkdir_mode=no])

  AS_IF(
   [test "x$ac_cv_cv_mkdir_mode" = xno],
   [AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
     [[#include <io.h>]],
     [[mkdir( "" )]] )],
    [AC_MSG_RESULT(
     [with single argument])
    ac_cv_cv_mkdir=yes],
    [ac_cv_cv_mkdir=no])
   ])

  AC_LANG_POP(C)
  CFLAGS="$SAVE_CFLAGS"

  AS_IF(
   [test "x$ac_cv_cv_mkdir_mode" = xno && test "x$ac_cv_cv_mkdir" = xno],
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
   [test "x$ac_cv_cv_mkdir_mode" = xyes],
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

dnl Function to detect if libsystem dependencies are available
AC_DEFUN([AX_LIBSYSTEM_CHECK_LOCAL],
 [dnl Types used in libsystem/libsystem_date_time.c
 AC_STRUCT_TM

 dnl Headers included in libsystem/libsystem_file.h, libsystem/libsystem_glob.h,
 dnl libsystem/libsystem_notify.h and libsystem/libsystem_string.h
 AC_CHECK_HEADERS([errno.h])

 dnl Headers included in libsystem/libsystem_directory_io.h, libsystem/libsystem_file_io.h
 AC_CHECK_HEADERS([sys/stat.h])

 dnl Headers included in libsystem/libsystem_file_io.h
 AC_CHECK_HEADERS([fcntl.h unistd.h])

 dnl Headers included in libsystem/libsystem_date_time.h
 AC_HEADER_TIME

 dnl Headers included in libsystem/libsystem_glob.h
 AC_CHECK_HEADERS([glob.h])

 AS_IF(
  [test "x$ac_cv_header_glob_h" = xno],
  [AC_CHECK_HEADERS([io.h])
  ])

 dnl Headers included in libsystem/libsystem_signal.h
 AC_CHECK_HEADERS([signal.h sys/signal.h])

 dnl File input/output functions used in libsystem/libsystem_file_io.h
 AC_CHECK_FUNCS([close fstat ftruncate lseek open read stat write])

 AS_IF(
  [test "x$ac_cv_func_close" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: close],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_fstat" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: fstat],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_ftruncate" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: ftruncate],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_lseek" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: lseek],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_open" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: open],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_read" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: read],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_stat" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: stat],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_write" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: write],
   [1])
  ])

 dnl Date and time functions used in libsystem/libsystem_date_time.c
 AC_CHECK_FUNCS([localtime localtime_r mktime])

 AS_IF(
  [test "x$ac_cv_func_localtime" != xyes && test "x$ac_cv_func_localtime_r" != xyes],
  [AC_MSG_FAILURE(
   [Missing functions: localtime and localtime_r],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_mktime" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: mktime],
   [1])
  ])
 
 dnl Date and time functions used in libsystem/libsystem_date_time.h
 AX_LIBSYSTEM_CHECK_FUNC_CTIME
 
 AC_CHECK_FUNCS([gmtime gmtime_r time])

 AS_IF(
  [test "x$ac_cv_func_gmtime" != xyes && test "x$ac_cv_func_gmtime_r" != xyes],
  [AC_MSG_FAILURE(
   [Missing functions: gmtime_r and gmtime],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_time" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: time],
   [1])
  ])
 
 dnl Directory functions used in libsystem/libsystem_directory_io.h
 AC_CHECK_FUNCS([chdir])
 
 AS_IF(
  [test "x$ac_cv_func_chdir" != xyes],
  [AC_MSG_FAILURE(
   [Missing functions: chdir],
   [1])
  ])
 
 AX_LIBSYSTEM_CHECK_FUNC_MKDIR
 
 dnl Check for error string functions used in libsystem/libsystem_error_string.c
 AC_FUNC_STRERROR_R()
 
 AS_IF(
  [test "x$ac_cv_have_decl_strerror_r" != xyes],
  [AC_CHECK_FUNCS([strerror])

  AS_IF(
   [test "x$ac_cv_func_strerror" != xyes],
   [AC_MSG_FAILURE(
    [Missing functions: strerror_r and strerror],
    [1])
   ])
  ])
 
 dnl Commandline argument/option parsing functions in libsystem/libsystem_getopt.h
 AC_CHECK_FUNCS([getopt])
 
 dnl Path functions used in libsystem/libsystem_path.h
 AC_CHECK_FUNCS([getcwd])
 
 AS_IF(
  [test "x$ac_cv_func_getcwd" != xyes],
  [AC_MSG_FAILURE(
   [Missing functions: getcwd],
   [1])
  ])
 
 dnl Check for IO buffering functions in libsystem/libsystem_support.c
 AC_CHECK_FUNCS([setvbuf])

 dnl Check for internationalization functions in libsystem/libsystem_support.c
 AC_CHECK_FUNCS([bindtextdomain textdomain])

 ])

