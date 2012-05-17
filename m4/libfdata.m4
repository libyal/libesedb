dnl Functions for libfdata
dnl
dnl Version: 20120501

dnl Function to detect if libfdata is available
dnl ac_libfdata_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBFDATA_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libfdata" != x && test "x$ac_cv_with_libfdata" != xno && test "x$ac_cv_with_libfdata" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libfdata"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libfdata}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libfdata}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libfdata])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libfdata" = xno],
  [ac_cv_libfdata=no],
  [dnl Check for a pkg-config file
  AS_IF(
   [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
   [PKG_CHECK_MODULES(
    [libfdata],
    [libfdata >= 20120405],
    [ac_cv_libfdata=yes],
    [ac_cv_libfdata=no])
   ])

  AS_IF(
   [test "x$ac_cv_libfdata" = xyes],
   [ac_cv_libfdata_CPPFLAGS="$pkg_cv_libfdata_CFLAGS"
   ac_cv_libfdata_LIBADD="$pkg_cv_libfdata_LIBS"],
   [dnl Check for headers
   AC_CHECK_HEADERS([libfdata.h])
 
   AS_IF(
    [test "x$ac_cv_header_libfdata_h" = xno],
    [ac_cv_libfdata=no],
    [dnl Check for the individual functions
    ac_cv_libfdata=yes
 
    AC_CHECK_LIB(
     fdata,
     libfdata_get_version,
     [ac_cv_libfdata_dummy=yes],
     [ac_cv_libfdata=no])
   
    dnl TODO add functions
 
    ac_cv_libfdata_LIBADD="-lfdata"
    ])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libfdata" = xyes],
  [AC_DEFINE(
   [HAVE_LIBFDATA],
   [1],
   [Define to 1 if you have the `fdata' library (-lfdata).])
  ])

 AS_IF(
  [test "x$ac_cv_libfdata" = xyes],
  [AC_SUBST(
   [HAVE_LIBFDATA],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBFDATA],
   [0])
  ])
 ])

dnl Function to detect how to enable libfdata
AC_DEFUN([AX_LIBFDATA_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libfdata],
  [libfdata],
  [search for libfdata in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 dnl Check for a shared library version
 AX_LIBFDATA_CHECK_LIB

 dnl Check if the dependencies for the local library version
 AS_IF(
  [test "x$ac_cv_libfdata" != xyes],
  [ac_cv_libfdata_CPPFLAGS="-I../libfdata";
  ac_cv_libfdata_LIBADD="../libfdata/libfdata.la";

  ac_cv_libfdata=local

  AC_DEFINE(
   [HAVE_LOCAL_LIBFDATA],
   [1],
   [Define to 1 if the local version of libfdata is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBFDATA],
   [1])
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBFDATA],
  [test "x$ac_cv_libfdata" = xlocal])
 AS_IF(
  [test "x$ac_cv_libfdata_CPPFLAGS" != "x"],
  [AC_SUBST(
   [LIBFDATA_CPPFLAGS],
   [$ac_cv_libfdata_CPPFLAGS])
  ])
 AS_IF(
  [test "x$ac_cv_libfdata_LIBADD" != "x"],
  [AC_SUBST(
   [LIBFDATA_LIBADD],
   [$ac_cv_libfdata_LIBADD])
  ])

 AS_IF(
  [test "x$ac_cv_libfdata" = xyes],
  [AC_SUBST(
   [ax_libfdata_pc_libs_private],
   [-lfdata])
  ])

 AS_IF(
  [test "x$ac_cv_libfdata" = xyes],
  [AC_SUBST(
   [ax_libfdata_spec_requires],
   [libfdata])
  AC_SUBST(
   [ax_libfdata_spec_build_requires],
   [libfdata-devel])
  ])
 ])
