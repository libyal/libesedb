dnl Functions for libcstring
dnl
dnl Version: 20120825

dnl Function to detect if libcstring is available
dnl ac_libcstring_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBCSTRING_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libcstring" != x && test "x$ac_cv_with_libcstring" != xno && test "x$ac_cv_with_libcstring" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libcstring"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libcstring}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libcstring}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libcstring])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libcstring" = xno],
  [ac_cv_libcstring=no],
  [AS_IF(
   [test "x$ac_cv_header_libcstring_h" = xno],
   [ac_cv_libcstring=no],
   [dnl Check for a pkg-config file
   AS_IF(
    [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
    [PKG_CHECK_MODULES(
     [libcstring],
     [libcstring >= 20120425],
     [ac_cv_libcstring=yes],
     [ac_cv_libcstring=no])
    ])
   ])

  AS_IF(
   [test "x$ac_cv_libcstring" = xyes],
   [ac_cv_libcstring_CPPFLAGS="$pkg_cv_libcstring_CFLAGS"
   ac_cv_libcstring_LIBADD="$pkg_cv_libcstring_LIBS"],
   [dnl Check for headers
   AC_CHECK_HEADERS([libcstring.h])
 
   AS_IF(
    [test "x$ac_cv_header_libcstring_h" = xno],
    [ac_cv_libcstring=no],
    [dnl Check for the individual functions
    ac_cv_libcstring=yes

    AC_CHECK_LIB(
     cstring,
     libcstring_get_version,
     [ac_cv_libcstring_dummy=yes],
     [ac_cv_libcstring=no])
  
    dnl TODO add functions

    ac_cv_libcstring_LIBADD="-lcstring"
    ])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libcstring" = xyes],
  [AC_DEFINE(
   [HAVE_LIBCSTRING],
   [1],
   [Define to 1 if you have the `cstring' library (-lcstring).])
  ])

 AS_IF(
  [test "x$ac_cv_libcstring" = xyes],
  [AC_SUBST(
   [HAVE_LIBCSTRING],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBCSTRING],
   [0])
  ])
 ])

dnl Function to detect if libcstring dependencies are available
AC_DEFUN([AX_LIBCSTRING_CHECK_LOCAL],
 [dnl Headers included in libcstring/libcstring_narrow_string.h
 AC_CHECK_HEADERS([stdlib.h string.h])

 dnl Headers included in libcstring/libcstring_wide.h
 AC_CHECK_HEADERS([wchar.h wctype.h])

 dnl Narrow character string functions used in libcstring/libcstring_narrow_string.h
 AC_CHECK_FUNCS([fgets memchr memcmp memcpy memrchr snprintf sscanf strcasecmp strchr strlen strncasecmp strncmp strncpy strrchr strstr vsnprintf])

 AS_IF(
  [test "x$ac_cv_func_fgets" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: fgets],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_memchr" != xyes && test "x$ac_cv_func_strchr" != xyes],
  [AC_MSG_FAILURE(
   [Missing functions: memchr and strchr],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_memcmp" != xyes && test "x$ac_cv_func_strncmp" != xyes],
  [AC_MSG_FAILURE(
   [Missing functions: memcmp and strncmp],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_memcpy" != xyes && test "x$ac_cv_func_strncpy" != xyes],
  [AC_MSG_FAILURE(
   [Missing functions: memcpy and strncpy],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_memrchr" = xyes],
  [AC_CHECK_DECLS([memrchr])

  AS_IF(
   [test "x$ac_cv_decl_memrchr" != xyes],
   [ac_cv_func_memrchr=no])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_memrchr" != xyes && test "x$ac_cv_func_strrchr" != xyes],
  [AC_MSG_FAILURE(
   [Missing functions: strrchr and memrchr],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_snprintf" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: snprintf],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_sscanf" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: sscanf],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_strlen" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: strlen],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_strcasecmp" != xyes && test "x$ac_cv_func_strncasecmp" != xyes],
  [AC_MSG_FAILURE(
   [Missing functions: strncasecmp and strcasecmp],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_strstr" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: strstr],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_strstr" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: vsnprintf],
   [1])
  ])

 dnl Wide character string functions used in libcstring/libcstring_wide_string.h
 AS_IF(
  [test "x$ac_cv_enable_wide_character_type" != xno],
  [AC_CHECK_FUNCS([swprintf towlower wcscasecmp wcschr wcslen wcsncasecmp wcsncmp wcsncpy wcsrchr wcsstr wmemchr wmemcmp wmemcpy wmemrchr])

  AS_IF(
   [test "x$ac_cv_func_swprintf" != xyes],
   [AC_MSG_FAILURE(
    [Missing function: swprintf],
    [1])
   ])
 
  AS_IF(
   [test "x$ac_cv_func_wmemchr" != xyes && test "x$ac_cv_func_wcschr" != xyes],
   [AC_MSG_FAILURE(
    [Missing functions: wmemchr and wcschr],
    [1])
   ])
 
  AS_IF(
   [test "x$ac_cv_func_wmemcmp" != xyes && test "x$ac_cv_func_wcsncmp" != xyes],
   [AC_MSG_FAILURE(
    [Missing functions: wmemcmp and wcsncmp],
    [1])
   ])
 
  AS_IF(
   [test "x$ac_cv_func_wmemcpy" != xyes && test "x$ac_cv_func_wcsncpy" != xyes],
   [AC_MSG_FAILURE(
    [Missing functions: wmemcpy and wcsncpy],
    [1])
   ])
 
  AS_IF(
   [test "x$ac_cv_func_wmemrchr" != xyes && test "x$ac_cv_func_wcsrchr" != xyes],
   [AC_MSG_FAILURE(
    [Missing functions: wmemrchr and wcsrchr],
    [1])
   ])
 
  AS_IF(
   [test "x$ac_cv_func_wcslen" != xyes],
   [AC_MSG_FAILURE(
    [Missing function: wcslen],
    [1])
   ])
  
  AS_IF(
   [test "x$ac_cv_func_wcsncasecmp" != xyes && test "x$ac_cv_func_wcscasecmp" != xyes && test "x$ac_cv_func_towlower" != xyes],
   [AC_MSG_FAILURE(
    [Missing functions: wcsncasecmp, wcscasecmp and towlower],
    [1])
   ])
 
  AS_IF(
   [test "x$ac_cv_func_wcsstr" != xyes],
   [AC_MSG_FAILURE(
    [Missing function: wcsstr],
    [1])
   ])
  ])

 ac_cv_libcstring_CPPFLAGS="-I../libcstring";
 ac_cv_libcstring_LIBADD="../libcstring/libcstring.la";

 ac_cv_libcstring=local
 ])

dnl Function to detect how to enable libcstring
AC_DEFUN([AX_LIBCSTRING_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libcstring],
  [libcstring],
  [search for libcstring in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 dnl Check for a shared library version
 AX_LIBCSTRING_CHECK_LIB

 dnl Check if the dependencies for the local library version
 AS_IF(
  [test "x$ac_cv_libcstring" != xyes],
  [AX_LIBCSTRING_CHECK_LOCAL

  AC_DEFINE(
   [HAVE_LOCAL_LIBCSTRING],
   [1],
   [Define to 1 if the local version of libcstring is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBCSTRING],
   [1])
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBCSTRING],
  [test "x$ac_cv_libcstring" = xlocal])
 AS_IF(
  [test "x$ac_cv_libcstring_CPPFLAGS" != "x"],
  [AC_SUBST(
   [LIBCSTRING_CPPFLAGS],
   [$ac_cv_libcstring_CPPFLAGS])
  ])
 AS_IF(
  [test "x$ac_cv_libcstring_LIBADD" != "x"],
  [AC_SUBST(
   [LIBCSTRING_LIBADD],
   [$ac_cv_libcstring_LIBADD])
  ])

 AS_IF(
  [test "x$ac_cv_libcstring" = xyes],
  [AC_SUBST(
   [ax_libcstring_pc_libs_private],
   [-lcstring])
  ])

 AS_IF(
  [test "x$ac_cv_libcstring" = xyes],
  [AC_SUBST(
   [ax_libcstring_spec_requires],
   [libcstring])
  AC_SUBST(
   [ax_libcstring_spec_build_requires],
   [libcstring-devel])
  ])
 ])

