dnl Functions for libfdatetime

dnl Function to detect if libfdatetime is available
AC_DEFUN([AC_CHECK_LIBFDATETIME],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libfdatetime" != x && test "x$ac_cv_with_libfdatetime" != xno && test "x$ac_cv_with_libfdatetime" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libfdatetime"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libfdatetime}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libfdatetime}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libfdatetime])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libfdatetime" != xno],
  [dnl Check for headers
  AC_CHECK_HEADERS([libfdatetime.h])
 
  AS_IF(
   [test "x$ac_cv_header_libfdatetime_h" = xno],
   [ac_cv_libfdatetime=no],
   [ac_cv_libfdatetime=yes
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_get_version,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
  
   dnl Filetime functions
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_filetime_initialize,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_filetime_free,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_filetime_copy_from_byte_stream,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_filetime_copy_from_uint64,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_filetime_copy_to_utf8_string,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_filetime_add,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libfdatetime" = xyes],
  [AC_DEFINE(
   [HAVE_LIBFDATETIME],
   [1],
   [Define to 1 if you have the `fdatetime' library (-lfdatetime).])
  LIBS="-lfdatetime $LIBS"
  ])

 AS_IF(
  [test "x$ac_cv_libfdatetime" = xyes],
  [AC_SUBST(
   [HAVE_LIBFDATETIME],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBFDATETIME],
   [0])
  ])
 ])

