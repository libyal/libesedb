/*
 * The internal libcfile header
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

#if !defined( _ESEDBTOOLS_LIBCFILE_H )
#define _ESEDBTOOLS_LIBCFILE_H

#include <common.h>

/* Define HAVE_LOCAL_LIBCFILE for local use of libcfile
 */
#if defined( HAVE_LOCAL_LIBCFILE )

#include <libcfile_definitions.h>
#include <libcfile_file.h>
#include <libcfile_support.h>
#include <libcfile_types.h>

#else

/* If libtool DLL support is enabled set LIBCFILE_DLL_IMPORT
 * before including libcfile.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT ) && !defined( HAVE_STATIC_EXECUTABLES )
#define LIBCFILE_DLL_IMPORT
#endif

#include <libcfile.h>

#endif /* defined( HAVE_LOCAL_LIBCFILE ) */

#endif /* !defined( _ESEDBTOOLS_LIBCFILE_H ) */

