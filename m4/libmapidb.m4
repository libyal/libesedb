dnl Functions for libmapidb

dnl Function to detect if libmapidb is available
AC_DEFUN([AC_CHECK_LIBMAPIDB],
 [dnl Check if parameters were provided
 AS_IF(
  [test x"$ac_cv_with_libmapidb" != x && test "x$ac_cv_with_libmapidb" != xno && test "x$ac_cv_with_libmapidb" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libmapidb"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libmapidb}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libmapidb}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libmapidb])
   ])
  ])

 AS_IF(
  [test x"$ac_cv_with_libmapidb" != xno],
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

