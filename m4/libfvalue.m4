dnl Functions for libfvalue
dnl
dnl Version: 20111025

dnl Function to detect if libfvalue available
dnl ac_libfvalue_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBFVALUE_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libfvalue" != x && test "x$ac_cv_with_libfvalue" != xno && test "x$ac_cv_with_libfvalue" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libfvalue"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libfvalue}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libfvalue}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libfvalue])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libfvalue" = xno],
  [ac_cv_libfvalue=no],
  [dnl Check for headers
  AC_CHECK_HEADERS([libfvalue.h])
 
  AS_IF(
   [test "x$ac_cv_header_libfvalue_h" = xno],
   [ac_cv_libfvalue=no],
   [ac_cv_libfvalue=yes
   AC_CHECK_LIB(
    fvalue,
    libfvalue_get_version,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
  
   dnl UTF-8 string functions
   AC_CHECK_LIB(
    fvalue,
    libfvalue_utf8_string_decimal_copy_from_8bit,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_utf8_string_decimal_copy_to_8bit,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_utf8_string_decimal_copy_from_16bit,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_utf8_string_decimal_copy_to_16bit,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_utf8_string_decimal_copy_from_32bit,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_utf8_string_decimal_copy_to_32bit,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_utf8_string_decimal_copy_from_64bit,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_utf8_string_decimal_copy_to_64bit,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_utf8_string_hexadecimal_copy_to_64bit,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])

   AC_CHECK_LIB(
    fvalue,
    libfvalue_utf8_string_split,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])

   dnl UTF-16 string functions
   AC_CHECK_LIB(
    fvalue,
    libfvalue_utf16_string_decimal_copy_from_8bit,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_utf16_string_decimal_copy_from_16bit,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
  
   dnl Split string functions
   AC_CHECK_LIB(
    fvalue,
    libfvalue_split_utf8_string_free,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_split_utf8_string_get_number_of_segments,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_split_utf8_string_get_segment_by_index,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])

   dnl Table functions
   AC_CHECK_LIB(
    fvalue,
    libfvalue_table_initialize,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_table_free,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_table_clone,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_table_get_number_of_values,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_table_get_value_by_identifier,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_table_get_value_by_index,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_table_set_value,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_table_set_value_by_index,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_table_copy_from_utf8_xml_string,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
  
   dnl Value functions
   AC_CHECK_LIB(
    fvalue,
    libfvalue_value_initialize,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_value_free,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_value_clone,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_value_get_identifier,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_value_set_identifier,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_value_get_data,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_value_set_data,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_value_has_data,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
 
   AC_CHECK_LIB(
    fvalue,
    libfvalue_value_copy_from_64bit,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_value_copy_to_64bit,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
 
   AC_CHECK_LIB(
    fvalue,
    libfvalue_value_get_utf8_string_size,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_value_copy_from_utf8_string,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_value_copy_to_utf8_string,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_value_get_utf16_string_size,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_value_copy_from_utf16_string,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   AC_CHECK_LIB(
    fvalue,
    libfvalue_value_copy_to_utf16_string,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
 
   AC_CHECK_LIB(
    fvalue,
    libfvalue_value_write_to_file_stream,
    [ac_cv_libfvalue_dummy=yes],
    [ac_cv_libfvalue=no])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libfvalue" = xyes],
  [AC_DEFINE(
   [HAVE_LIBFVALUE],
   [1],
   [Define to 1 if you have the `fvalue' library (-lfvalue).])
  LIBS="-lfvalue $LIBS"
  ])

 AS_IF(
  [test "x$ac_cv_libfvalue" = xyes],
  [AC_SUBST(
   [HAVE_LIBFVALUE],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBFVALUE],
   [0])
  ])
 ])

dnl Function to detect how to enable libfvalue
AC_DEFUN([AX_LIBFVALUE_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libfvalue],
  [libfvalue],
  [search for libfvalue in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 AX_LIBFVALUE_CHECK_LIB

 AS_IF(
  [test "x$ac_cv_libfvalue" != xyes],
  [AC_DEFINE(
   [HAVE_LOCAL_LIBFVALUE],
   [1],
   [Define to 1 if the local version of libfvalue is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBFVALUE],
   [1])
  AC_SUBST(
   [LIBFVALUE_CPPFLAGS],
   [-I../libfvalue])
  AC_SUBST(
   [LIBFVALUE_LIBADD],
   [../libfvalue/libfvalue.la])

  ac_cv_libfvalue=local
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBFVALUE],
  [test "x$ac_cv_libfvalue" = xlocal])

 AS_IF(
  [test "x$ac_cv_libfvalue" = xyes],
  [AC_SUBST(
   [ax_libfvalue_pc_libs_private],
   [-lfvalue])
  ])

 AS_IF(
  [test "x$ac_cv_libfvalue" = xyes],
  [AC_SUBST(
   [ax_libfvalue_spec_requires],
   [libfvalue])
  AC_SUBST(
   [ax_libfvalue_spec_build_requires],
   [libfvalue-devel])
  ])
 ])

