/*
 * Configuration file for Microsoft Visual Studio C++ compiler
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _CONFIG_MSC_H )
#define _CONFIG_MSC_H

/* Define to the address where bug reports for this package should be sent.
 */
#define PACKAGE_BUGREPORT "forensics@hoffmannbv.nl"

/* Use the safe size and offset types
 */
#define HAVE_SIZE32_T   0
#define HAVE_SSIZE32_T  0
#define HAVE_SIZE64_T   0
#define HAVE_SSIZE64_T  0
#define HAVE_OFF64_T    0

/* Define the wide character type
 */
#if !defined( HAVE_WCHAR_H )
#define HAVE_WCHAR_H		1
#endif

#if defined( SIZEOF_WCHAR_T )
#undef SIZEOF_WCHAR_T
#endif

#define SIZEOF_WCHAR_T		2

/* Use the C Runtime (CRT) functions instead of the WINAPI functions
#define USE_CRT_FUNCTIONS	1
 */

/* Disables a Windows 98 safe version of GetFileSizeEx
 * uses a hardcoded version
#define HAVE_GETFILESIZEEX	1
 */

/* Disables a Windows 98 safe version of SetFilePointerEx
 * uses a hardcoded version
#define HAVE_SETFILEPOINTEREX	1
 */

/* Enable verbose output
#define HAVE_VERBOSE_OUTPUT	1
 */

/* Enable debug output
#define HAVE_DEBUG_OUTPUT	1
 */

/* Enable the v1 API
 */
#define HAVE_V1_API		1

/* Enable the v2 API
#define HAVE_V2_API		1
 */

#endif

