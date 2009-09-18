AC_PREREQ( 2.59 )

AC_INIT(
 [libesedb],
  [20090918],
  [forensics@hoffmannbv.nl])

AC_CONFIG_SRCDIR(
 [include/libesedb.h.in])

AM_INIT_AUTOMAKE(
 [libesedb],
 [20090918])

AC_CONFIG_MACRO_DIR([m4])

dnl Check for host type
AC_CANONICAL_HOST

dnl Check for large file support
AC_SYS_LARGEFILE

dnl Check for configure arguments
LIBESEDB_TEST_ENABLE(
 [wide-character-type],
 [wide_character_type],
 [enable wide character type support],
 [no])
LIBESEDB_TEST_ENABLE(
 [static-executables],
 [static_executables],
 [build the esedbtools as static executables],
 [no])
LIBESEDB_TEST_ENABLE(
 [verbose-output],
 [verbose_output],
 [enable verbose output],
 [no])
LIBESEDB_TEST_ENABLE(
 [debug-output],
 [debug_output],
 [enable debug output],
 [no])

dnl Check for libtool DLL support
AC_LIBTOOL_WIN32_DLL

dnl Checks for programs
AC_PROG_CC
AC_PROG_GCC_TRADITIONAL
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

dnl Check for compiler language support
AC_C_CONST
AC_C_VOLATILE

dnl Check for headers
dnl Headers included in libesedb/types.h
AC_CHECK_HEADER(
 [sys/types.h],
 [AC_DEFINE(
  [HAVE_SYS_TYPES_H],
  [1],
  [Define to 1 if you have the <sys/types.h> header file.])
 AC_SUBST(
  [HAVE_SYS_TYPES_H],
  [1])],
 [AC_SUBST(
  [HAVE_SYS_TYPES_H],
  [0])])

AC_CHECK_HEADER(
 [inttypes.h],
 [AC_DEFINE(
  [HAVE_INTTYPES_H],
  [1],
  [Define to 1 if you have the <inttypes.h> header file.])
 AC_SUBST(
  [HAVE_INTTYPES_H],
  [1])],
 [AC_SUBST(
  [HAVE_INTTYPES_H],
  [0])])

AC_CHECK_HEADER(
 [stdint.h],
 [AC_DEFINE(
  [HAVE_STDINT_H],
  [1],
  [Define to 1 if you have the <stdint.h> header file.])
 AC_SUBST(
  [HAVE_STDINT_H],
  [1])],
 [AC_SUBST(
  [HAVE_STDINT_H],
  [0])])

dnl Headers included in common/types.h
AC_CHECK_HEADERS([limits.h])

dnl Headers included in common/narrow_string.h and common/memory.h
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([stdlib.h])

dnl Headers included in libnotify/libnotify.c
AC_CHECK_HEADERS([stdarg.h])
AC_CHECK_HEADERS([varargs.h])

dnl Headers included in libnotify/libnotify.c, libbfio/libbfio_file.c and libewf/libewf_header_values.c
AC_CHECK_HEADERS([errno.h])

dnl Headers included in libbfio/libbfio_file.c, libsystem/libsystem_directory_io.h and libsystem/libsystem_file_io.h
AC_CHECK_HEADERS([sys/stat.h])

dnl Headers included in libbfio/libbfio_file.c and libsystem/libsystem_file_io.h
AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([unistd.h])

dnl Headers included in libsystem/libsystem_glob.h
AC_CHECK_HEADERS([glob.h])

dnl Headers included in libsytem/libsystem_support.c
AC_CHECK_HEADERS([langinfo.h])
AC_CHECK_HEADERS([locale.h])

dnl Headers included in libsystem/libsystem_signal.h
AC_CHECK_HEADERS([signal.h])
AC_CHECK_HEADERS([sys/signal.h])

dnl Other headers

dnl Check for declarations and type definitions
AC_TYPE_OFF_T
AC_TYPE_SIZE_T

