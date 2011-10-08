dnl Functions for libfguid
dnl
dnl Version: 20111007

dnl Function to detect if libfguid is available
dnl ac_libfguid_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBFGUID_CHECK_LIB],
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

dnl Function to detect how to enable libfguid
AC_DEFUN([AX_LIBFGUID_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libfguid],
  [libfguid],
  [search for libfguid in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 AX_LIBFGUID_CHECK_LIB

 AS_IF(
  [test "x$ac_cv_libfguid" != xyes],
  [AC_DEFINE(
   [HAVE_LOCAL_LIBFGUID],
   [1],
   [Define to 1 if the local version of libfguid is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBFGUID],
   [1])
  AC_SUBST(
   [LIBFGUID_CPPFLAGS],
   [-I../libfguid])
  AC_SUBST(
   [LIBFGUID_LIBADD],
   [../libfguid/libfguid.la])

  ac_cv_libfguid=local
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBFGUID],
  [test "x$ac_cv_libfguid" = xlocal])

 AS_IF(
  [test "x$ac_cv_libfguid" = xyes],
  [AC_SUBST(
   [ax_libfguid_pc_libs_private],
   [-lfguid])
  ])

 AS_IF(
  [test "x$ac_cv_libfguid" = xyes],
  [AC_SUBST(
   [ax_libfguid_spec_requires],
   [libfguid])
  AC_SUBST(
   [ax_libfguid_spec_build_requires],
   [libfguid-devel])
  ])
 ])

