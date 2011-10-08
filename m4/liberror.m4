dnl Functions for liberror
dnl
dnl Version: 20111004

dnl Function to detect if liberror dependencies are available
AC_DEFUN([AX_LIBERROR_CHECK_LOCAL],
 [dnl Headers included in liberror/liberror_error.c
 AC_CHECK_HEADERS([stdarg.h varargs.h])

 AS_IF(
  [test "x$ac_cv_header_stdarg_h" != xyes && test "x$ac_cv_header_varargs_h" != xyes],
  [AC_MSG_FAILURE(
   [Missing headers: stdarg.h and varargs.h],
   [1])
  ])

 dnl Wide character string functions used in liberror/liberror_error.h
 AS_IF(
  [test "x$ac_cv_enable_wide_character_type" != xno],
  [AC_CHECK_FUNCS([wcstombs])

  AS_IF(
   [test "x$ac_cv_func_wcstombs" != xyes],
   [AC_MSG_FAILURE(
    [Missing function: wcstombs],
    [1])
   ])
  ])
 ])