AC_CHECK_TYPE(
 [size32_t],
 [AC_SUBST(
  [HAVE_SIZE32_T],
  [1])],
 [AC_SUBST(
  [HAVE_SIZE32_T],
  [0])])

AC_CHECK_TYPE(
 [ssize32_t],
 [AC_SUBST(
  [HAVE_SSIZE32_T],
  [1])],
 [AC_SUBST(
  [HAVE_SSIZE32_T],
  [0])])

AC_CHECK_TYPE(
 [size64_t],
 [AC_SUBST(
  [HAVE_SIZE64_T],
  [1])],
 [AC_SUBST(
  [HAVE_SIZE64_T],
  [0])])

AC_CHECK_TYPE(
 [ssize64_t],
 [AC_SUBST(
  [HAVE_SSIZE64_T],
  [1])],
 [AC_SUBST(
  [HAVE_SSIZE64_T],
  [0])])

AC_CHECK_TYPE(
 [off64_t],
 [AC_SUBST(
  [HAVE_OFF64_T],
  [1])],
 [AC_SUBST(
  [HAVE_OFF64_T],
  [0])])

AC_CHECK_TYPE([ssize_t])

AC_CHECK_SIZEOF([off_t])
AC_CHECK_SIZEOF([size_t])

dnl Check for functions
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
 [AC_MSG_FAILURE(
  [Missing function: fseeko],
  [1]) ])

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

AC_CHECK_FUNCS(
 [realloc],
 [],
 [AC_MSG_FAILURE(
  [Missing function: realloc],
  [1]) ])

