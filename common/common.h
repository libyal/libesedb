/*
 * Common include file
 *
 * Copyright (c) 2006-2013, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _COMMON_H )
#define _COMMON_H

#if defined( HAVE_CONFIG_H )
#include <config.h>
#endif

/* Check to see if the Microsoft Visual Studio C++ compiler is used
 */
#if defined( _MSC_VER )
#include <config_msc.h>
#endif

/* Check to see if the Borland/CodeGear C++ Builder compiler is used
 */
#if defined( __BORLANDC__ )
#include <config_borlandc.h>
#endif

/* Make sure WINAPI is defined
 */
#if defined( _MSC_VER ) || defined( __BORLANDC__ ) || defined( HAVE_WINDOWS_H )
/* #define WIN32_LEAN_AND_MEAN */

#include <windows.h>
#endif

/* Make sure the WINAPI version is build with both the narrow and wide character functions
 */
#if defined( WINAPI ) && !defined( HAVE_WIDE_CHARACTER_TYPE )
#define HAVE_WIDE_CHARACTER_TYPE	1
#endif

#endif

