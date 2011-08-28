dnl Functions for libcstring

dnl Function to detect whether nl_langinfo supports CODESET
AC_DEFUN([AC_CHECK_FUNC_LANGINFO_CODESET],
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
AC_DEFUN([AC_CHECK_LOCAL_LIBCSTRING],
 [dnl Headers included in libcstring/libcstring_narrow_string.h
 AC_CHECK_HEADERS([stdlib.h string.h])

 dnl Headers included in libcstring/libcstring_locale.c
 AC_CHECK_HEADERS([langinfo.h locale.h])

 dnl Check for environment functions in libcstring/libcstring_locale.c
 AC_CHECK_FUNCS(
  [getenv],
  [],
  [AC_MSG_FAILURE(
   [Missing function: getenv],
   [1]) ])
 
 dnl Check for localization functions in libcstring/libcstring_locale.c
 AS_IF(
  [test "x$ac_cv_enable_winapi" = xno],
  [AC_CHECK_FUNCS(
   [localeconv],
   [],
   [AC_MSG_FAILURE(
    [Missing function: localeconv],
    [1]) ]) ])
 
 AC_CHECK_FUNCS(
  [setlocale],
  [],
  [AC_MSG_FAILURE(
   [Missing function: setlocale],
   [1]) ])
 
 AC_CHECK_FUNC_LANGINFO_CODESET
 
 dnl Narrow character string functions used in libcstring/libcstring_narrow_string.h
 AC_CHECK_FUNCS(
  [fgets],
  [],
  [AC_MSG_FAILURE(
   [Missing function: fgets],
   [1]) ])
 
 AC_CHECK_FUNCS(
  [memchr],
  [],
  [AC_CHECK_FUNCS(
   [strchr],
   [],
   [AC_MSG_FAILURE(
    [Missing functions: memchr and strchr],
    [1]) ]) ])
 
 AC_CHECK_FUNCS(
  [memcmp],
  [],
  [AC_CHECK_FUNCS(
   [strncmp],
   [],
   [AC_MSG_FAILURE(
    [Missing functions: memcmp and strncmp],
    [1]) ]) ])
 
 AC_CHECK_FUNCS(
  [memcpy],
  [],
  [AC_CHECK_FUNCS(
   [strncpy],
   [],
   [AC_MSG_FAILURE(
    [Missing functions: memcpy and strncpy],
    [1]) ]) ])
 
 AC_CHECK_FUNCS(
  [memrchr],
  [AC_CHECK_DECLS(
   [memrchr],
   [ac_cv_memrchr=yes],
   [ac_cv_memrchr=no]) ],
  [ac_cv_memrchr=no])
 
 AS_IF(
  [test "x$ac_cv_memrchr" = xno],
  [AC_CHECK_FUNCS(
   [strrchr],
   [],
   [AC_MSG_FAILURE(
    [Missing functions: strrchr and memrchr],
    [1]) ]) ])
 
 AC_CHECK_FUNCS(
  [snprintf],
  [],
  [AC_MSG_FAILURE(
   [Missing function: snprintf],
   [1]) ])
 
 AC_CHECK_FUNCS(
  [sscanf],
  [],
  [AC_MSG_FAILURE(
   [Missing function: sscanf],
   [1]) ])
 
 AC_CHECK_FUNCS(
  [strlen],
  [],
  [AC_MSG_FAILURE(
   [Missing function: strlen],
   [1]) ])
 
 AC_CHECK_FUNCS(
  [strncasecmp],
  [],
  [AC_CHECK_FUNCS(
   [strcasecmp],
   [],
   [AC_MSG_FAILURE(
    [Missing functions: strncasecmp and strcasecmp],
    [1]) ]) ])
 
 AC_CHECK_FUNCS(
  [strstr],
  [],
  [AC_MSG_FAILURE(
   [Missing function: strstr],
   [1]) ])
 
 AC_CHECK_FUNCS(
  [strtoll],
  [],
  [AC_CHECK_FUNCS(
   [atoll],
   [],
   [AC_MSG_FAILURE(
    [Missing functions: strtoll and atoll],
    [1]) ]) ])
 
 AC_CHECK_FUNCS(
  [strtoull],
  [],
  [AC_CHECK_FUNCS(
   [atoll],
   [],
   [AC_MSG_FAILURE(
    [Missing functions: strtoull and atoll],
    [1]) ]) ])
 
 AC_CHECK_FUNCS(
  [vsnprintf],
  [],
  [AC_MSG_FAILURE(
   [Missing function: vsnprintf],
   [1]) ])

 AS_IF(
  [test "x$ac_cv_enable_wide_character_type" != xno],
  [dnl Wide character string functions used in libcstring/libcstring_wide_string.h
  AC_CHECK_FUNCS(
   [swprintf],
   [],
   [AC_MSG_FAILURE(
    [Missing function: swprintf],
    [1]) ])
 
  AC_CHECK_FUNCS(
   [wmemchr],
   [],
   [AC_CHECK_FUNCS(
    [wcschr],
    [],
    [AC_MSG_FAILURE(
     [Missing functions: wmemchr and wcschr],
     [1]) ]) ])
 
  AC_CHECK_FUNCS(
   [wmemcmp],
   [],
   [AC_CHECK_FUNCS(
    [wcsncmp],
    [],
    [AC_MSG_FAILURE(
     [Missing functions: wmemcmp and wcsncmp],
     [1]) ]) ])
 
  AC_CHECK_FUNCS(
   [wmemcpy],
   [],
   [AC_CHECK_FUNCS(
    [wcsncpy],
    [],
    [AC_MSG_FAILURE(
     [Missing functions: wmemcpy and wcsncpy],
     [1]) ]) ])
 
  AC_CHECK_FUNCS(
   [wmemrchr],
   [],
   [AC_CHECK_FUNCS(
    [wcsrchr],
    [],
    [AC_MSG_FAILURE(
     [Missing functions: wmemrchr and wcsrchr],
     [1]) ]) ])
 
  AC_CHECK_FUNCS(
   [wcslen],
   [],
   [AC_MSG_FAILURE(
    [Missing function: wcslen],
    [1]) ])
  
  AC_CHECK_FUNCS(
   [wcsncasecmp],
   [],
   [AC_CHECK_FUNCS(
    [wcscasecmp],
    [],
    [AC_CHECK_FUNCS(
     [towlower],
     [],
     [AC_MSG_FAILURE(
      [Missing functions: wcsncasecmp, wcscasecmp and towlower],
      [1]) ]) ]) ])
 
  AC_CHECK_FUNCS(
   [wcsstr],
   [],
   [AC_MSG_FAILURE(
    [Missing function: wcsstr],
    [1]) ])
 
  AC_CHECK_FUNCS(
   [wcstoll],
   [],
   [AC_MSG_FAILURE(
    [Missing function: wcstoll],
    [1]) ])
 
  AC_CHECK_FUNCS(
   [wcstoull],
   [],
   [AC_MSG_FAILURE(
    [Missing function: wcstoull],
    [1]) ])
  ])
 ])