dnl Narrow character string functions used in common/narrow_string.h
AC_CHECK_FUNCS(
 [snprintf],
 [],
 [AC_CHECK_FUNCS(
  [sprintf],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: snprintf and sprintf],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [sscanf],
 [],
 [AC_MSG_FAILURE(
  [Missing function: sscanf],
  [1]) ])

AC_CHECK_FUNCS(
 [strlen],
 [],
 [AC_MSG_FAILURE(
  [Missing function: strlen],
  [1]) ])

AC_CHECK_FUNCS(
 [strchr],
 [],
 [AC_CHECK_FUNCS(
  [memchr],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strchr and memchr],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [strncasecmp],
 [],
 [AC_CHECK_FUNCS(
  [strcasecmp],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strncasecmp and strcasecmp],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [strncmp],
 [],
 [AC_CHECK_FUNCS(
  [strcmp],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strncmp and strcmp],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [strncpy],
 [],
 [AC_CHECK_FUNCS(
  [strcpy],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strncpy and strcpy],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [strrchr],
 [],
 [AC_CHECK_FUNCS(
  [memrchr],
  [AC_CHECK_DECLS(
   [memrchr],
   [],
   [AC_MSG_FAILURE(
    [Missing declaration for function memrchr],
    [1]) ]) ],
  [AC_MSG_FAILURE(
   [Missing functions: strrchr and memrchr],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [strtoll],
 [],
 [AC_CHECK_FUNCS(
  [atoll],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strtoll and atoll],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [strtoull],
 [],
 [AC_CHECK_FUNCS(
  [atoll],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strtoull and atoll],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [vsnprintf],
 [],
 [AC_MSG_FAILURE(
  [Missing function: vsnprintf],
  [1]) ])

dnl File input/output functions used in libbfio/libbfio_file.h
AC_CHECK_FUNCS(
 [close],
 [],
 [AC_MSG_FAILURE(
  [Missing function: close],
  [1]) ])

AC_CHECK_FUNCS(
 [lseek],
 [],
 [AC_MSG_FAILURE(
  [Missing function: lseek],
  [1]) ])

AC_CHECK_FUNCS(
 [open],
 [],
 [AC_MSG_FAILURE(
  [Missing function: open],
  [1]) ])

AC_CHECK_FUNCS(
 [read],
 [],
 [AC_MSG_FAILURE(
  [Missing function: read],
  [1]) ])

AC_CHECK_FUNCS(
 [write],
 [],
 [AC_MSG_FAILURE(
  [Missing function: write],
  [1]) ])

AC_CHECK_FUNCS(
 [fstat],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fstat],
  [1]) ])

dnl Date and time functions used in libewf/libewf_date_time.h
AC_CHECK_FUNCS(
 [localtime_r],
 [],
 [AC_CHECK_FUNCS(
  [localtime],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: localtime_r and localtime],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [mktime],
 [],
 [AC_MSG_FAILURE(
  [Missing function: mktime],
  [1]) ])

dnl Date and time functions used in libsystem/libsystem_date_time.h
LIBESEDB_CHECK_FUNC_CTIME

AC_CHECK_FUNCS(
 [gmtime_r],
 [],
 [AC_CHECK_FUNCS(
  [gmtime],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: gmtime_r and gmtime],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [time],
 [],
 [AC_MSG_FAILURE(
  [Missing function: time],
  [1]) ])

dnl Directory functions used in libsystem/libsystem_directory_io.h
AC_CHECK_FUNCS(
 [chdir],
 [],
 [AC_MSG_FAILURE(
  [Missing functions: chdir],
  [1]) ])

AC_CHECK_FUNCS(
 [mkdir],
 [],
 [AC_MSG_FAILURE(
  [Missing functions: mkdir],
  [1]) ])

dnl Check for error string functions used in libsystem/libsystem_error.c
AC_FUNC_STRERROR_R(
 [],
 [AC_CHECK_FUNCS(
  [strerror],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strerror_r and strerror],
   [1]) ]) ])

dnl Commandline argument/option parsing functions in libsystem/libsystem_getopt.h
AC_CHECK_FUNCS([getopt])

dnl Check for locale functions in libsystem/libsystem_support.h
AC_CHECK_FUNCS(
 [setlocale],
 [],
 [AC_MSG_FAILURE(
  [Missing function: setlocale],
  [1]) ])

LIBESEDB_LANGINFO_CODESET

dnl Other functions

dnl Check for printf conversion specifier support
LIBESEDB_PRINTF_JD
LIBESEDB_PRINTF_ZD

dnl Checks for libraries and library functions
dnl ac_libesedb_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments

dnl Check for enabling libuna for Unicode string support
AC_CHECK_HEADERS(
 [libuna.h],
 [],
 [ac_libesedb_have_libuna=no])

AS_IF(
	[test "x$ac_libesedb_have_libuna" != xno],
	[ac_libesedb_have_libuna=yes
	AC_CHECK_LIB(
	 una,
	 libuna_get_version,
	 [],
	 [ac_libesedb_have_libuna=no])

	dnl Byte stream functions
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_string_size_from_byte_stream,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_string_copy_from_byte_stream,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_byte_stream_size_from_utf8,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_byte_stream_copy_from_utf8,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_string_size_from_byte_stream,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_string_copy_from_byte_stream,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_byte_stream_size_from_utf16,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_byte_stream_copy_from_utf16,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf32_string_size_from_byte_stream,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf32_string_copy_from_byte_stream,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_byte_stream_size_from_utf32,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_byte_stream_copy_from_utf32,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])

	dnl UTF-8 stream functions
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_string_size_from_utf8_stream,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_string_copy_from_utf8_stream,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_stream_size_from_utf8,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_stream_copy_from_utf8,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_string_size_from_utf8_stream,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_string_copy_from_utf8_stream,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_stream_size_from_utf16,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_stream_copy_from_utf16,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf32_string_size_from_utf8_stream,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf32_string_copy_from_utf8_stream,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_stream_size_from_utf32,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_stream_copy_from_utf32,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])

	dnl UTF-16 stream functions
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_string_size_from_utf16_stream,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_string_copy_from_utf16_stream,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_stream_size_from_utf8,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_stream_copy_from_utf8,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_string_size_from_utf16_stream,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_string_copy_from_utf16_stream,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_stream_size_from_utf16,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_stream_copy_from_utf16,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf32_string_size_from_utf16_stream,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf32_string_copy_from_utf16_stream,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_stream_size_from_utf32,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_stream_copy_from_utf32,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libuna=no])
	])

AS_IF(
	[test "x$ac_libesedb_have_libuna" = xyes],
	[AC_SUBST(
	 [libesedb_pc_libs_private_libuna],
	 [-luna])
	AC_SUBST(
	 [libesedb_spec_requires_libuna],
	 [libuna])
	AC_SUBST(
	 [libesedb_spec_build_requires_libuna],
	 [libuna-devel])
	AC_SUBST(
	 [LIBUNA_CPPFLAGS],
	 [])
	AC_SUBST(
	 [LIBUNA_LIBADD],
	 []) ],
	[dnl If libuna is not available use the (local) version that came with libesedb
	AC_DEFINE(
	 [HAVE_LOCAL_LIBUNA],
	 [1],
	 [Define to 1 if the local version of libuna is used.])
	AC_SUBST(
	 [HAVE_LOCAL_LIBUNA],
	 [1])
	AC_SUBST(
	 [libesedb_pc_libs_private_libuna],
	 [])
	AC_SUBST(
	 [libesedb_spec_requires_libuna],
	 [])
	AC_SUBST(
	 [libesedb_spec_build_requires_libuna],
	 [])
	AC_SUBST(
	 [LIBUNA_CPPFLAGS],
	 [-I../libuna])
	AC_SUBST(
	 [LIBUNA_LIBADD],
	 [../libuna/libuna.la])
	ac_libesedb_have_libuna=local])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBUNA],
 [test "x$ac_libesedb_have_libuna" = xlocal])

