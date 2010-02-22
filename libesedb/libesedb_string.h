/*
 * Character type string functions
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _LIBESEDB_STRING_H )
#define _LIBESEDB_STRING_H

#include <common.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include <liberror.h>

#include "libesedb_libuna.h"

#if defined( _cplusplus )
extern "C" {
#endif

/* The internal string type contains UTF-8
 */
typedef libuna_utf8_character_t libesedb_character_t;

#define PRIc_LIBESEDB	"c"
#define PRIs_LIBESEDB	"s"

#define _LIBESEDB_STRING( string ) \
	(libesedb_character_t *) string

#define libesedb_string_compare( string1, string2, size ) \
	narrow_string_compare( (char *) string1, (char *) string2, size )

#define libesedb_string_copy( destination, source, size ) \
	narrow_string_copy( (char *) destination, (char *) source, size )

#define libesedb_string_length( string ) \
	narrow_string_length( (char *) string )

#define libesedb_string_search( string, character, size ) \
	(libesedb_character_t *) narrow_string_search( (char *) string, (int) character, size )

#define libesedb_string_search_reverse( string, character, size ) \
	(libesedb_character_t *) narrow_string_search_reverse( (char *) string, (int) character, size )

#define libesedb_string_snprintf( target, size, format, ... ) \
	narrow_string_snprintf( (char *) target, size, format, __VA_ARGS__ )

#define libesedb_string_to_int64( string, end_of_string, base ) \
	narrow_string_to_signed_long_long( (char *) string, (char **) end_of_string, base )

#define libesedb_string_to_uint64( string, end_of_string, base ) \
	narrow_string_to_unsigned_long_long( (char *) string, (char **) end_of_string, base )

/* Byte stream functions
 */
#define libesedb_string_size_from_byte_stream( stream, stream_size, codepage, string_size, error ) \
	libuna_utf8_string_size_from_byte_stream( stream, stream_size, codepage, string_size, error )

#define libesedb_string_copy_from_byte_stream( string, string_size, stream, stream_size, codepage, error ) \
	libuna_utf8_string_copy_from_byte_stream( string, string_size, stream, stream_size, codepage, error )

#define byte_stream_size_from_libesedb_string( string, string_size, codepage, stream_size, error ) \
	libuna_byte_stream_size_from_utf8( string, string_size, codepage, stream_size, error )

#define byte_stream_copy_from_libesedb_string( stream, stream_size, codepage, string, string_size, error ) \
	libuna_byte_stream_copy_from_utf8( stream, stream_size, codepage, string, string_size, error )

/* UTF-8 stream functions
 */
#define libesedb_string_size_from_utf8_stream( stream, stream_size, string_size, error ) \
	libuna_utf8_string_size_from_utf8_stream( stream, stream_size, string_size, error )

#define libesedb_string_copy_from_utf8_stream( string, string_size, stream, stream_size, error ) \
	libuna_utf8_string_copy_from_utf8_stream( string, string_size, stream, stream_size, error )

#define utf8_stream_size_from_libesedb_string( string, string_size, stream_size, error ) \
	libuna_utf8_stream_size_from_utf8( string, string_size, stream_size, error )

#define utf8_stream_copy_from_libesedb_string( stream, stream_size, string, string_size, error ) \
	libuna_utf8_stream_copy_from_utf8( stream, stream_size, string, string_size, error )

/* UTF-16 stream functions
 */
#define libesedb_string_size_from_utf16_stream( stream, stream_size, byte_order, string_size, error ) \
	libuna_utf8_string_size_from_utf16_stream( stream, stream_size, byte_order, string_size, error )

#define libesedb_string_copy_from_utf16_stream( string, string_size, stream, stream_size, byte_order, error ) \
	libuna_utf8_string_copy_from_utf16_stream( string, string_size, stream, stream_size, byte_order, error )

#define utf16_stream_size_from_libesedb_string( string, string_size, stream_size, error ) \
	libuna_utf16_stream_size_from_utf8( string, string_size, stream_size, error )

#define utf16_stream_copy_from_libesedb_string( stream, stream_size, byte_order, string, string_size, error ) \
	libuna_utf16_stream_copy_from_utf8( stream, stream_size, byte_order, string, string_size, error )

#if defined( _cplusplus )
}
#endif

#endif

