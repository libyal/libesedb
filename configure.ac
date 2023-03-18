AC_PREREQ([2.71])

AC_INIT(
 [libesedb],
 [20230318],
 [joachim.metz@gmail.com])

AC_CONFIG_SRCDIR(
 [include/libesedb.h.in])

AM_INIT_AUTOMAKE([gnu 1.6 tar-ustar])

AC_CONFIG_MACRO_DIR([m4])

dnl Check for host type
AC_CANONICAL_HOST

dnl Check for libtool DLL support
LT_INIT([win32-dll])

dnl Checks for programs
AC_PROG_CC
AC_PROG_GCC_TRADITIONAL
AC_PROG_CXX
AC_PROG_MAKE_SET
AC_PROG_INSTALL

dnl Check for libtool
AC_SUBST(LIBTOOL_DEPS)

dnl Check for pkg-config
AC_PATH_PROG(PKGCONFIG,[pkg-config])

dnl Support of internationalization (i18n)
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.21])

dnl Check for compiler language support
AC_C_CONST
AC_C_VOLATILE

dnl Check for large file support
AC_SYS_LARGEFILE

dnl Check if shared library support should be disabled
AX_COMMON_CHECK_DISABLE_SHARED_LIBS

dnl Check if tools should be build as static executables
AX_COMMON_CHECK_ENABLE_STATIC_EXECUTABLES

dnl Check if WINAPI support should be enabled
AX_COMMON_CHECK_ENABLE_WINAPI

dnl Check if wide character type should be enabled
AX_COMMON_CHECK_ENABLE_WIDE_CHARACTER_TYPE

dnl Check if verbose output should be enabled
AX_COMMON_CHECK_ENABLE_VERBOSE_OUTPUT

dnl Check if debug output should be enabled
AX_COMMON_CHECK_ENABLE_DEBUG_OUTPUT

dnl Check for type definitions
AX_TYPES_CHECK_LOCAL

dnl Check if common required headers and functions are available
AX_COMMON_CHECK_LOCAL

dnl Check if libcerror or required headers and functions are available
AX_LIBCERROR_CHECK_ENABLE

dnl Check if libcthreads or required headers and functions are available
AX_LIBCTHREADS_CHECK_ENABLE

dnl Check if libcdata or required headers and functions are available
AX_LIBCDATA_CHECK_ENABLE

dnl Check if libclocale or required headers and functions are available
AX_LIBCLOCALE_CHECK_ENABLE

dnl Check if libcnotify or required headers and functions are available
AX_LIBCNOTIFY_CHECK_ENABLE

dnl Check if libcsplit or required headers and functions are available
AX_LIBCSPLIT_CHECK_ENABLE

dnl Check if libuna or required headers and functions are available
AX_LIBUNA_CHECK_ENABLE

dnl Check if libcfile or required headers and functions are available
AX_LIBCFILE_CHECK_ENABLE

dnl Check if libcpath or required headers and functions are available
AX_LIBCPATH_CHECK_ENABLE

dnl Check if libbfio or required headers and functions are available
AX_LIBBFIO_CHECK_ENABLE

dnl Check if libfcache or required headers and functions are available
AX_LIBFCACHE_CHECK_ENABLE

dnl Check if libfdata or required headers and functions are available
AX_LIBFDATA_CHECK_ENABLE

dnl Check if libfdatetime or required headers and functions are available
AX_LIBFDATETIME_CHECK_ENABLE

dnl Check if libfguid or required headers and functions are available
AX_LIBFGUID_CHECK_ENABLE

dnl Check if libfvalue or required headers and functions are available
AX_LIBFVALUE_CHECK_ENABLE

dnl Check if libfwnt or required headers and functions are available
AX_LIBFWNT_CHECK_ENABLE

dnl Check if libesedb required headers and functions are available
AX_LIBESEDB_CHECK_LOCAL

dnl Check if libesedb Python bindings (pyesedb) required headers and functions are available
AX_PYTHON_CHECK_ENABLE

AS_IF(
  [test "x${ac_cv_enable_python}" != xno || test "x${ac_cv_enable_python2}" != xno || test "x${ac_cv_enable_python3}" != xno],
  [dnl Headers included in pyesedb/pyesedb_error.c
  AC_CHECK_HEADERS([stdarg.h varargs.h])

  AS_IF(
    [test "x$ac_cv_header_stdarg_h" != xyes && test "x$ac_cv_header_varargs_h" != xyes],
    [AC_MSG_FAILURE(
      [Missing headers: stdarg.h and varargs.h],
      [1])
    ])
  ])

dnl Check if libfmapi or required headers and functions are available
AX_LIBFMAPI_CHECK_ENABLE

dnl Check if libmapidb or required headers and functions are available
AX_LIBMAPIDB_CHECK_ENABLE

dnl Check if esedbtools required headers and functions are available
AX_ESEDBTOOLS_CHECK_LOCAL

dnl Check if DLL support is needed
AX_LIBESEDB_CHECK_DLL_SUPPORT

dnl Check if tests required headers and functions are available
AX_TESTS_CHECK_LOCAL
AX_TESTS_CHECK_OSSFUZZ

dnl Set additional compiler flags
CFLAGS="$CFLAGS -Wall";

