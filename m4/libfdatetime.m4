dnl Functions for libfdatetime
dnl
dnl Version: 20111025

dnl Function to detect if libfdatetime is available
dnl ac_libfdatetime_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBFDATETIME_CHECK_LIB],
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
  [test "x$ac_cv_with_libfdatetime" = xno],
  [ac_cv_libfdatetime=no],
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
  
   dnl FAT date time functions
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_fat_date_time_initialize,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_fat_date_time_free,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_fat_date_time_copy_from_byte_stream,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_fat_date_time_copy_from_uint32,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_fat_date_time_copy_to_utf8_string,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_fat_date_time_copy_to_utf16_string,
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
    libfdatetime_filetime_copy_to_utf16_string,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_filetime_add,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])

   dnl NSF timedate functions
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_nsf_timedate_initialize,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_nsf_timedate_free,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_nsf_timedate_copy_from_byte_stream,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_nsf_timedate_copy_from_uint64,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_nsf_timedate_copy_to_utf8_string,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_nsf_timedate_copy_to_utf16_string,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])

   dnl POSIX time functions
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_posix_time_initialize,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_posix_time_free,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_posix_time_copy_from_byte_stream,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_posix_time_copy_from_uint32,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_posix_time_copy_to_utf8_string,
    [ac_cv_libfdatetime_dummy=yes],
    [ac_cv_libfdatetime=no])
   AC_CHECK_LIB(
    fdatetime,
    libfdatetime_posix_time_copy_to_utf16_string,
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

dnl Function to detect how to enable libfdatetime
AC_DEFUN([AX_LIBFDATETIME_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libfdatetime],
  [libfdatetime],
  [search for libfdatetime in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 AX_LIBFDATETIME_CHECK_LIB

 AS_IF(
  [test "x$ac_cv_libfdatetime" != xyes],
  [AC_DEFINE(
   [HAVE_LOCAL_LIBFDATETIME],
   [1],
   [Define to 1 if the local version of libfdatetime is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBFDATETIME],
   [1])
  AC_SUBST(
   [LIBFDATETIME_CPPFLAGS],
   [-I../libfdatetime])
  AC_SUBST(
   [LIBFDATETIME_LIBADD],
   [../libfdatetime/libfdatetime.la])

  ac_cv_libfdatetime=local
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBFDATETIME],
  [test "x$ac_cv_libfdatetime" = xlocal])

 AS_IF(
  [test "x$ac_cv_libfdatetime" = xyes],
  [AC_SUBST(
   [ax_libfdatetime_pc_libs_private],
   [-lfdatetime])
  ])

 AS_IF(
  [test "x$ac_cv_libfdatetime" = xyes],
  [AC_SUBST(
   [ax_libfdatetime_spec_requires],
   [libfdatetime])
  AC_SUBST(
   [ax_libfdatetime_spec_build_requires],
   [libfdatetime-devel])
  ])
 ])

