/*
 * The libfmapi header wrapper
 *
 * Copyright (C) 2009-2017, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _ESEDBTOOLS_LIBFMAPI_H )
#define _ESEDBTOOLS_LIBFMAPI_H

#include <common.h>

/* Define HAVE_LOCAL_LIBFMAPI for local use of libfmapi
 */
#if defined( HAVE_LOCAL_LIBFMAPI )

#include <libfmapi_definitions.h>
#include <libfmapi_types.h>

#if defined( HAVE_DEBUG_OUTPUT )
#include <libfmapi_debug.h>
#endif

#else

/* If libtool DLL support is enabled set LIBFMAPI_DLL_IMPORT
 * before including libfmapi.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBFMAPI_DLL_IMPORT
#endif

#include <libfmapi.h>

#endif

#endif

