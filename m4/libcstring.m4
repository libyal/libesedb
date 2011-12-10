dnl Functions for libcstring
dnl
dnl Version: 201111203

dnl Function to detect whether nl_langinfo supports CODESET
AC_DEFUN([AX_LIBCSTRING_CHECK_FUNC_LANGINFO_CODESET],
 [AC_CHECK_FUNCS([nl_langinfo])

 AS_IF(
  [test "x$ac_cv_func_nl_langinfo" = xyes],
  [AC_CACHE_CHECK(
   [for nl_langinfo CODESET support],
   [ac_cv_cv_langinfo_codeset],
   [AC_LANG_PUSH(C)
   AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
     [[#include <langinfo.h>]],
     [[char* charset = nl_langinfo( CODESET );]] )],
    [ac_cv_cv_langinfo_codeset=yes],
    [ac_cv_cv_langinfo_codeset=no])
   AC_LANG_POP(C) ]) ],
  [ac_cv_cv_langinfo_codeset=no])

 AS_IF(
  [test "x$ac_cv_cv_langinfo_codeset" = xyes],
  [AC_DEFINE(
   [HAVE_LANGINFO_CODESET],
   [1],
   [Define if nl_langinfo has CODESET support.])
  ])
 ])

dnl Function to detect if libcstring dependencies are available
AC_DEFUN([AX_LIBCSTRING_CHECK_LOCAL],
 [dnl Headers included in libcstring/libcstring_narrow_string.h
 AC_CHECK_HEADERS([stdlib.h string.h])

 dnl Headers included in libcstring/libcstring_locale.c
 AC_CHECK_HEADERS([langinfo.h locale.h])

 dnl Check for environment functions in libcstring/libcstring_locale.c
 AC_CHECK_FUNCS([getenv])

 AS_IF(
  [test "x$ac_cv_func_getenv" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: getenv],
   [1])
  ])
 
 dnl Check for localization functions in libcstring/libcstring_locale.c
 AS_IF(
  [test "x$ac_cv_enable_winapi" = xno],
  [AC_CHECK_FUNCS([localeconv])

  AS_IF(
   [test "x$ac_cv_func_localeconv" != xyes],
   [AC_MSG_FAILURE(
    [Missing function: localeconv],
    [1])
   ])
  ])
 
 AC_CHECK_FUNCS([setlocale])

 AS_IF(
  [test "x$ac_cv_func_setlocale" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: setlocale],
   [1])
  ])
 
 AX_LIBCSTRING_CHECK_FUNC_LANGINFO_CODESET
 
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
 ])

