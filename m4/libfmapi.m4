dnl Checks for libfmapi required headers and functions
dnl
dnl Version: 20240531

dnl Function to detect if libfmapi is available
dnl ac_libfmapi_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBFMAPI_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libfmapi" = xno],
    [ac_cv_libfmapi=no],
    [ac_cv_libfmapi=check
    dnl Check if the directory provided as parameter exists
    dnl For both --with-libfmapi which returns "yes" and --with-libfmapi= which returns ""
    dnl treat them as auto-detection.
    AS_IF(
      [test "x$ac_cv_with_libfmapi" != x && test "x$ac_cv_with_libfmapi" != xauto-detect && test "x$ac_cv_with_libfmapi" != xyes],
      [AX_CHECK_LIB_DIRECTORY_EXISTS([libfmapi])],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libfmapi],
          [libfmapi >= 20230404],
          [ac_cv_libfmapi=yes],
          [ac_cv_libfmapi=check])
        ])
      AS_IF(
        [test "x$ac_cv_libfmapi" = xyes],
        [ac_cv_libfmapi_CPPFLAGS="$pkg_cv_libfmapi_CFLAGS"
        ac_cv_libfmapi_LIBADD="$pkg_cv_libfmapi_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libfmapi" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libfmapi.h])

      AS_IF(
        [test "x$ac_cv_header_libfmapi_h" = xno],
        [ac_cv_libfmapi=no],
        [ac_cv_libfmapi=yes

        AX_CHECK_LIB_FUNCTIONS(
         [libmfapi],
          [mfapi],
          [[libfmapi_get_version],
           [libfmapi_entry_identifier_initialize],
           [libfmapi_entry_identifier_free],
           [libfmapi_entry_identifier_copy_from_byte_stream],
           [libfmapi_entry_identifier_get_service_provider_identifier],
           [libfmapi_one_off_entry_identifier_initialize],
           [libfmapi_one_off_entry_identifier_free],
           [libfmapi_one_off_entry_identifier_copy_from_byte_stream],
           [libfmapi_one_off_entry_identifier_get_version],
           [libfmapi_one_off_entry_identifier_get_flags],
           [libfmapi_one_off_entry_identifier_get_utf8_display_name_size],
           [libfmapi_one_off_entry_identifier_get_utf8_display_name],
           [libfmapi_one_off_entry_identifier_get_utf16_display_name_size],
           [libfmapi_one_off_entry_identifier_get_utf16_display_name],
           [libfmapi_one_off_entry_identifier_get_utf8_address_type_size],
           [libfmapi_one_off_entry_identifier_get_utf8_address_type],
           [libfmapi_one_off_entry_identifier_get_utf16_address_type_size],
           [libfmapi_one_off_entry_identifier_get_utf16_address_type],
           [libfmapi_one_off_entry_identifier_get_utf8_email_address_size],
           [libfmapi_one_off_entry_identifier_get_utf8_email_address],
           [libfmapi_one_off_entry_identifier_get_utf16_email_address_size],
           [libfmapi_one_off_entry_identifier_get_utf16_email_address],
           [libfmapi_class_identifier_public_strings],
           [libfmapi_lzfu_get_uncompressed_data_size],
           [libfmapi_lzfu_decompress]])

        ac_cv_libfmapi_LIBADD="-lfmapi"])
      ])

    AX_CHECK_LIB_DIRECTORY_MSG_ON_FAILURE([libfmapi])
    ])

  dnl Check for debug functions
  AS_IF(
    [test "x$ac_cv_libfmapi" = xyes && test "x$ac_cv_enable_debug_output" != xno],
    [AC_CHECK_LIB(
      fmapi,
      libfmapi_property_identifier_get_identifier,
      [ac_cv_libfmapi_dummy=yes],
      [ac_cv_libfmapi=no])
    AC_CHECK_LIB(
      fmapi,
      libfmapi_property_identifier_get_description,
      [ac_cv_libfmapi_dummy=yes],
      [ac_cv_libfmapi=no])

    AC_CHECK_LIB(
      fmapi,
      libfmapi_value_type_get_identifier,
      [ac_cv_libfmapi_dummy=yes],
      [ac_cv_libfmapi=no])
    AC_CHECK_LIB(
      fmapi,
      libfmapi_value_type_get_description,
      [ac_cv_libfmapi_dummy=yes],
      [ac_cv_libfmapi=no])
    ])

  AS_IF(
    [test "x$ac_cv_libfmapi" = xyes],
    [AC_DEFINE(
      [HAVE_LIBFMAPI],
      [1],
      [Define to 1 if you have the `fmapi' library (-lfmapi).])
    ])

  AS_IF(
    [test "x$ac_cv_libfmapi" = xyes],
    [AC_SUBST(
      [HAVE_LIBFMAPI],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBFMAPI],
      [0])
    ])
  ])

dnl Function to detect if libfmapi dependencies are available
AC_DEFUN([AX_LIBFMAPI_CHECK_LOCAL],
  [dnl No additional checks.

  ac_cv_libfmapi_CPPFLAGS="-I../libfmapi -I\$(top_srcdir)/libfmapi";
  ac_cv_libfmapi_LIBADD="../libfmapi/libfmapi.la";

  ac_cv_libfmapi=local
  ])

dnl Function to detect how to enable libfmapi
AC_DEFUN([AX_LIBFMAPI_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libfmapi],
    [libfmapi],
    [search for libfmapi in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBFMAPI_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libfmapi" != xyes],
    [AX_LIBFMAPI_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBFMAPI],
      [1],
      [Define to 1 if the local version of libfmapi is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBFMAPI],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBFMAPI],
    [test "x$ac_cv_libfmapi" = xlocal])
  AS_IF(
    [test "x$ac_cv_libfmapi_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBFMAPI_CPPFLAGS],
      [$ac_cv_libfmapi_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libfmapi_LIBADD" != "x"],
    [AC_SUBST(
      [LIBFMAPI_LIBADD],
      [$ac_cv_libfmapi_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libfmapi" = xyes],
    [AC_SUBST(
      [ax_libfmapi_pc_libs_private],
      [-lfmapi])
    ])

  AS_IF(
    [test "x$ac_cv_libfmapi" = xyes],
    [AC_SUBST(
      [ax_libfmapi_spec_requires],
      [libfmapi])
    AC_SUBST(
      [ax_libfmapi_spec_build_requires],
      [libfmapi-devel])
    ])
  ])

