dnl Functions for liberror

dnl Function to detect if liberror dependencies are available
AC_DEFUN([AC_CHECK_LOCAL_LIBERROR],
 [dnl Headers included in liberror/liberror_error.c
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

 AS_IF(
  [test "x$ac_cv_enable_wide_character_type" != xno],
  [dnl Wide character string functions used in liberror/liberror_error.h
  AC_CHECK_FUNCS(
   [wcstombs],
   [],
   [AC_MSG_FAILURE(
    [Missing function: wcstombs],
    [1])
   ])
  ])
 ])

