dnl Functions for libfwnt

dnl Function to detect if libfwnt is available
AC_DEFUN([AC_CHECK_LIBFWNT],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libfwnt" != x && test "x$ac_cv_with_libfwnt" != xno && test "x$ac_cv_with_libfwnt" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libfwnt"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libfwnt}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libfwnt}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libfwnt])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libfwnt" != xno],
  [dnl Check for headers
  AC_CHECK_HEADERS([libfwnt.h])
 
  AS_IF(
   [test "x$ac_cv_header_libfwnt_h" = xno],
   [ac_cv_libfwnt=no],
   [ac_cv_libfwnt=yes
   AC_CHECK_LIB(
    fdatetime,
    libfwnt_get_version,
    [ac_cv_libfwnt_dummy=yes],
    [ac_cv_libfwnt=no])
  
   dnl TODO add functions
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libfwnt" = xyes],
  [AC_DEFINE(
   [HAVE_LIBFWNT],
   [1],
   [Define to 1 if you have the `fwnt' library (-lfwnt).])
  LIBS="-lfwnt $LIBS"
  ])

 AS_IF(
  [test "x$ac_cv_libfwnt" = xyes],
  [AC_SUBST(
   [HAVE_LIBFWNT],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBFWNT],
   [0])
  ])
 ])

