dnl Functions for common

dnl Function to detect if common dependencies are available
AC_DEFUN([AC_CHECK_LOCAL_COMMON],
 [dnl Headers included in common/common.h
 AS_IF(
  [test "x$ac_cv_enable_winapi" = xyes],
  [AC_CHECK_HEADERS([windows.h])

  AS_IF(
   [test "x$ac_cv_header_windows_h" = xno],
   [AC_MSG_FAILURE(
    [Missing header: windows.h header is required to compile with winapi support],
    [1])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_enable_winapi" = xno],
  [AC_CHECK_HEADERS([libintl.h]) ])

 dnl Headers included in common/types.h
 AC_CHECK_HEADERS([limits.h])

 dnl Headers included in common/memory.h
 AC_CHECK_HEADERS([stdlib.h string.h])

 dnl File stream functions used in common/file_stream.h
 AC_CHECK_FUNCS([fclose feof fgets fopen fread fseeko fseeko64 fwrite vfprintf])

 AS_IF(
  [test "x$ac_cv_func_fclose" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: fclose],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_feof" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: feof],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_fgets" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: fgets],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_fopen" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: fopen],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_fread" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: fread],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_fseeko" != xyes && test "x$ac_cv_func_fseeko64" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: fseeko and fseeko64],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_fwrite" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: fwrite],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_vfprintf" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: vfprintf],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_enable_wide_character_type" != xno],
  [AC_CHECK_FUNCS([fgetws])

  AS_IF(
   [test "x$ac_cv_func_fgetws" != xyes],
   [AC_MSG_FAILURE(
    [Missing function: fgetws],
    [1])
   ])
  ])

 dnl Memory functions used in common/memory.h
 AC_CHECK_FUNCS([free malloc memcmp memcpy memset realloc])

 AS_IF(
  [test "x$ac_cv_func_free" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: free],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_malloc" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: malloc],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_memcmp" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: memcmp],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_memcpy" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: memcpy],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_memset" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: memset],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_realloc" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: realloc],
   [1])
  ])
 ])

