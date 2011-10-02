dnl Functions for libfguid

dnl Function to detect if libfguid is available
AC_DEFUN([AC_CHECK_LIBFGUID],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libfguid" != x && test "x$ac_cv_with_libfguid" != xno && test "x$ac_cv_with_libfguid" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libfguid"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libfguid}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libfguid}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libfguid])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libfguid" != xno],
  [dnl Check for headers
  AC_CHECK_HEADERS([libfguid.h])
 
  AS_IF(
   [test "x$ac_cv_header_libfguid_h" = xno],
   [ac_cv_libfguid=no],
   [ac_cv_libfguid=yes
   AC_CHECK_LIB(
    fdatetime,
    libfguid_get_version,
    [ac_cv_libfguid_dummy=yes],
    [ac_cv_libfguid=no])
  
   dnl TODO add functions
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libfguid" = xyes],
  [AC_DEFINE(
   [HAVE_LIBFGUID],
   [1],
   [Define to 1 if you have the `fguid' library (-lfguid).])
  LIBS="-lfguid $LIBS"
  ])

 AS_IF(
  [test "x$ac_cv_libfguid" = xyes],
  [AC_SUBST(
   [HAVE_LIBFGUID],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBFGUID],
   [0])
  ])
 ])

