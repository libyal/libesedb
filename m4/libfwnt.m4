dnl Checks for libfwnt required headers and functions
dnl
dnl Version: 20240519

dnl Function to detect if libfwnt is available
dnl ac_libfwnt_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBFWNT_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libfwnt" = xno],
    [ac_cv_libfwnt=no],
    [ac_cv_libfwnt=check
    dnl Check if the directory provided as parameter exists
    dnl For both --with-libfwnt which returns "yes" and --with-libfwnt= which returns ""
    dnl treat them as auto-detection.
    AS_IF(
      [test "x$ac_cv_with_libfwnt" != x && test "x$ac_cv_with_libfwnt" != xauto-detect && test "x$ac_cv_with_libfwnt" != xyes],
      [AX_CHECK_LIB_DIRECTORY_EXISTS([libfwnt])],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libfwnt],
          [libfwnt >= 20191217],
          [ac_cv_libfwnt=yes],
          [ac_cv_libfwnt=check])
        ])
      AS_IF(
        [test "x$ac_cv_libfwnt" = xyes],
        [ac_cv_libfwnt_CPPFLAGS="$pkg_cv_libfwnt_CFLAGS"
        ac_cv_libfwnt_LIBADD="$pkg_cv_libfwnt_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libfwnt" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libfwnt.h])

      AS_IF(
        [test "x$ac_cv_header_libfwnt_h" = xno],
        [ac_cv_libfwnt=no],
        [ac_cv_libfwnt=yes

        AX_CHECK_LIB_FUNCTIONS(
          [libfwnt],
          [fwnt],
          [[libfwnt_get_version],
           [libfwnt_array_initialize],
           [libfwnt_security_descriptor_initialize],
           [libfwnt_security_descriptor_free],
           [libfwnt_security_descriptor_copy_from_byte_stream],
           [libfwnt_security_descriptor_get_owner],
           [libfwnt_security_descriptor_get_group],
           [libfwnt_security_descriptor_get_discretionary_acl],
           [libfwnt_security_descriptor_get_system_acl],
           [libfwnt_security_identifier_initialize],
           [libfwnt_security_identifier_free],
           [libfwnt_security_identifier_copy_from_byte_stream],
           [libfwnt_security_identifier_get_string_size],
           [libfwnt_security_identifier_copy_to_utf8_string],
           [libfwnt_security_identifier_copy_to_utf8_string_with_index],
           [libfwnt_security_identifier_copy_to_utf16_string],
           [libfwnt_security_identifier_copy_to_utf16_string_with_index],
           [libfwnt_security_identifier_copy_to_utf32_string],
           [libfwnt_security_identifier_copy_to_utf32_string_with_index],
           [libfwnt_access_control_list_free],
           [libfwnt_access_control_list_get_number_of_entries],
           [libfwnt_access_control_list_get_entry_by_index],
           [libfwnt_access_control_entry_free],
           [libfwnt_access_control_entry_get_type],
           [libfwnt_access_control_entry_get_flags],
           [libfwnt_access_control_entry_get_access_mask],
           [libfwnt_access_control_entry_get_security_identifier],
           [libfwnt_lznt1_decompress],
           [libfwnt_lzx_decompress],
           [libfwnt_lzxpress_decompress],
           [libfwnt_lzxpress_huffman_decompress]])

        ac_cv_libfwnt_LIBADD="-lfwnt"])
      ])

    AX_CHECK_LIB_DIRECTORY_MSG_ON_FAILURE([libfwnt])
    ])

  AS_IF(
    [test "x$ac_cv_libfwnt" = xyes],
    [AC_DEFINE(
      [HAVE_LIBFWNT],
      [1],
      [Define to 1 if you have the `fwnt' library (-lfwnt).])
    ])

  AS_IF(
    [test "x$ac_cv_libfwnt" = xyes],
    [AC_SUBST(
      [HAVE_LIBFWNT],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBFWNT],
      [0])
    ])
  ])

dnl Function to detect if libfwnt dependencies are available
AC_DEFUN([AX_LIBFWNT_CHECK_LOCAL],
  [dnl No additional checks.

  ac_cv_libfwnt_CPPFLAGS="-I../libfwnt -I\$(top_srcdir)/libfwnt";
  ac_cv_libfwnt_LIBADD="../libfwnt/libfwnt.la";

  ac_cv_libfwnt=local
  ])

dnl Function to detect how to enable libfwnt
AC_DEFUN([AX_LIBFWNT_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libfwnt],
    [libfwnt],
    [search for libfwnt in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBFWNT_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libfwnt" != xyes],
    [AX_LIBFWNT_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBFWNT],
      [1],
      [Define to 1 if the local version of libfwnt is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBFWNT],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBFWNT],
    [test "x$ac_cv_libfwnt" = xlocal])
  AS_IF(
    [test "x$ac_cv_libfwnt_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBFWNT_CPPFLAGS],
      [$ac_cv_libfwnt_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libfwnt_LIBADD" != "x"],
    [AC_SUBST(
      [LIBFWNT_LIBADD],
      [$ac_cv_libfwnt_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libfwnt" = xyes],
    [AC_SUBST(
      [ax_libfwnt_pc_libs_private],
      [-lfwnt])
    ])

  AS_IF(
    [test "x$ac_cv_libfwnt" = xyes],
    [AC_SUBST(
      [ax_libfwnt_spec_requires],
      [libfwnt])
    AC_SUBST(
      [ax_libfwnt_spec_build_requires],
      [libfwnt-devel])
    ])
  ])