dnl Check for enabling libbfio for basic file IO support
AC_CHECK_HEADERS(
 [libbfio.h],
 [],
 [ac_libesedb_have_libbfio=no])

AS_IF(
	[test "x$ac_libesedb_have_libbfio" != xno],
	[ac_libesedb_have_libbfio=yes
	AC_CHECK_LIB(
	 bfio,
	 libbfio_get_version,
	 [],
	 [ac_libesedb_have_libbfio=no])

	dnl Handle functions
	AC_CHECK_LIB(
	 bfio,
	 libbfio_handle_free,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libbfio=no])
	AC_CHECK_LIB(
	 bfio,
	 libbfio_handle_open,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libbfio=no])
	AC_CHECK_LIB(
	 bfio,
	 libbfio_handle_close,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libbfio=no])
	AC_CHECK_LIB(
	 bfio,
	 libbfio_handle_read,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libbfio=no])

	AC_CHECK_LIB(
	 bfio,
	 libbfio_handle_set_track_offsets_read,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libbfio=no])
	AC_CHECK_LIB(
	 bfio,
	 libbfio_handle_get_offset_read,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libbfio=no])
	AC_CHECK_LIB(
	 bfio,
	 libbfio_handle_get_amount_of_offsets_read,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libbfio=no])

	dnl File functions
	AC_CHECK_LIB(
	 bfio,
	 libbfio_file_initialize,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libbfio=no])
	AC_CHECK_LIB(
	 bfio,
	 libbfio_file_set_name,
	 [ac_libesedb_dummy=yes],
	 [ac_libesedb_have_libbfio=no])

	AS_IF(
		[test "x$ac_cv_libesedb_enable_wide_character_type" != xno ],
		[AC_CHECK_LIB(
		 bfio,
		 libbfio_file_set_name_wide,
		 [ac_libesedb_dummy=yes],
		 [ac_libesedb_have_libbfio=no])
		])
	])

