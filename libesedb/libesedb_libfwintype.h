/*
 * The libfwintype header wrapper
 *
 * Copyright (c) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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

#if !defined( _LIBESEDB_LIBFWINTYPE_H )
#define _LIBESEDB_LIBFWINTYPE_H

#include <common.h>

/* Define HAVE_LOCAL_LIBFWINTYPE for local use of libfwintype
 */
#if defined( HAVE_LOCAL_LIBFWINTYPE )

#include <libfwintype_definitions.h>
#include <libfwintype_error.h>
#include <libfwintype_guid.h>
#include <libfwintype_types.h>

#elif defined( HAVE_LIBFWINTYPE_H )

/* If libtool DLL support is enabled set LIBFWINTYPE_DLL_IMPORT
 * before including libfwintype.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBFWINTYPE_DLL_IMPORT
#endif

#include <libfwintype.h>

#else
#error Missing libfwintype.h
#endif

#endif

