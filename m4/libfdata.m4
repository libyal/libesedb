dnl Functions for libfdata

dnl Function to detect if libfdata is available
AC_DEFUN([AC_CHECK_LIBFDATA],
 [dnl Check if parameters were provided
 AS_IF(
  [test x"$ac_cv_with_libfdata" != x && test "x$ac_cv_with_libfdata" != xno && test "x$ac_cv_with_libfdata" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libfdata"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libfdata}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libfdata}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libfdata])
   ])
  ])

 AS_IF(
  [test x"$ac_cv_with_libfdata" != xno],
  [dnl Check for headers
  AC_CHECK_HEADERS([libfdata.h])
 
  AS_IF(
   [test "x$ac_cv_header_libfdata_h" = xno],
   [ac_cv_libfdata=no],
   [ac_cv_libfdata=yes
   AC_CHECK_LIB(
    fdatetime,
    libfdata_get_version,
    [ac_cv_libfdata_dummy=yes],
    [ac_cv_libfdata=no])
  
   dnl TODO add functions
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libfdata" = xyes],
  [AC_DEFINE(
   [HAVE_LIBFDATA],
   [1],
   [Define to 1 if you have the `fdata' library (-lfdata).])
  LIBS="-lfdata $LIBS"
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