AS_IF(
	[test "x$ac_libesedb_have_libbfio" = xyes],
	[AC_SUBST(
	 [libesedb_pc_libs_private_libbfio],
	 [-lbfio])
	AC_SUBST(
	 [libesedb_spec_requires_libbfio],
	 [libbfio])
	AC_SUBST(
	 [libesedb_spec_build_requires_libbfio],
	 [libbfio-devel])
	AC_SUBST(
	 [LIBBFIO_CPPFLAGS],
	 [])
	AC_SUBST(
	 [LIBBFIO_LIBADD],
	 [])
	AC_SUBST(
	 [HAVE_LIBBFIO],
	 [1]) ],
	[dnl If libbfio is not available use the (local) version that came with libesedb
	AC_DEFINE(
	 [HAVE_LOCAL_LIBBFIO],
	 [1],
	 [Define to 1 if the local version of libbfio is used.])
	AC_SUBST(
	 [HAVE_LOCAL_LIBBFIO],
	 [1])
	AC_SUBST(
	 [libesedb_pc_libs_private_libbfio],
	 [])
	AC_SUBST(
	 [libesedb_spec_requires_libbfio],
	 [])
	AC_SUBST(
	 [libesedb_spec_build_requires_libbfio],
	 [])
	AC_SUBST(
	 [LIBBFIO_CPPFLAGS],
	 [-I../libbfio])
	AC_SUBST(
	 [LIBBFIO_LIBADD],
	 [../libbfio/libbfio.la])
	AC_SUBST(
	 [HAVE_LIBBFIO],
	 [0])
	ac_libesedb_have_libbfio=local])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBBFIO],
 [test "x$ac_libesedb_have_libbfio" = xlocal])

dnl Check for enabling libfdatetime for file date and time support
AC_CHECK_HEADERS(
 [libfdatetime.h],
 [],
 [ac_libesedb_have_libfdatetime=no])

AS_IF(
	[test "x$ac_libesedb_have_libfdatetime" != xno],
	[ac_libesedb_have_libfdatetime=yes
	AC_CHECK_LIB(
	 fdatetime,
	 libfdatetime_get_version,
	 [],
	 [ac_libesedb_have_libfdatetime=no])

	])

AS_IF(
	[test "x$ac_libesedb_have_libfdatetime" = xyes],
	[AC_SUBST(
	 [libesedb_pc_libs_private_libfdatetime],
	 [-lfdatetime])
	AC_SUBST(
	 [libesedb_spec_requires_libfdatetime],
	 [libfdatetime])
	AC_SUBST(
	 [libesedb_spec_build_requires_libfdatetime],
	 [libfdatetime-devel])
	AC_SUBST(
	 [LIBFDATETIME_CPPFLAGS],
	 [])
	AC_SUBST(
	 [LIBFDATETIME_LIBADD],
	 []) ],
	[dnl If libfdatetime is not available use the (local) version that came with libesedb
	AC_DEFINE(
	 [HAVE_LOCAL_LIBFDATETIME],
	 [1],
	 [Define to 1 if the local version of libfdatetime is used.])
	AC_SUBST(
	 [HAVE_LOCAL_LIBFDATETIME],
	 [1])
	AC_SUBST(
	 [libesedb_pc_libs_private_libfdatetime],
	 [])
	AC_SUBST(
	 [libesedb_spec_requires_libfdatetime],
	 [])
	AC_SUBST(
	 [libesedb_spec_build_requires_libfdatetime],
	 [])
	AC_SUBST(
	 [LIBFDATETIME_CPPFLAGS],
	 [-I../libfdatetime])
	AC_SUBST(
	 [LIBFDATETIME_LIBADD],
	 [../libfdatetime/libfdatetime.la])
	ac_libesedb_have_libfdatetime=local])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBFDATETIME],
 [test "x$ac_libesedb_have_libfdatetime" = xlocal])

