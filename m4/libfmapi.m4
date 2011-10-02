dnl Functions for libfmapi

dnl Function to detect if libfmapi is available
AC_DEFUN([AC_CHECK_LIBFMAPI],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libfmapi" != x && test "x$ac_cv_with_libfmapi" != xno && test "x$ac_cv_with_libfmapi" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libfmapi"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libfmapi}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libfmapi}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libfmapi])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libfmapi" != xno],
  [dnl Check for headers
  AC_CHECK_HEADERS([libfmapi.h])
 
  AS_IF(
   [test "x$ac_cv_header_libfmapi_h" = xno],
   [ac_cv_libfmapi=no],
   [ac_cv_libfmapi=yes
   AC_CHECK_LIB(
    fdatetime,
    libfmapi_get_version,
    [ac_cv_libfmapi_dummy=yes],
    [ac_cv_libfmapi=no])
  
   dnl TODO add functions
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libfmapi" = xyes],
  [AC_DEFINE(
   [HAVE_LIBFMAPI],
   [1],
   [Define to 1 if you have the `fmapi' library (-lfmapi).])
  LIBS="-lfmapi $LIBS"
  ])

 AS_IF(
  [test "x$ac_cv_libfmapi" = xyes],
  [AC_SUBST(
   [HAVE_LIBFMAPI],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBFMAPI],
   [0])
  ])
 ])

