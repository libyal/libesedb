dnl Functions for libnotify
dnl
dnl Version: 20111004

dnl Function to detect if libnotify dependencies are available
AC_DEFUN([AX_LIBNOTIFY_CHECK_LOCAL],
 [dnl Headers included in libnotify/libnotify_print.c
 AC_CHECK_HEADERS([stdarg.h varargs.h])

 AS_IF(
  [test "x$ac_cv_header_stdarg_h" != xyes && test "x$ac_cv_header_varargs_h" != xyes],
  [AC_MSG_FAILURE(
   [Missing headers: stdarg.h and varargs.h],
   [1])
  ])

 dnl Headers included in libnotify/libnotify_stream.c
 AC_CHECK_HEADERS([errno.h])

 ])