dnl Check if libesedb should be build with wide character type support
AS_IF(
	[test "x$ac_cv_libesedb_enable_wide_character_type" != xno ],
	[dnl Check for the wide character header
	AC_CHECK_HEADERS(
	 [wchar.h],
	 [AC_DEFINE(
	  [HAVE_WCHAR_H],
	  [1],
	  [Define to 1 if you have the <wchar.h> header file.])
	 AC_SUBST(
	  [HAVE_WCHAR_H],
	  [1]) ],
	 [AC_MSG_FAILURE(
	  [Missing header: wchar.h],
	  [1]) ])

	dnl Determine wide character byte size
	AC_CHECK_SIZEOF([wchar_t])

	dnl Check for wide character string functions
	dnl Wide character string functions used in common/string_conversion.h
	AC_CHECK_FUNCS(
	 [btowc],
	 [],
	 [AC_MSG_FAILURE(
	  [Missing functions: btowc],
	  [1]) ])

	AC_CHECK_FUNCS(
	 [wctob],
	 [],
	 [AC_MSG_FAILURE(
	  [Missing functions: wctob],
	  [1]) ])

	dnl Wide character string functions used in common/wide_string.h
	AC_CHECK_FUNCS(
	 [fgetws],
	 [],
	 [AC_MSG_FAILURE(
	  [Missing functions: fgetws],
	  [1]) ])

	AC_CHECK_FUNCS(
	 [swprintf],
	 [],
	 [AC_MSG_FAILURE(
	  [Missing functions: swprintf],
	  [1]) ])

	AC_CHECK_FUNCS(
	 [wcschr],
	 [],
	 [AC_CHECK_FUNCS(
	  [wmemchr],
	  [],
	  [AC_MSG_FAILURE(
	   [Missing functions: wcschr and wmemchr],
	   [1]) ]) ])

	AC_CHECK_FUNCS(
	 [wcsncasecmp],
	 [],
	 [AC_CHECK_FUNCS(
	  [wcscasecmp],
	  [],
	  [AC_MSG_FAILURE(
	   [Missing functions: wcsncasecmp and wcscasecmp],
	   [1]) ]) ])

	AC_CHECK_FUNCS(
	 [wcsncmp],
	 [],
	 [AC_CHECK_FUNCS(
	  [wmemcmp],
	  [],
	  [AC_CHECK_FUNCS(
	   [wcscmp],
	   [],
	   [AC_MSG_FAILURE(
	    [Missing functions: wcsncmp, wmemcmp and wcscmp],
	    [1]) ]) ]) ])

	AC_CHECK_FUNCS(
	 [wcsncpy],
	 [],
	 [AC_CHECK_FUNCS(
	  [wmemcpy],
	  [],
	  [AC_CHECK_FUNCS(
	   [wcscpy],
	   [],
	   [AC_MSG_FAILURE(
	    [Missing functions: wcsncpy, wmemcpy and wcscpy],
	    [1]) ]) ]) ])

	AC_CHECK_FUNCS(
	 [wcslen],
	 [],
	 [AC_MSG_FAILURE(
	  [Missing functions: wcslen],
	  [1]) ])

	AC_CHECK_FUNCS(
	 [wcsrchr],
	 [],
	 [AC_CHECK_FUNCS(
	  [wmemrchr],
	  [],
	  [AC_MSG_FAILURE(
	   [Missing functions: wcsrchr and wmemrchr],
	   [1]) ]) ])

	AC_CHECK_FUNCS(
	 [wcstoll],
	 [],
	 [AC_MSG_FAILURE(
	  [Missing functions: wcstoll],
	  [1]) ])

	AC_CHECK_FUNCS(
	 [wcstoull],
	 [],
	 [AC_MSG_FAILURE(
	  [Missing functions: wcstoull],
	  [1]) ])

	dnl Make the necessary definitions
	AC_DEFINE(
	 [HAVE_WIDE_CHARACTER_TYPE],
	 [1],
	 [Defines if wide character type should be used])
	AC_SUBST(
	 [HAVE_WIDE_CHARACTER_TYPE],
	 [1])
	ac_cv_libesedb_enable_wide_character_type=yes],

	[AC_SUBST(
	 [HAVE_WIDE_CHARACTER_TYPE],
	 [0])
	AC_SUBST(
	 [HAVE_WCHAR_H],
	 [0]) ])

