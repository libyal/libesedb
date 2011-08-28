dnl Functions for libnotify

dnl Function to detect if libnotify dependencies are available
AC_DEFUN([AC_CHECK_LOCAL_LIBNOTIFY],
 [dnl Headers included in libnotify/libnotify_print.c
 AC_CHECK_HEADERS([stdarg.h])

 AS_IF(
  [test "x$ac_cv_header_stdarg_h" != xyes],
  [AC_CHECK_HEADERS([varargs.h]) ])

 AS_IF(
  [test "x$ac_cv_header_stdarg_h" != xyes && test "x$ac_cv_header_varargs_h" != xyes],
  [AC_MSG_FAILURE(
   [Missing headers: stdarg.h and varargs.h],
   [1])
  ])

 dnl Headers included in libnotify/libnotify_stream.c
 AC_CHECK_HEADERS([errno.h])

 ])

