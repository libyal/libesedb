dnl Functions for libfcache
dnl
dnl Version: 20111124

dnl Function to detect if libfcache is available
dnl ac_libfcache_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBFCACHE_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libfcache" != x && test "x$ac_cv_with_libfcache" != xno && test "x$ac_cv_with_libfcache" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libfcache"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libfcache}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libfcache}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libfcache])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libfcache" = xno],
  [ac_cv_libfcache=no],
  [dnl Check for headers
  AC_CHECK_HEADERS([libfcache.h])
 
  AS_IF(
   [test "x$ac_cv_header_libfcache_h" = xno],
   [ac_cv_libfcache=no],
   [ac_cv_libfcache=yes
   AC_CHECK_LIB(
    fcache,
    libfcache_get_version,
    [ac_cv_libfcache_dummy=yes],
    [ac_cv_libfcache=no])
  
   dnl TODO add functions
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libfcache" = xyes],
  [AC_DEFINE(
   [HAVE_LIBFCACHE],
   [1],
   [Define to 1 if you have the `fcache' library (-lfcache).])
  LIBS="-lfcache $LIBS"
  ])

 AS_IF(
  [test "x$ac_cv_libfcache" = xyes],
  [AC_SUBST(
   [HAVE_LIBFCACHE],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBFCACHE],
   [0])
  ])
 ])

dnl Function to detect if libfcache dependencies are available
AC_DEFUN([AX_LIBFCACHE_CHECK_LOCAL],
 [dnl Types used in libfcache/libfcache_date_time.h
 AC_STRUCT_TM

 dnl Headers included in libfcache/libfcache_date_time.h
 AC_HEADER_TIME

 dnl Date and time functions used in libfcache/libfcache_date_time.h
 AC_CHECK_FUNCS(
  [time],
  [],
  [AC_MSG_FAILURE(
   [Missing function: time],
   [1])
  ])
 ])

dnl Function to detect how to enable libfcache
AC_DEFUN([AX_LIBFCACHE_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libfcache],
  [libfcache],
  [search for libfcache in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 AX_LIBFCACHE_CHECK_LIB

 AS_IF(
  [test "x$ac_cv_libfcache" != xyes],
  [AX_LIBFCACHE_CHECK_LOCAL

  AC_DEFINE(
   [HAVE_LOCAL_LIBFCACHE],
   [1],
   [Define to 1 if the local version of libfcache is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBFCACHE],
   [1])
  AC_SUBST(
   [LIBFCACHE_CPPFLAGS],
   [-I../libfcache])
  AC_SUBST(
   [LIBFCACHE_LIBADD],
   [../libfcache/libfcache.la])
  ac_cv_libfcache=local
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBFCACHE],
  [test "x$ac_cv_libfcache" = xlocal])

 AS_IF(
  [test "x$ac_cv_libfcache" = xyes],
  [AC_SUBST(
   [ax_libfcache_pc_libs_private],
   [-lfcache])
  ])

 AS_IF(
  [test "x$ac_cv_libfcache" = xyes],
  [AC_SUBST(
   [ax_libfcache_spec_requires],
   [libfcache])
  AC_SUBST(
   [ax_libfcache_spec_build_requires],
   [libfcache-devel])
  ])
 ])