AC_SUBST(
 [HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS],
 [0])

dnl Check if esedbtools should be build as static executables
AS_IF(
	[test "x$ac_cv_libesedb_enable_static_executables" != xno],
	[STATIC_LDFLAGS="-all-static";
	AC_SUBST(
	 [STATIC_LDFLAGS])
	ac_cv_libesedb_enable_static_executables=yes])

dnl Check if libesedb should be build with verbose output
AS_IF(
	[test "x$ac_cv_libesedb_enable_verbose_output" != xno ],
	[AC_DEFINE(
	 [HAVE_VERBOSE_OUTPUT],
	 [1],
	 [Defines if verbose output should be used] )
	ac_cv_libesedb_enable_verbose_output=yes])

dnl Check if libesedb should be build with debug output
AS_IF(
	[test "x$ac_cv_libesedb_enable_debug_output" != xno ],
	[AC_DEFINE(
	 [HAVE_DEBUG_OUTPUT],
	 [1],
	 [Defines if debug output should be used] )
	ac_cv_libesedb_enable_debug_output=yes])

dnl Check if requires and build requires should be set in spec file
AS_IF(
	[test "x$ac_libesedb_have_libuna" = xyes || test "x$ac_libesedb_have_libbfio" = xyes || test "x$ac_libesedb_have_libfdatetime" = xyes],
	[AC_SUBST(
	 [libesedb_spec_requires],
	 [Requires:])
	AC_SUBST(
	 [libesedb_spec_build_requires],
	 [BuildRequires:]) ],
	[AC_SUBST(
	 [libesedb_spec_requires],
	 [])
	AC_SUBST(
	 [libesedb_spec_build_requires],
	 [])
	ac_libesedb_dummy=yes])

dnl Set memwatch support
ac_libesedb_have_memwatch=yes

AS_IF(
	[test "x$ac_libesedb_have_memwatch" != xno ],
	[AC_SUBST(
	 [MEMWATCH_CPPFLAGS],
	 ["-DMEMWATCH -DMEMWATCH_STDIO"])
	AC_SUBST(
	 [MEMWATCH_LIBADD],
	 [../common/libmemwatch.la]) ])

AM_CONDITIONAL(
 [HAVE_MEMWATCH],
 [test "x$ac_libesedb_have_memwatch" != xno])

dnl Set additional compiler flags
CFLAGS="$CFLAGS -Wall";

dnl Check if DLL support is needed in esedbtools
AS_IF(
	[test x"$enable_shared" = xyes],
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
AC_CONFIG_FILES([liberror/Makefile])
AC_CONFIG_FILES([libnotify/Makefile])
AC_CONFIG_FILES([libuna/Makefile])
AC_CONFIG_FILES([libbfio/Makefile])
AC_CONFIG_FILES([libfdatetime/Makefile])
AC_CONFIG_FILES([libesedb/Makefile])
AC_CONFIG_FILES([libsystem/Makefile])
AC_CONFIG_FILES([esedbtools/Makefile])
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
AC_CONFIG_FILES([macosx/Introduction.rtf])
AC_CONFIG_FILES([libesedb.pc])
AC_CONFIG_FILES([libesedb.spec])
dnl Have configure write its configuration
AC_CONFIG_HEADERS([common/config.h])

AC_OUTPUT

dnl Print a summary
AC_MSG_NOTICE([
Building:
   libuna support:                             $ac_libesedb_have_libuna
   libbfio support:                            $ac_libesedb_have_libbfio
   libfdatetime support:                       $ac_libesedb_have_libfdatetime

Features:
   Wide character type support:                $ac_cv_libesedb_enable_wide_character_type
   esedbtools are build as static executables: $ac_cv_libesedb_enable_static_executables
   Verbose output:                             $ac_cv_libesedb_enable_verbose_output
   Debug output:                               $ac_cv_libesedb_enable_debug_output
]);


