/*
 * Configuration file for Windows native compilation
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
 * Copyright (c) 2006-2010, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _CONFIG_WINDOWS_H )
#define _CONFIG_WINDOWS_H

#include "config.h"

/* Windows does not have other platform specific headers
 */
#undef HAVE_CYGWIN_FS_H
#undef HAVE_LINUX_FS_H
#undef HAVE_SYS_DISKLABEL_H
#undef HAVE_SYS_DISK_H

/* Windows does not have <inttypes.h> or <stdint.h>
 */
#undef HAVE_INTTYPES_H
#undef HAVE_STDINT_H

#define HAVE_SIZE32_T   0
#define HAVE_SSIZE32_T  0
#define HAVE_SIZE64_T   0
#define HAVE_SSIZE64_T  0
#define HAVE_OFF64_T    0

/* Windows does not have %jd and %zd printf conversion specifiers
 */
#undef HAVE_PRINTF_JD
#undef HAVE_PRINTF_ZD

/* Windows does not have <sys/time.h>
 */
#undef TIME_WITH_SYS_TIME
#undef HAVE_SYS_TIME_H

/* Windows does not have <unistd.h> but uses <io.h> and <share.h> instead
 */
#undef HAVE_UNISTD_H
#define HAVE_IO_H	1
#define HAVE_SHARE_H	1

#if !defined( HAVE_FCNTL_H )
#define HAVE_FCNTL_H    1
#endif

/* Windows does not have <sys/ioctl.h> and <sys/utsname.h>
 */
#undef HAVE_SYS_IOCTL_H
#undef HAVE_SYS_UTSNAME_H

/* Use Windows crypto API instead of libcrypto or equivalent
 */
#undef HAVE_OPENSSL_MD5_H
#undef HAVE_OPENSSL_OPENSSLV_H
#undef HAVE_LIBCRYPTO
#define HAVE_WINCPRYPT_H	1

/* Use Windows GUID (UUID) calculation algorithms instead of libuuid
 */
#undef HAVE_UUID_UUID_H
#undef HAVE_LIBUUID
#undef HAVE_UUID_GENERATE_RANDOM
#undef HAVE_UUID_GENERATE_TIME

/* Windows does not have getopt or <glob.h>
 */
#undef HAVE_GETOPT
#undef HAVE_GLOB_H

/* Windows does not have <signal.h>
 */
#undef HAVE_SIGNAL_H

/* Windows does not have <langinfo.h>
 */
#undef HAVE_LANGINFO_H
#undef HAVE_LANGINFO_CODESET

/* If wide character support was enabled
 * make use of the wide character support functions
 */
#if !defined( HAVE_WIDE_CHARACTER_TYPE )
#define HAVE_WIDE_CHARACTER_TYPE
#endif

#if !defined( HAVE_WCHAR_H )
#define HAVE_WCHAR_H		1
#endif

#if defined( SIZEOF_WCHAR_T )
#undef SIZEOF_WCHAR_T
#endif

#define SIZEOF_WCHAR_T		2

/* Make sure the function definitions are available
 * these should be normally defined in config.h
 * use the following defintions to control the function
 * definitions per source file in common
 */
#define HAVE_DATE_TIME		1
#define HAVE_ERROR_STRING	1
#define HAVE_FILE_IO		1
#define HAVE_NARROW_STRING	1
#define HAVE_WIDE_STRING	1

/* Functions in unatools/date_time.h
 */
#if defined( HAVE_DATE_TIME )
#if !defined( HAVE_CTIME_R )
#define HAVE_CTIME_R		1
#endif

#define HAVE_WCTIME_R		1

#if !defined( HAVE_GMTIME_R )
#define HAVE_GMTIME_R		1
#endif

#if !defined( HAVE_LOCALTIME_R )
#define HAVE_LOCALTIME_R	1
#endif

#if !defined( HAVE_MKTIME )
#define HAVE_MKTIME		1
#endif

#if !defined( HAVE_TIME )
#define HAVE_TIME		1
#endif

#else
#undef HAVE_CTIME_R
#undef HAVE_WCTIME_R
#undef HAVE_GMTIME_R
#undef HAVE_LOCALTIME_R
#undef HAVE_MKTIME
#undef HAVE_TIME
#endif

/* Functions in unatools/error_string.h
 */
#if defined( HAVE_ERROR_STRING )
#if !defined( HAVE_STRERROR_R )
#define HAVE_STRERROR_R		1
#endif

#define HAVE_WCSERROR_R		1

#else
#undef HAVE_STRERROR_R
#undef HAVE_WCSERROR_R
#endif

/* Functions in unatools/file_io.h
 */
#if defined( HAVE_FILE_IO )
#if !defined( HAVE_OPEN )
#define HAVE_OPEN		1
#endif

#define HAVE_WOPEN		1

#if !defined( HAVE_CLOSE )
#define HAVE_CLOSE		1
#endif

#if !defined( HAVE_LSEEK )
#define HAVE_LSEEK		1
#endif

#if !defined( HAVE_READ )
#define HAVE_READ		1
#endif

#if !defined( HAVE_WRITE )
#define HAVE_WRITE		1
#endif

#else
#undef HAVE_OPEN
#undef HAVE_WOPEN
#undef HAVE_CLOSE
#undef HAVE_LSEEK
#undef HAVE_READ
#undef HAVE_WRITE
#endif

/* Functions in common/narrow_string.h
 */
#if defined( HAVE_NARROW_STRING )
#if !defined( HAVE_STRLEN )
#define HAVE_STRLEN		1
#endif

#if !defined( HAVE_STRNCMP )
#define HAVE_STRNCMP		1
#endif

#if !defined( HAVE_STRNCPY )
#define HAVE_STRNCPY		1
#endif

#if !defined( HAVE_STRCHR )
#define HAVE_STRCHR		1
#endif

#if !defined( HAVE_STRRCHR )
#define HAVE_STRRCHR		1
#endif

#if !defined( HAVE_FGETS )
#define HAVE_FGETS		1
#endif

#else
#undef HAVE_STRLEN
#undef HAVE_STRNCMP
#undef HAVE_STRNCPY
#undef HAVE_STRCHR
#undef HAVE_STRRCHR
#undef HAVE_FGETS
#endif

/* Functions in common/wide_string.h
 */
#if defined( HAVE_WIDE_STRING )
#if !defined( HAVE_WCSLEN )
#define HAVE_WCSLEN		1
#endif

#if !defined( HAVE_WCSNCMP )
#define HAVE_WCSNCMP		1
#endif

#if !defined( HAVE_WCSNCPY )
#define HAVE_WCSNCPY		1
#endif

#if !defined( HAVE_WCSCHR )
#define HAVE_WCSCHR		1
#endif

#if !defined( HAVE_WCSRCHR )
#define HAVE_WCSRCHR		1
#endif

#if !defined( HAVE_FGETWS )
#define HAVE_FGETWS		1
#endif

#else
#undef HAVE_WCSLEN
#undef HAVE_WCSNCMP
#undef HAVE_WCSNCPY
#undef HAVE_WCSCHR
#undef HAVE_WCSRCHR
#undef HAVE_FGETWS
#endif

/* Use the native WINAPI functions instead of the POSIX like functions
#define USE_NATIVE_WINAPI_FUNCTIONS	1
 */

#if defined( LIBEWF_OPERATING_SYSTEM )
#undef LIBEWF_OPERATING_SYSTEM
#endif

#define LIBEWF_OPERATING_SYSTEM		"Windows"

#endif

