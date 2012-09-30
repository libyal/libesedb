dnl Functions for libcdata
dnl
dnl Version: 20120902

dnl Function to detect if libcdata is available
dnl ac_libcdata_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBCDATA_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libcdata" != x && test "x$ac_cv_with_libcdata" != xno && test "x$ac_cv_with_libcdata" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libcdata"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libcdata}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libcdata}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libcdata])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libcdata" = xno],
  [ac_cv_libcdata=no],
  [dnl Check for a pkg-config file
  AS_IF(
   [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
   [PKG_CHECK_MODULES(
    [libcdata],
    [libcdata >= 20120425],
    [ac_cv_libcdata=yes],
    [ac_cv_libcdata=no])
   ])

  AS_IF(
   [test "x$ac_cv_libcdata" = xyes],
   [ac_cv_libcdata_CPPFLAGS="$pkg_cv_libcdata_CFLAGS"
   ac_cv_libcdata_LIBADD="$pkg_cv_libcdata_LIBS"],
   [dnl Check for headers
   AC_CHECK_HEADERS([libcdata.h])
 
   AS_IF(
    [test "x$ac_cv_header_libcdata_h" = xno],
    [ac_cv_libcdata=no],
    [dnl Check for the individual functions
    ac_cv_libcdata=yes

    AC_CHECK_LIB(
     cdata,
     libcdata_get_version,
     [ac_cv_libcdata_dummy=yes],
     [ac_cv_libcdata=no])
   
    dnl TODO add functions

    ac_cv_libcdata_LIBADD="-lcdata"
    ])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libcdata" = xyes],
  [AC_DEFINE(
   [HAVE_LIBCDATA],
   [1],
   [Define to 1 if you have the `cdata' library (-lcdata).])
  ])

 AS_IF(
  [test "x$ac_cv_libcdata" = xyes],
  [AC_SUBST(
   [HAVE_LIBCDATA],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBCDATA],
   [0])
  ])
 ])

dnl Function to detect how to enable libcdata
AC_DEFUN([AX_LIBCDATA_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libcdata],
  [libcdata],
  [search for libcdata in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 dnl Check for a shared library version
 AX_LIBCDATA_CHECK_LIB

 dnl Check if the dependencies for the local library version
 AS_IF(
  [test "x$ac_cv_libcdata" != xyes],
  [ac_cv_libcdata_CPPFLAGS="-I../libcdata";
  ac_cv_libcdata_LIBADD="../libcdata/libcdata.la";

  ac_cv_libcdata=local

  AC_DEFINE(
   [HAVE_LOCAL_LIBCDATA],
   [1],
   [Define to 1 if the local version of libcdata is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBCDATA],
   [1])
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBCDATA],
  [test "x$ac_cv_libcdata" = xlocal])
 AS_IF(
  [test "x$ac_cv_libcdata_CPPFLAGS" != "x"],
  [AC_SUBST(
   [LIBCDATA_CPPFLAGS],
   [$ac_cv_libcdata_CPPFLAGS])
  ])
 AS_IF(
  [test "x$ac_cv_libcdata_LIBADD" != "x"],
  [AC_SUBST(
   [LIBCDATA_LIBADD],
   [$ac_cv_libcdata_LIBADD])
  ])

 AS_IF(
  [test "x$ac_cv_libcdata" = xyes],
  [AC_SUBST(
   [ax_libcdata_pc_libs_private],
   [-lcdata])
  ])

 AS_IF(
  [test "x$ac_cv_libcdata" = xyes],
  [AC_SUBST(
   [ax_libcdata_spec_requires],
   [libcdata])
  AC_SUBST(
   [ax_libcdata_spec_build_requires],
   [libcdata-devel])
  ])
 ])

