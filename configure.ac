AC_PREREQ( 2.59 )

AC_INIT(
 [libesedb],
 [20111030],
 [jbmetz@users.sourceforge.net])

AC_CONFIG_SRCDIR(
 [include/libesedb.h.in])

AM_INIT_AUTOMAKE([gnu 1.6])

AC_CONFIG_MACRO_DIR([m4])

dnl Check for host type
AC_CANONICAL_HOST

dnl Check for large file support
AC_SYS_LARGEFILE

dnl Check for libtool DLL support
AC_LIBTOOL_WIN32_DLL

dnl Checks for programs
AC_PROG_CC
AC_PROG_GCC_TRADITIONAL
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

dnl Support of internationalization (i18n)
AM_GNU_GETTEXT([external])

dnl Check for compiler language support
AC_C_CONST
AC_C_VOLATILE

dnl Check if WINAPI support should be enabled
AX_COMMON_CHECK_ENABLE_WINAPI

dnl Check if wide character type should be enabled
AX_COMMON_CHECK_ENABLE_WIDE_CHARACTER_TYPE

dnl Check for type definitions
AX_TYPES_CHECK_LOCAL

dnl Checks for libraries and library functions
AX_COMMON_CHECK_LOCAL
AX_LIBCSTRING_CHECK_LOCAL
AX_LIBERROR_CHECK_LOCAL
AX_LIBNOTIFY_CHECK_LOCAL
AX_LIBSYSTEM_CHECK_LOCAL

dnl Check for enabling libuna for Unicode string support
AX_LIBUNA_CHECK_ENABLE

dnl Check for enabling libbfio for basic file IO support
AX_LIBBFIO_CHECK_ENABLE

dnl Check for enabling libfdata for file and memory data block support
AX_LIBFDATA_CHECK_ENABLE

dnl Check for enabling libfdatetime for file date and time support
AX_LIBFDATETIME_CHECK_ENABLE

dnl Check for enabling libfguid for file GUID/UUID support
AX_LIBFGUID_CHECK_ENABLE

dnl Check for enabling libfvalue for file value support
AX_LIBFVALUE_CHECK_ENABLE

dnl Check for enabling libfwnt for file Windows NT type support
AX_LIBFWNT_CHECK_ENABLE

dnl Check for enabling libfmapi for file MAPI types support
AX_LIBFMAPI_CHECK_ENABLE

dnl Check for enabling libmapidb for MAPI database support
AX_LIBMAPIDB_CHECK_ENABLE

dnl Check for library function support
AX_LIBESEDB_CHECK_LOCAL

dnl Check if esedbtools should be build as static executables
AX_COMMON_CHECK_ENABLE_STATIC_EXECUTABLES

dnl Check if libesedb should be build with verbose output
AX_COMMON_CHECK_ENABLE_VERBOSE_OUTPUT

dnl Check if libesedb should be build with debug output
AX_COMMON_CHECK_ENABLE_DEBUG_OUTPUT

dnl Check if requires and build requires should be set in spec file
AS_IF(
 [test "x$ac_cv_libuna" = xyes || test "x$ac_cv_libbfio" = xyes || test "x$ac_cv_libfdata" = xyes || test "x$ac_cv_libfdatetime" = xyes || test "x$ac_cv_libfguid" = xyes],
 [AC_SUBST(
  [libesedb_spec_requires],
  [Requires:])
 AC_SUBST(
  [libesedb_spec_build_requires],
  [BuildRequires:])
 ])

AS_IF(
 [test "x$ac_cv_libfwnt" = xyes || test "x$ac_cv_libfmapi" = xyes || test "x$ac_cv_libmapidb" = xyes],
 [AC_SUBST(
  [libesedb_spec_tools_build_requires],
  [BuildRequires:])
 ])

dnl Set memwatch support
ac_cv_enable_memwatch=no

