dnl Checks for libfwnt required headers and functions
dnl
dnl Version: 20191217

dnl Function to detect if libfwnt is available
dnl ac_libfwnt_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBFWNT_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libfwnt" = xno],
    [ac_cv_libfwnt=no],
    [ac_cv_libfwnt=check
    dnl Check if the directory provided as parameter exists
    AS_IF(
      [test "x$ac_cv_with_libfwnt" != x && test "x$ac_cv_with_libfwnt" != xauto-detect],
      [AS_IF(
        [test -d "$ac_cv_with_libfwnt"],
        [CFLAGS="$CFLAGS -I${ac_cv_with_libfwnt}/include"
        LDFLAGS="$LDFLAGS -L${ac_cv_with_libfwnt}/lib"],
        [AC_MSG_FAILURE(
          [no such directory: $ac_cv_with_libfwnt],
          [1])
        ])
      ],
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
        [dnl Check for the individual functions
        ac_cv_libfwnt=yes

        AC_CHECK_LIB(
          fwnt,
          libfwnt_get_version,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])

        dnl Security descriptor functions
        AC_CHECK_LIB(
          fwnt,
          libfwnt_security_descriptor_initialize,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])
        AC_CHECK_LIB(
          fwnt,
          libfwnt_security_descriptor_free,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])

        AC_CHECK_LIB(
          fwnt,
          libfwnt_security_descriptor_copy_from_byte_stream,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])
        AC_CHECK_LIB(
          fwnt,
          libfwnt_security_descriptor_get_owner,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])
        AC_CHECK_LIB(
          fwnt,
          libfwnt_security_descriptor_get_group,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])
        AC_CHECK_LIB(
          fwnt,
          libfwnt_security_descriptor_get_discretionary_acl,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])
        AC_CHECK_LIB(
          fwnt,
          libfwnt_security_descriptor_get_system_acl,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])

        dnl Security identifier (SID) functions
        AC_CHECK_LIB(
          fwnt,
          libfwnt_security_identifier_initialize,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])
        AC_CHECK_LIB(
          fwnt,
          libfwnt_security_identifier_free,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])

        AC_CHECK_LIB(
          fwnt,
          libfwnt_security_identifier_copy_from_byte_stream,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])
        AC_CHECK_LIB(
          fwnt,
          libfwnt_security_identifier_get_string_size,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])
        AC_CHECK_LIB(
          fwnt,
          libfwnt_security_identifier_copy_to_utf8_string,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])
        AC_CHECK_LIB(
          fwnt,
          libfwnt_security_identifier_copy_to_utf8_string_with_index,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])
        AC_CHECK_LIB(
          fwnt,
          libfwnt_security_identifier_copy_to_utf16_string,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])
        AC_CHECK_LIB(
          fwnt,
          libfwnt_security_identifier_copy_to_utf16_string_with_index,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])
        AC_CHECK_LIB(
          fwnt,
          libfwnt_security_identifier_copy_to_utf32_string,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])
        AC_CHECK_LIB(
          fwnt,
          libfwnt_security_identifier_copy_to_utf32_string_with_index,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])

        dnl Access control list (ACL) functions
        AC_CHECK_LIB(
          fwnt,
          libfwnt_access_control_list_free,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])

        AC_CHECK_LIB(
          fwnt,
          libfwnt_access_control_list_get_number_of_entries,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])
        AC_CHECK_LIB(
          fwnt,
          libfwnt_access_control_list_get_entry_by_index,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])

        dnl Access control entry (ACE) functions
        AC_CHECK_LIB(
          fwnt,
          libfwnt_access_control_entry_free,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])

        AC_CHECK_LIB(
          fwnt,
          libfwnt_access_control_entry_get_type,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])
        AC_CHECK_LIB(
          fwnt,
          libfwnt_access_control_entry_get_flags,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])
        AC_CHECK_LIB(
          fwnt,
          libfwnt_access_control_entry_get_access_mask,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])
        AC_CHECK_LIB(
          fwnt,
          libfwnt_access_control_entry_get_security_identifier,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])

        dnl LZNT1 functions
        AC_CHECK_LIB(
          fwnt,
          libfwnt_lznt1_decompress,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])

        dnl LZXPRESS functions
        AC_CHECK_LIB(
          fwnt,
          libfwnt_lzx_decompress,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])

        dnl LZXPRESS functions
        AC_CHECK_LIB(
          fwnt,
          libfwnt_lzxpress_decompress,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])
        AC_CHECK_LIB(
          fwnt,
          libfwnt_lzxpress_huffman_decompress,
          [ac_cv_libfwnt_dummy=yes],
          [ac_cv_libfwnt=no])

        ac_cv_libfwnt_LIBADD="-lfwnt"])
      ])
    AS_IF(
      [test "x$ac_cv_with_libfwnt" != x && test "x$ac_cv_with_libfwnt" != xauto-detect && test "x$ac_cv_libfwnt" != xyes],
      [AC_MSG_FAILURE(
        [unable to find supported libfwnt in directory: $ac_cv_with_libfwnt],
        [1])
      ])
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

  ac_cv_libfwnt_CPPFLAGS="-I../libfwnt";
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

