/*
 * The libfdatetime header wrapper
 *
 * Copyright (c) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _LIBESEDB_LIBFDATETIME_H )
#define _LIBESEDB_LIBFDATETIME_H

#include <common.h>

/* Define HAVE_LOCAL_LIBFDATETIME for local use of libfdatetime
 */
#if defined( HAVE_LOCAL_LIBFDATETIME )

#include <libfdatetime_definitions.h>
#include <libfdatetime_error.h>
#include <libfdatetime_fat_date_time.h>
#include <libfdatetime_filetime.h>
#include <libfdatetime_types.h>

#elif defined( HAVE_LIBFDATETIME_H )

/* If libtool DLL support is enabled set LIBFDATETIME_DLL_IMPORT
 * before including libfdatetime.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBFDATETIME_DLL_IMPORT
#endif

#include <libfdatetime.h>

#else
#error Missing libfdatetime.h
#endif

#endif

