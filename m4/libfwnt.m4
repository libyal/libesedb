dnl Functions for libfwnt
dnl
dnl Version: 20111025

dnl Function to detect if libfwnt is available
dnl ac_libfwnt_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBFWNT_CHECK_LIB],
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
  [test "x$ac_cv_with_libfwnt" = xno],
  [ac_cv_libfwnt=no],
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

dnl Function to detect how to enable libfwnt
AC_DEFUN([AX_LIBFWNT_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libfwnt],
  [libfwnt],
  [search for libfwnt in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 AX_LIBFWNT_CHECK_LIB

 AS_IF(
  [test "x$ac_cv_libfwnt" != xyes],
  [AC_DEFINE(
   [HAVE_LOCAL_LIBFWNT],
   [1],
   [Define to 1 if the local version of libfwnt is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBFWNT],
   [1])
  AC_SUBST(
   [LIBFWNT_CPPFLAGS],
   [-I../libfwnt])
  AC_SUBST(
   [LIBFWNT_LIBADD],
   [../libfwnt/libfwnt.la])
  ac_cv_libfwnt=local
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBFWNT],
  [test "x$ac_cv_libfwnt" = xlocal])

 AS_IF(
  [test "x$ac_cv_libfwnt" = xyes],
  [AC_SUBST(
   [ax_libfwnt_pc_libs_private],
   [-lfwnt])
  ])

 AS_IF(
  [test "x$ac_cv_libfwnt" = xyes],
  [AC_SUBST(
   [ax_libfwnt_spec_requires],
   [libfwnt])
  AC_SUBST(
   [ax_libfwnt_spec_build_requires],
   [libfwnt-devel])
  ])
 ])

