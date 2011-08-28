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
 AC_CHECK_FUNCS(
  [fclose],
  [],
  [AC_MSG_FAILURE(
   [Missing function: fclose],
   [1]) ])
 
 AC_CHECK_FUNCS(
  [feof],
  [],
  [AC_MSG_FAILURE(
   [Missing function: feof],
   [1]) ])
 
 AC_CHECK_FUNCS(
  [fgets],
  [],
  [AC_MSG_FAILURE(
   [Missing function: fgets],
   [1]) ])
 
 AS_IF(
  [test "x$ac_cv_enable_wide_character_type" != xno],
  [AC_CHECK_FUNCS(
   [fgetws],
   [],
   [AC_MSG_FAILURE(
    [Missing function: fgetws],
    [1])
   ])
  ])

 AC_CHECK_FUNCS(
  [fopen],
  [],
  [AC_MSG_FAILURE(
   [Missing function: fopen],
   [1]) ])
 
 AC_CHECK_FUNCS(
  [fread],
  [],
  [AC_MSG_FAILURE(
   [Missing function: fread],
   [1]) ])
 
 AC_CHECK_FUNCS(
  [fseeko],
  [],
  [AC_CHECK_FUNCS(
   [fseeko64],
   [],
   [AC_MSG_FAILURE(
    [Missing function: fseeko and fseeko64],
    [1]) ]) ])
 
 AC_CHECK_FUNCS(
  [fwrite],
  [],
  [AC_MSG_FAILURE(
   [Missing function: fwrite],
   [1]) ])
 
 AC_CHECK_FUNCS(
  [vfprintf],
  [],
  [AC_MSG_FAILURE(
   [Missing function: vfprintf],
   [1]) ])
 
 dnl Memory functions used in common/memory.h
 AC_CHECK_FUNCS(
  [free],
  [],
  [AC_MSG_FAILURE(
   [Missing function: free],
   [1]) ])
 
 AC_CHECK_FUNCS(
  [malloc],
  [],
  [AC_MSG_FAILURE(
   [Missing function: malloc],
   [1]) ])
 
 AC_CHECK_FUNCS(
  [memcmp],
  [],
  [AC_MSG_FAILURE(
   [Missing function: memcmp],
   [1]) ])
 
 AC_CHECK_FUNCS(
  [memcpy],
  [],
  [AC_MSG_FAILURE(
   [Missing function: memcpy],
   [1]) ])
 
 AC_CHECK_FUNCS(
  [memset],
  [],
  [AC_MSG_FAILURE(
   [Missing function: memset],
   [1]) ])
 
 dnl TODO re-enable in the future
 dnl AC_CHECK_FUNCS([posix_memalign])
 
 AC_CHECK_FUNCS(
  [realloc],
  [],
  [AC_MSG_FAILURE(
   [Missing function: realloc],
   [1]) ])
 
 ])

