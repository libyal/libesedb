dnl Functions for libfmapi
dnl
dnl Version: 20111008

dnl Function to detect if libfmapi is available
dnl ac_libfmapi_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBFMAPI_CHECK_LIB],
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

dnl Function to detect how to enable libfmapi
AC_DEFUN([AX_LIBFMAPI_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libfmapi],
  [libfmapi],
  [search for libfmapi in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 AX_LIBFMAPI_CHECK_LIB

 AS_IF(
  [test "x$ac_cv_libfmapi" != xyes],
  [AC_DEFINE(
   [HAVE_LOCAL_LIBFMAPI],
   [1],
   [Define to 1 if the local version of libfmapi is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBFMAPI],
   [1])
  AC_SUBST(
   [LIBFMAPI_CPPFLAGS],
   [-I../libfmapi])
  AC_SUBST(
   [LIBFMAPI_LIBADD],
   [../libfmapi/libfmapi.la])
  ac_cv_libfmapi=local
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBFMAPI],
  [test "x$ac_cv_libfmapi" = xlocal])

 AS_IF(
  [test "x$ac_cv_libfmapi" = xyes],
  [AC_SUBST(
   [ax_libfmapi_pc_libs_private],
   [-lfmapi])
  ])

 AS_IF(
  [test "x$ac_cv_libfmapi" = xyes],
  [AC_SUBST(
   [ax_libfmapi_spec_requires],
   [libfmapi])
  AC_SUBST(
   [ax_libfmapi_spec_build_requires],
   [libfmapi-devel])
  ])
 ])

