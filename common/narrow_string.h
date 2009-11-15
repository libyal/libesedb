/*
 * Narrow character string functions
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

#if !defined( _NARROW_STRING_H )
#define _NARROW_STRING_H

#include "common.h"
#include "types.h"

#if defined( HAVE_GLIB_H )
#include <glib.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_STRING_H ) || defined( WINAPI )
#include <string.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

/* String length
 */
#if defined( HAVE_STRLEN ) || defined( WINAPI )
#define narrow_string_length( string ) \
	strlen( string )
#endif

/* String compare
 */
#if defined( HAVE_STRNCMP ) || defined( WINAPI )
#define narrow_string_compare( string1, string2, size ) \
	strncmp( string1, string2, size )

#elif defined( HAVE_MEMCMP )
#define narrow_string_compare( string1, string2, size ) \
	memcmp( (void *) string1, (void *) string2, size )

#elif defined( HAVE_STRCMP )
#define narrow_string_compare( string1, string2, size ) \
	strcmp( string1, string2 )
#endif

/* Caseless string compare
 */
#if defined( HAVE_GLIB_H )
#define narrow_string_compare_no_case( string1, string2, size ) \
	g_ascii_strncasecmp( string1, string2, size )

#elif defined( _MSC_VER )
#define narrow_string_compare_no_case( string1, string2, size ) \
	_strnicmp( string1, string2, size )

#elif defined( WINAPI )
#define narrow_string_compare_no_case( string1, string2, size ) \
	strnicmp( string1, string2, size )

#elif defined( HAVE_STRNCASECMP )
#define narrow_string_compare_no_case( string1, string2, size ) \
	strncasecmp( string1, string2, size )

#elif defined( HAVE_STRCASECMP )
#define narrow_string_compare_no_case( string1, string2, size ) \
	strcasecmp( string1, string2 )
#endif

/* String copy
 */
#if defined( HAVE_STRNCPY ) || defined( WINAPI )
#define narrow_string_copy( destination, source, size ) \
	strncpy( destination, source, size )

#elif defined( HAVE_MEMCPY )
#define narrow_string_copy( destination, source, size ) \
	(char *) memcpy( (void *) destination, (void *) source, size )

#elif defined( HAVE_STRCPY )
#define narrow_string_copy( destination, source, size ) \
	strcpy( destination, source )
#endif

/* String search
 */
#if defined( HAVE_STRCHR ) || defined( WINAPI )
#define narrow_string_search( string, character, size ) \
	strchr( string, (int) character )

#elif defined( HAVE_MEMCHR )
#define narrow_string_search( string, character, size ) \
	(char *) memchr( (void *) string, (int) character, size )
#endif

/* String reverse search
 */
#if defined( HAVE_STRRCHR ) || defined( WINAPI )
#define narrow_string_search_reverse( string, character, size ) \
	strrchr( string, (int) character )

#elif defined( HAVE_MEMRCHR ) && ( HAVE_DECL_MEMRCHR_ == 1 )
#define narrow_string_search_reverse( string, character, size ) \
	(char *) memrchr( (void *) string, (int) character, size )
#endif

/* String formatted print (snprinf)
 */
#if defined( HAVE_GLIB_H )
#define narrow_string_snprintf( target, size, ... ) \
	g_snprintf( target, size, __VA_ARGS__ )

#elif defined( _MSC_VER )
#define narrow_string_snprintf( target, size, ... ) \
	sprintf_s( target, size, __VA_ARGS__ )

#elif defined( HAVE_SNPRINTF ) || defined( WINAPI )
#define narrow_string_snprintf( target, size, ... ) \
	snprintf( target, size, __VA_ARGS__ )

#elif defined( HAVE_SPRINTF )
#define narrow_string_snprintf( target, size, ... ) \
	sprintf( target, __VA_ARGS__ )
#endif

/* String input conversion (sscanf)
 */
#if defined( HAVE_SSCANF ) || defined( WINAPI )
#define narrow_string_sscanf( string, format, ... ) \
	sscanf( string, format, __VA_ARGS__ )
#endif

/* String to singed long long (int64)
 */
#if defined( HAVE_GLIB_H )
#define narrow_string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) g_ascii_strtoll( string, end_of_string, base )

#elif defined( WINAPI )
#define narrow_string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) _atoi64( string )

#elif defined( HAVE_STRTOLL )
#define narrow_string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) strtoll( string, end_of_string, base )

#elif defined( HAVE_ATOLL )
#define narrow_string_to_signed_long_long( string, end_of_string, base ) \
	(int64_t) atoll( string )
#endif

/* String to unsigned long long (uint64)
 */
#if defined( HAVE_GLIB_H )
#define narrow_string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) g_ascii_strtoull( string, end_of_string, base )

#elif defined( WINAPI )
#define narrow_string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) _atoi64( string )

#elif defined( HAVE_STRTOULL )
#define narrow_string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) strtoull( string, end_of_string, base )

#elif defined( HAVE_ATOLL )
#define narrow_string_to_unsigned_long_long( string, end_of_string, base ) \
	(uint64_t) atoll( string )
#endif

/* Variable arguments formatted print to string function
 */
#if defined( HAVE_GLIB_H )
#define narrow_string_vsnprintf( string, size, format, ... ) \
	g_vsnprintf( string, size, format, __VA_ARGS__ )

/* This definition causes problems
#elif defined( _MSC_VER )
#define narrow_string_vsnprintf( string, size, format, ... ) \
	_vsnprintf_s( string, size, size, format, __VA_ARGS__ )
*/

#elif defined( HAVE_VSNPRINTF ) || defined( WINAPI )
#define narrow_string_vsnprintf( string, size, format, ... ) \
	vsnprintf( string, size, format, __VA_ARGS__ )
#endif

#if defined( __cplusplus )
}
#endif

#endif

