dnl Functions for libcsystem
dnl
dnl Version: 20130106

dnl Function to detect if libcsystem is available
dnl ac_libcsystem_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBCSYSTEM_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libcsystem" != x && test "x$ac_cv_with_libcsystem" != xno && test "x$ac_cv_with_libcsystem" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libcsystem"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libcsystem}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libcsystem}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libcsystem])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libcsystem" = xno],
  [ac_cv_libcsystem=no],
  [dnl Check for a pkg-config file
  AS_IF(
   [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
   [PKG_CHECK_MODULES(
    [libcsystem],
    [libcsystem >= 20120425],
    [ac_cv_libcsystem=yes],
    [ac_cv_libcsystem=no])
   ])

  AS_IF(
   [test "x$ac_cv_libcsystem" = xyes],
   [ac_cv_libcsystem_CPPFLAGS="$pkg_cv_libcsystem_CFLAGS"
   ac_cv_libcsystem_LIBADD="$pkg_cv_libcsystem_LIBS"],
   [dnl Check for headers
   AC_CHECK_HEADERS([libcsystem.h])
  
   AS_IF(
    [test "x$ac_cv_header_libcsystem_h" = xno],
    [ac_cv_libcsystem=no],
    [dnl Check for the individual functions
    ac_cv_libcsystem=yes
 
    AC_CHECK_LIB(
     csystem,
     libcsystem_get_version,
     [ac_cv_libcsystem_dummy=yes],
     [ac_cv_libcsystem=no])
   
    dnl TODO add functions
 
    ac_cv_libcsystem_LIBADD="-lcsystem"
    ])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libcsystem" = xyes],
  [AC_DEFINE(
   [HAVE_LIBCSYSTEM],
   [1],
   [Define to 1 if you have the `csystem' library (-lcsystem).])
  ])

 AS_IF(
  [test "x$ac_cv_libcsystem" = xyes],
  [AC_SUBST(
   [HAVE_LIBCSYSTEM],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBCSYSTEM],
   [0])
  ])
 ])

dnl Function to detect if libcsystem dependencies are available
AC_DEFUN([AX_LIBCSYSTEM_CHECK_LOCAL],
 [dnl Headers included in libcsystem/libcsystem_file.h, libcsystem/libcsystem_glob.h,
 dnl and libcsystem/libcsystem_notify.h
 AC_CHECK_HEADERS([errno.h])

 dnl Headers included in libcsystem/libcsystem_file_io.h
 AC_CHECK_HEADERS([fcntl.h unistd.h sys/stat.h])

 dnl Headers included in libcsystem/libcsystem_glob.h
 AC_CHECK_HEADERS([glob.h])

 AS_IF(
  [test "x$ac_cv_header_glob_h" = xno],
  [AC_CHECK_HEADERS([io.h])
  ])

 dnl Headers included in libcsystem/libcsystem_signal.h
 AC_CHECK_HEADERS([signal.h sys/signal.h])

 dnl File input/output functions used in libcsystem/libcsystem_file_io.h
 AC_CHECK_FUNCS([close fstat ftruncate lseek open read stat write])

 AS_IF(
  [test "x$ac_cv_func_close" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: close],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_fstat" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: fstat],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_ftruncate" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: ftruncate],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_lseek" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: lseek],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_open" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: open],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_read" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: read],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_stat" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: stat],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_write" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: write],
   [1])
  ])

 dnl Commandline argument/option parsing functions in libcsystem/libcsystem_getopt.h
 AC_CHECK_FUNCS([getopt])
 
 dnl Check for IO buffering functions in libcsystem/libcsystem_support.c
 AC_CHECK_FUNCS([setvbuf])

 dnl Check for internationalization functions in libcsystem/libcsystem_support.c
 AC_CHECK_FUNCS([bindtextdomain textdomain])

 ac_cv_libcsystem_CPPFLAGS="-I../libcsystem";
 ac_cv_libcsystem_LIBADD="../libcsystem/libcsystem.la";

 ac_cv_libcsystem=local
 ])

dnl Function to detect how to enable libcsystem
AC_DEFUN([AX_LIBCSYSTEM_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libcsystem],
  [libcsystem],
  [search for libcsystem in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 dnl Check for a shared library version
 AX_LIBCSYSTEM_CHECK_LIB

 dnl Check if the dependencies for the local library version
 AS_IF(
  [test "x$ac_cv_libcsystem" != xyes],
  [AX_LIBCSYSTEM_CHECK_LOCAL

  AC_DEFINE(
   [HAVE_LOCAL_LIBCSYSTEM],
   [1],
   [Define to 1 if the local version of libcsystem is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBCSYSTEM],
   [1])
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBCSYSTEM],
  [test "x$ac_cv_libcsystem" = xlocal])
 AS_IF(
  [test "x$ac_cv_libcsystem_CPPFLAGS" != "x"],
  [AC_SUBST(
   [LIBCSYSTEM_CPPFLAGS],
   [$ac_cv_libcsystem_CPPFLAGS])
  ])
 AS_IF(
  [test "x$ac_cv_libcsystem_LIBADD" != "x"],
  [AC_SUBST(
   [LIBCSYSTEM_LIBADD],
   [$ac_cv_libcsystem_LIBADD])
  ])

 AS_IF(
  [test "x$ac_cv_libcsystem" = xyes],
  [AC_SUBST(
   [ax_libcsystem_pc_libs_private],
   [-lstring])
  ])

 AS_IF(
  [test "x$ac_cv_libcsystem" = xyes],
  [AC_SUBST(
   [ax_libcsystem_spec_requires],
   [libcsystem])
  AC_SUBST(
   [ax_libcsystem_spec_build_requires],
   [libcsystem-devel])
  ])
 ])

