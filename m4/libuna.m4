dnl Functions for libuna

dnl Function to detect if libuna is available
AC_DEFUN([AC_CHECK_LIBUNA],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libuna" != x && test "x$ac_cv_with_libuna" != xno && test "x$ac_cv_with_libuna" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libuna"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libuna}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libuna}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libuna])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libuna" != xno],
  [dnl Check for headers
  AC_CHECK_HEADERS([libuna.h])

  AS_IF(
   [test "x$ac_cv_header_libuna_h" = xno],
   [ac_cv_libuna=no],
   [ac_cv_libuna=yes
   AC_CHECK_LIB(
    una,
    libuna_get_version,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
  
   dnl Byte stream functions
   AC_CHECK_LIB(
    una,
    libuna_byte_stream_size_from_utf8,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_byte_stream_copy_from_utf8,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_byte_stream_size_from_utf16,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_byte_stream_copy_from_utf16,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_byte_stream_size_from_utf32,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_byte_stream_copy_from_utf32,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
  
   dnl UTF-8 stream functions
   AC_CHECK_LIB(
    una,
    libuna_utf8_stream_size_from_utf8,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf8_stream_copy_from_utf8,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf8_stream_size_from_utf16,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf8_stream_copy_from_utf16,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf8_stream_size_from_utf32,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf8_stream_copy_from_utf32,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
 
   dnl UTF-16 stream functions
   AC_CHECK_LIB(
    una,
    libuna_utf16_stream_size_from_utf8,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf16_stream_copy_from_utf8,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf16_stream_size_from_utf16,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf16_stream_copy_from_utf16,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf16_stream_size_from_utf32,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf16_stream_copy_from_utf32,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
 
   dnl UTF-32 stream functions
   AC_CHECK_LIB(
    una,
    libuna_utf32_stream_size_from_utf8,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf32_stream_copy_from_utf8,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf32_stream_size_from_utf16,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf32_stream_copy_from_utf16,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf32_stream_size_from_utf32,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf32_stream_copy_from_utf32,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
 
   dnl UTF-8 string functions
   AC_CHECK_LIB(
    una,
    libuna_utf8_string_size_from_byte_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf8_string_copy_from_byte_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf8_string_size_from_utf8_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf8_string_copy_from_utf8_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf8_string_size_from_utf16_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf8_string_copy_from_utf16_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf8_string_size_from_utf32_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf8_string_copy_from_utf32_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf8_string_size_from_utf16,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf8_string_copy_from_utf16,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf8_string_size_from_utf32,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf8_string_copy_from_utf32,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
 
   dnl UTF-16 string functions
   AC_CHECK_LIB(
    una,
    libuna_utf16_string_size_from_byte_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf16_string_copy_from_byte_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf16_string_size_from_utf8_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf16_string_copy_from_utf8_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf16_string_size_from_utf16_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf16_string_copy_from_utf16_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf16_string_size_from_utf32_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf16_string_copy_from_utf32_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf16_string_size_from_utf8,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf16_string_copy_from_utf8,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
 
   dnl UTF-32 string functions
   AC_CHECK_LIB(
    una,
    libuna_utf32_string_size_from_byte_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf32_string_copy_from_byte_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf32_string_size_from_utf8_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf32_string_copy_from_utf8_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf32_string_size_from_utf16_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf32_string_copy_from_utf16_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf32_string_size_from_utf32_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf32_string_copy_from_utf32_stream,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf32_string_size_from_utf8,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   AC_CHECK_LIB(
    una,
    libuna_utf32_string_copy_from_utf8,
    [ac_cv_libuna_dummy=yes],
    [ac_cv_libuna=no])
   ])
 ])

 AS_IF(
  [test "x$ac_cv_libuna" = xyes],
  [AC_DEFINE(
   [HAVE_LIBUNA],
   [1],
   [Define to 1 if you have the `una' library (-luna).])
  LIBS="-luna $LIBS"
  ])

 AS_IF(
  [test "x$ac_cv_libuna" = xyes],
  [AC_SUBST(
   [HAVE_LIBUNA],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBUNA],
   [0])
  ])
 ])

