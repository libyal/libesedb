dnl Function to detect if libesedb dependencies are available
AC_DEFUN([AX_LIBESEDB_CHECK_LOCAL],
 [dnl Check for internationalization functions in libesedb/libesedb_i18n.c 
 AC_CHECK_FUNCS([bindtextdomain])
 ])

