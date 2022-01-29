/*
 * The libfvalue header wrapper
 *
 * Copyright (C) 2009-2022, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _LIBESEDB_LIBFVALUE_H )
#define _LIBESEDB_LIBFVALUE_H

#include <common.h>

/* Define HAVE_LOCAL_LIBFVALUE for local use of libfvalue
 */
#if defined( HAVE_LOCAL_LIBFVALUE )

#include <libfvalue_codepage.h>
#include <libfvalue_data_handle.h>
#include <libfvalue_definitions.h>
#include <libfvalue_floating_point.h>
#include <libfvalue_integer.h>
#include <libfvalue_split_utf16_string.h>
#include <libfvalue_split_utf8_string.h>
#include <libfvalue_string.h>
#include <libfvalue_table.h>
#include <libfvalue_types.h>
#include <libfvalue_value.h>
#include <libfvalue_value_type.h>
#include <libfvalue_utf16_string.h>
#include <libfvalue_utf8_string.h>

#else

/* If libtool DLL support is enabled set LIBFVALUE_DLL_IMPORT
 * before including libfvalue.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBFVALUE_DLL_IMPORT
#endif

#include <libfvalue.h>

#endif /* defined( HAVE_LOCAL_LIBFVALUE ) */

#endif /* !defined( _LIBESEDB_LIBFVALUE_H ) */