AS_IF(
 [test "x$ac_cv_enable_memwatch" != xno ],
 [AC_SUBST(
  [MEMWATCH_CPPFLAGS],
  ["-DMEMWATCH -DMEMWATCH_STDIO"])
 AC_SUBST(
  [MEMWATCH_LIBADD],
  [../common/libmemwatch.la]) ])

AM_CONDITIONAL(
 [HAVE_MEMWATCH],
 [test "x$ac_cv_enable_memwatch" != xno])

dnl Set additional compiler flags
CFLAGS="$CFLAGS -Wall";

dnl Check if DLL support is needed in esedbtools
AS_IF(
 [test "x$enable_shared" = xyes && test "x$ac_cv_enable_static_executables" = xno],
 [AS_CASE(
  [$host],
  [*cygwin* | *mingw*],
  [AC_SUBST(
   [LIBESEDB_DLL_IMPORT],
   ["-DLIBESEDB_DLL_IMPORT"])
  ])
 ])

dnl Have configure make the Makefiles
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([include/Makefile])
AC_CONFIG_FILES([common/Makefile])
AC_CONFIG_FILES([libcstring/Makefile])
AC_CONFIG_FILES([liberror/Makefile])
AC_CONFIG_FILES([libnotify/Makefile])
AC_CONFIG_FILES([libuna/Makefile])
AC_CONFIG_FILES([libbfio/Makefile])
AC_CONFIG_FILES([libfdata/Makefile])
AC_CONFIG_FILES([libfdatetime/Makefile])
AC_CONFIG_FILES([libfguid/Makefile])
AC_CONFIG_FILES([libfvalue/Makefile])
AC_CONFIG_FILES([libesedb/Makefile])
AC_CONFIG_FILES([libfwnt/Makefile])
AC_CONFIG_FILES([libfmapi/Makefile])
AC_CONFIG_FILES([libmapidb/Makefile])
AC_CONFIG_FILES([libsystem/Makefile])
AC_CONFIG_FILES([esedbtools/Makefile])
AC_CONFIG_FILES([po/Makefile.in])
AC_CONFIG_FILES([po/Makevars])
AC_CONFIG_FILES([manuals/Makefile])
AC_CONFIG_FILES([macosx/Makefile])
AC_CONFIG_FILES([msvscpp/Makefile])
dnl Have configure make the include files
AC_CONFIG_FILES([include/libesedb.h])
AC_CONFIG_FILES([include/libesedb/definitions.h])
AC_CONFIG_FILES([include/libesedb/features.h])
AC_CONFIG_FILES([include/libesedb/types.h])
AC_CONFIG_FILES([libesedb/libesedb_definitions.h])
dnl Have configure make distribution specific files
AC_CONFIG_FILES([common/types.h])
AC_CONFIG_FILES([libesedb/libesedb.rc])
AC_CONFIG_FILES([macosx/Introduction.rtf])
AC_CONFIG_FILES([libesedb.pc])
AC_CONFIG_FILES([libesedb.spec])
dnl Have configure write its configuration
AC_CONFIG_HEADERS([common/config.h])

AC_OUTPUT

dnl Print a summary
AC_MSG_NOTICE([
Building:
   libuna support:                             $ac_cv_libuna
   libbfio support:                            $ac_cv_libbfio
   libfdata support:                           $ac_cv_libfdata
   libfdatetime support:                       $ac_cv_libfdatetime
   libfguid support:                           $ac_cv_libfguid
   libfvalue support:                          $ac_cv_libfvalue
   libfwnt support:                            $ac_cv_libfwnt
   libfmapi support:                           $ac_cv_libfmapi
   libmapidb support:                          $ac_cv_libmapidb

Features:
   Wide character type support:                $ac_cv_enable_wide_character_type
   esedbtools are build as static executables: $ac_cv_enable_static_executables
   Verbose output:                             $ac_cv_enable_verbose_output
   Debug output:                               $ac_cv_enable_debug_output
]);


