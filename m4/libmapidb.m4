dnl Functions for libmapidb
dnl
dnl Version: 20111008

dnl Function to detect if libmapidb is available
AC_DEFUN([AC_CHECK_LIBMAPIDB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libmapidb" != x && test "x$ac_cv_with_libmapidb" != xno && test "x$ac_cv_with_libmapidb" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libmapidb"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libmapidb}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libmapidb}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libmapidb])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libmapidb" != xno],
  [dnl Check for headers
  AC_CHECK_HEADERS([libmapidb.h])
 
  AS_IF(
   [test "x$ac_cv_header_libmapidb_h" = xno],
   [ac_cv_libmapidb=no],
   [ac_cv_libmapidb=yes
   AC_CHECK_LIB(
    fdatetime,
    libmapidb_get_version,
    [],
    [ac_cv_libmapidb=no])
  
   dnl TODO add functions
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libmapidb" = xyes],
  [AC_SUBST(
   [HAVE_LIBMAPIDB],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBMAPIDB],
   [0])
  ])
 ])

dnl Function to detect how to enable libmapidb
AC_DEFUN([AX_LIBMAPIDB_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libmapidb],
  [libmapidb],
  [search for libmapidb in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 AX_LIBMAPIDB_CHECK_LIB

 AS_IF(
  [test "x$ac_cv_libmapidb" != xyes],
  [AC_DEFINE(
   [HAVE_LOCAL_LIBMAPIDB],
   [1],
   [Define to 1 if the local version of libmapidb is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBMAPIDB],
   [1])
  AC_SUBST(
   [LIBMAPIDB_CPPFLAGS],
   [-I../libmapidb])
  AC_SUBST(
   [LIBMAPIDB_LIBADD],
   [../libmapidb/libmapidb.la])
  ac_cv_libmapidb=local
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBMAPIDB],
  [test "x$ac_cv_libmapidb" = xlocal])

 AS_IF(
  [test "x$ac_cv_libmapidb" = xyes],
  [AC_SUBST(
   [ax_libmapidb_pc_libs_private],
   [-lmapidb])
  ])

 AS_IF(
  [test "x$ac_cv_libmapidb" = xyes],
  [AC_SUBST(
   [ax_libmapidb_spec_requires],
   [libmapidb])
  AC_SUBST(
   [ax_libmapidb_spec_build_requires],
   [libmapidb-devel])
  ])
 ])