dnl Check if requires and build requires should be set in spec file
AS_IF(
 [test "x$ac_cv_libcerror" = xyes || test "x$ac_cv_libcthreads" = xyes || test "x$ac_cv_libcdata" = xyes || test "x$ac_cv_libclocale" = xyes || test "x$ac_cv_libcnotify" = xyes || test "x$ac_cv_libcsplit" = xyes || test "x$ac_cv_libuna" = xyes || test "x$ac_cv_libcfile" = xyes || test "x$ac_cv_libcpath" = xyes || test "x$ac_cv_libbfio" = xyes || test "x$ac_cv_libfcache" = xyes || test "x$ac_cv_libfdata" = xyes || test "x$ac_cv_libfdatetime" = xyes || test "x$ac_cv_libfguid" = xyes || test "x$ac_cv_libfvalue" = xyes || test "x$ac_cv_libfwnt" = xyes],
 [AC_SUBST(
  [libesedb_spec_requires],
  [Requires:])
 ])

AS_IF(
 [test "x$ac_cv_libfmapi" = xyes || test "x$ac_cv_libmapidb" = xyes],
 [AC_SUBST(
  [libesedb_spec_tools_build_requires],
  [BuildRequires:])
 ])

dnl Set the date for the dpkg files
AC_SUBST(
 [DPKG_DATE],
 [`date -R 2> /dev/null`])

dnl Set the date for the spec file
AC_SUBST(
 [SPEC_DATE],
 [`date +"%a %b %e %Y" 2> /dev/null`])

dnl Generate Makefiles
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([include/Makefile])
AC_CONFIG_FILES([common/Makefile])
AC_CONFIG_FILES([libcerror/Makefile])
AC_CONFIG_FILES([libcthreads/Makefile])
AC_CONFIG_FILES([libcdata/Makefile])
AC_CONFIG_FILES([libclocale/Makefile])
AC_CONFIG_FILES([libcnotify/Makefile])
AC_CONFIG_FILES([libcsplit/Makefile])
AC_CONFIG_FILES([libuna/Makefile])
AC_CONFIG_FILES([libcfile/Makefile])
AC_CONFIG_FILES([libcpath/Makefile])
AC_CONFIG_FILES([libbfio/Makefile])
AC_CONFIG_FILES([libfcache/Makefile])
AC_CONFIG_FILES([libfdata/Makefile])
AC_CONFIG_FILES([libfdatetime/Makefile])
AC_CONFIG_FILES([libfguid/Makefile])
AC_CONFIG_FILES([libfvalue/Makefile])
AC_CONFIG_FILES([libfwnt/Makefile])
AC_CONFIG_FILES([libesedb/Makefile])
AC_CONFIG_FILES([pyesedb/Makefile])
AC_CONFIG_FILES([pyesedb-python2/Makefile])
AC_CONFIG_FILES([pyesedb-python3/Makefile])
AC_CONFIG_FILES([libfmapi/Makefile])
AC_CONFIG_FILES([libmapidb/Makefile])
AC_CONFIG_FILES([esedbtools/Makefile])
AC_CONFIG_FILES([po/Makefile.in])
AC_CONFIG_FILES([po/Makevars])
AC_CONFIG_FILES([manuals/Makefile])
AC_CONFIG_FILES([tests/Makefile])
AC_CONFIG_FILES([ossfuzz/Makefile])
AC_CONFIG_FILES([msvscpp/Makefile])
dnl Generate header files
AC_CONFIG_FILES([include/libesedb.h])
AC_CONFIG_FILES([include/libesedb/definitions.h])
AC_CONFIG_FILES([include/libesedb/features.h])
AC_CONFIG_FILES([include/libesedb/types.h])
AC_CONFIG_FILES([libesedb/libesedb_definitions.h])
dnl Generate distribution specific files
AC_CONFIG_FILES([common/types.h])
AC_CONFIG_FILES([dpkg/changelog])
AC_CONFIG_FILES([libesedb/libesedb.rc])
AC_CONFIG_FILES([libesedb.pc])
AC_CONFIG_FILES([libesedb.spec])
dnl Generate a source configuration file
AC_CONFIG_HEADERS([common/config.h])

AC_OUTPUT

dnl Print a summary
AC_MSG_NOTICE([
Building:
   libcerror support:                          $ac_cv_libcerror
   libcthreads support:                        $ac_cv_libcthreads
   libcdata support:                           $ac_cv_libcdata
   libclocale support:                         $ac_cv_libclocale
   libcnotify support:                         $ac_cv_libcnotify
   libcsplit support:                          $ac_cv_libcsplit
   libuna support:                             $ac_cv_libuna
   libcfile support:                           $ac_cv_libcfile
   libcpath support:                           $ac_cv_libcpath
   libbfio support:                            $ac_cv_libbfio
   libfcache support:                          $ac_cv_libfcache
   libfdata support:                           $ac_cv_libfdata
   libfdatetime support:                       $ac_cv_libfdatetime
   libfguid support:                           $ac_cv_libfguid
   libfvalue support:                          $ac_cv_libfvalue
   libfwnt support:                            $ac_cv_libfwnt
   libfmapi support:                           $ac_cv_libfmapi
   libmapidb support:                          $ac_cv_libmapidb

Features:
   Multi-threading support:                    $ac_cv_libcthreads_multi_threading
   Wide character type support:                $ac_cv_enable_wide_character_type
   esedbtools are build as static executables: $ac_cv_enable_static_executables
   Python (pyesedb) support:                   $ac_cv_enable_python
   Verbose output:                             $ac_cv_enable_verbose_output
   Debug output:                               $ac_cv_enable_debug_output
]);

