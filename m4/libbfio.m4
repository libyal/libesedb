dnl Functions for libbfio
dnl
dnl Version: 20111224

dnl Function to detect if libbfio is available
AC_DEFUN([AX_LIBBFIO_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libbfio" != x && test "x$ac_cv_with_libbfio" != xno && test "x$ac_cv_with_libbfio" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libbfio"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libbfio}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libbfio}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libbfio])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libbfio" = xno],
  [ac_cv_libbfio=no],
  [dnl Check for headers
  AC_CHECK_HEADERS([libbfio.h])
 
  AS_IF(
   [test "x$ac_cv_header_libbfio_h" = xno],
   [ac_cv_libbfio=no],
   [ac_cv_libbfio=yes
   AC_CHECK_LIB(
    bfio,
    libbfio_get_version,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
  
   dnl Handle functions
   AC_CHECK_LIB(
    bfio,
    libbfio_handle_free,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_handle_open,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_handle_close,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_handle_exists,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_handle_read,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_handle_write,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_handle_seek_offset,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
 
   AC_CHECK_LIB(
    bfio,
    libbfio_handle_is_open,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_handle_get_offset,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_handle_get_size,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_handle_set_track_offsets_read,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_handle_get_number_of_offsets_read,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_handle_get_offset_read,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
  
   dnl File functions
   AC_CHECK_LIB(
    bfio,
    libbfio_file_initialize,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_file_get_name_size,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_file_get_name,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_file_set_name,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
  
   AS_IF(
    [test "x$ac_cv_enable_wide_character_type" != xno],
    [AC_CHECK_LIB(
     bfio,
     libbfio_file_get_name_size_wide,
     [ac_cv_libbfio_dummy=yes],
     [ac_cv_libbfio=no])
    AC_CHECK_LIB(
     bfio,
     libbfio_file_get_name_wide,
     [ac_cv_libbfio_dummy=yes],
     [ac_cv_libbfio=no])
    AC_CHECK_LIB(
     bfio,
     libbfio_file_set_name_wide,
     [ac_cv_libbfio_dummy=yes],
     [ac_cv_libbfio=no])
    ])
  
   dnl Pool functions
   AC_CHECK_LIB(
    bfio,
    libbfio_pool_initialize,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_pool_free,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_pool_clone,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_pool_open,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_pool_reopen,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_pool_close,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_pool_close_all,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_pool_read,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_pool_write,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_pool_seek_offset,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
  
   AC_CHECK_LIB(
    bfio,
    libbfio_pool_get_number_of_handles,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_pool_get_handle,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_pool_set_handle,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_pool_append_handle,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_pool_get_offset,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_pool_get_size,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   AC_CHECK_LIB(
    bfio,
    libbfio_pool_set_maximum_number_of_open_handles,
    [ac_cv_libbfio_dummy=yes],
    [ac_cv_libbfio=no])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libbfio" = xyes],
  [AC_DEFINE(
   [HAVE_LIBBFIO],
   [1],
   [Define to 1 if you have the `bfio' library (-lbfio).])
  LIBS="-lbfio $LIBS"
  ])

 AS_IF(
  [test "x$ac_cv_libbfio" = xyes],
  [AC_SUBST(
   [HAVE_LIBBFIO],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBBFIO],
   [0])
  ])
 ])

dnl Function to detect if libbfio dependencies are available
AC_DEFUN([AX_LIBBFIO_CHECK_LOCAL],
 [dnl Headers included in libbfio/libbfio_file.c
 AC_CHECK_HEADERS([errno.h fcntl.h sys/stat.h unistd.h])

 dnl File input/output functions used in libbfio/libbfio_file.h
 AC_CHECK_FUNCS([close fstat ftruncate getcwd lseek open read stat write])

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
  [test "x$ac_cv_func_getcwd" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: getcwd],
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

 dnl Check for error string functions used in libbfio/libbfio_error_string.c
 AC_FUNC_STRERROR_R()

 AS_IF(
  [test "x$ac_cv_have_decl_strerror_r" = xno],
  [AC_CHECK_FUNCS([strerror])

  AS_IF(
   [test "x$ac_cv_func_strerror" != xyes],
   [AC_MSG_FAILURE(
    [Missing functions: strerror and strerror_r],
    [1])
   ])
  ])
 ])

dnl Function to detect how to enable libbfio
AC_DEFUN([AX_LIBBFIO_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libbfio],
  [libbfio],
  [search for libbfio in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 AX_LIBBFIO_CHECK_LIB

 AS_IF(
  [test "x$ac_cv_libbfio" != xyes],
  [AX_LIBBFIO_CHECK_LOCAL

  AC_DEFINE(
   [HAVE_LOCAL_LIBBFIO],
   [1],
   [Define to 1 if the local version of libbfio is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBBFIO],
   [1])
  AC_SUBST(
   [LIBBFIO_CPPFLAGS],
   [-I../libbfio])
  AC_SUBST(
   [LIBBFIO_LIBADD],
   [../libbfio/libbfio.la])

  ac_cv_libbfio=local
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBBFIO],
  [test "x$ac_cv_libbfio" = xlocal])

 AS_IF(
  [test "x$ac_cv_libbfio" = xyes],
  [AC_SUBST(
   [ax_libbfio_pc_libs_private],
   [-lbfio])
  ])

 AS_IF(
  [test "x$ac_cv_libbfio" = xyes],
  [AC_SUBST(
   [ax_libbfio_spec_requires],
   [libbfio])
  AC_SUBST(
   [ax_libbfio_spec_build_requires],
   [libbfio-devel])
  ])
 ])

