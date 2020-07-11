dnl Checks for libfvalue required headers and functions
dnl
dnl Version: 20200711

dnl Function to detect if libfvalue is available
dnl ac_libfvalue_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBFVALUE_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libfvalue" = xno],
    [ac_cv_libfvalue=no],
    [ac_cv_libfvalue=check
    dnl Check if the directory provided as parameter exists
    AS_IF(
      [test "x$ac_cv_with_libfvalue" != x && test "x$ac_cv_with_libfvalue" != xauto-detect],
      [AS_IF(
        [test -d "$ac_cv_with_libfvalue"],
        [CFLAGS="$CFLAGS -I${ac_cv_with_libfvalue}/include"
        LDFLAGS="$LDFLAGS -L${ac_cv_with_libfvalue}/lib"],
        [AC_MSG_FAILURE(
          [no such directory: $ac_cv_with_libfvalue],
          [1])
        ])
      ],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libfvalue],
          [libfvalue >= 20200711],
          [ac_cv_libfvalue=yes],
          [ac_cv_libfvalue=check])
        ])
      AS_IF(
        [test "x$ac_cv_libfvalue" = xyes],
        [ac_cv_libfvalue_CPPFLAGS="$pkg_cv_libfvalue_CFLAGS"
        ac_cv_libfvalue_LIBADD="$pkg_cv_libfvalue_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libfvalue" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libfvalue.h])

      AS_IF(
        [test "x$ac_cv_header_libfvalue_h" = xno],
        [ac_cv_libfvalue=no],
        [dnl Check for the individual functions
        ac_cv_libfvalue=yes

        AC_CHECK_LIB(
          fvalue,
          libfvalue_get_version,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        dnl Data handle functions
        AC_CHECK_LIB(
          fvalue,
          libfvalue_data_handle_initialize,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_data_handle_free,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_data_handle_clone,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_data_handle_clear,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        AC_CHECK_LIB(
          fvalue,
          libfvalue_data_handle_get_data,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_data_handle_set_data,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        AC_CHECK_LIB(
          fvalue,
          libfvalue_data_handle_get_data_flags,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_data_handle_set_data_flags,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        AC_CHECK_LIB(
          fvalue,
          libfvalue_data_handle_get_number_of_value_entries,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_data_handle_get_value_entry,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_data_handle_set_value_entry,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_data_handle_append_value_entry,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        dnl Integer functions
        AC_CHECK_LIB(
          fvalue,
          libfvalue_string_size_from_integer,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        AC_CHECK_LIB(
          fvalue,
          libfvalue_utf8_string_copy_from_integer,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_utf8_string_with_index_copy_from_integer,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_utf8_string_copy_to_integer,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_utf8_string_with_index_copy_to_integer,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        AC_CHECK_LIB(
          fvalue,
          libfvalue_utf16_string_copy_from_integer,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_utf16_string_with_index_copy_from_integer,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_utf16_string_copy_to_integer,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_utf16_string_with_index_copy_to_integer,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        AC_CHECK_LIB(
          fvalue,
          libfvalue_utf32_string_copy_from_integer,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_utf32_string_with_index_copy_from_integer,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_utf32_string_copy_to_integer,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_utf32_string_with_index_copy_to_integer,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        dnl String functions
        AC_CHECK_LIB(
          fvalue,
          libfvalue_utf8_string_split,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        AC_CHECK_LIB(
          fvalue,
          libfvalue_utf16_string_split,
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
          libfvalue_split_utf8_string_get_string,
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

        AC_CHECK_LIB(
          fvalue,
          libfvalue_split_utf16_string_free,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_split_utf16_string_get_string,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_split_utf16_string_get_number_of_segments,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_split_utf16_string_get_segment_by_index,
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
          libfvalue_table_resize,
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
          libfvalue_table_get_index_by_identifier,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        AC_CHECK_LIB(
          fvalue,
          libfvalue_table_get_value_by_index,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_table_get_value_by_identifier,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_table_set_value_by_index,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_table_set_value,
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
          libfvalue_value_clear,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_get_type,
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
          libfvalue_value_get_data_handle,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_has_data,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_get_data_size,
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
          libfvalue_value_copy_data,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_get_format_flags,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_set_format_flags,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_get_number_of_value_entries,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_get_value_entry,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_set_value_entry,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_append_value_entry,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_copy_from_boolean,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_copy_to_boolean,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_copy_from_8bit,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_copy_to_8bit,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_copy_from_16bit,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_copy_to_16bit,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_copy_from_32bit,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_copy_to_32bit,
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
          libfvalue_value_copy_from_float,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_copy_to_float,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_copy_from_double,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_copy_to_double,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_copy_from_utf8_string,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_get_utf8_string_size,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_copy_to_utf8_string,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_copy_to_utf8_string_with_index,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_copy_from_utf16_string,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_get_utf16_string_size,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_copy_to_utf16_string,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_copy_to_utf16_string_with_index,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_copy_from_utf32_string,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_get_utf32_string_size,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_copy_to_utf32_string,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_copy_to_utf32_string_with_index,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_read_from_file_stream,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_write_to_file_stream,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        dnl Value type functions
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_type_initialize,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_type_initialize_with_data_handle,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_type_set_data_string,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])
        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_type_append_data_string,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        AC_CHECK_LIB(
          fvalue,
          libfvalue_value_type_set_data_strings_array,
          [ac_cv_libfvalue_dummy=yes],
          [ac_cv_libfvalue=no])

        ac_cv_libfvalue_LIBADD="-lfvalue"])
      ])
    AS_IF(
      [test "x$ac_cv_with_libfvalue" != x && test "x$ac_cv_with_libfvalue" != xauto-detect && test "x$ac_cv_libfvalue" != xyes],
      [AC_MSG_FAILURE(
        [unable to find supported libfvalue in directory: $ac_cv_with_libfvalue],
        [1])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_libfvalue" = xyes],
    [AC_DEFINE(
      [HAVE_LIBFVALUE],
      [1],
      [Define to 1 if you have the `fvalue' library (-lfvalue).])
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

dnl Function to detect if libfvalue dependencies are available
AC_DEFUN([AX_LIBFVALUE_CHECK_LOCAL],
  [dnl No additional checks.

  ac_cv_libfvalue_CPPFLAGS="-I../libfvalue";
  ac_cv_libfvalue_LIBADD="../libfvalue/libfvalue.la";

  ac_cv_libfvalue=local
  ])

dnl Function to detect how to enable libfvalue
AC_DEFUN([AX_LIBFVALUE_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libfvalue],
    [libfvalue],
    [search for libfvalue in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBFVALUE_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libfvalue" != xyes],
    [AX_LIBFVALUE_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBFVALUE],
      [1],
      [Define to 1 if the local version of libfvalue is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBFVALUE],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBFVALUE],
    [test "x$ac_cv_libfvalue" = xlocal])
  AS_IF(
    [test "x$ac_cv_libfvalue_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBFVALUE_CPPFLAGS],
      [$ac_cv_libfvalue_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libfvalue_LIBADD" != "x"],
    [AC_SUBST(
      [LIBFVALUE_LIBADD],
      [$ac_cv_libfvalue_LIBADD])
    ])

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

