/*
 * GUID functions
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
 * Copyright (c) 2008-2010, Joachim Metz <forensics@hoffmannbv.nl>,
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

#include <common.h>
#include <byte_stream.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

#include "guid.h"

/* Converts the GUID into a string
 * Returns 1 if successful or -1 on error
 */
int guid_to_string(
     guid_t *guid,
     int byte_order,
     libcstring_system_character_t *string,
     size_t string_size,
     liberror_error_t **error )
{
	static char *function = "guid_to_string";
	int print_count       = 0;

	if( guid == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid guid.",
		 function );

		return( -1 );
	}
	if( ( byte_order != _BYTE_STREAM_ENDIAN_BIG )
	 && ( byte_order != _BYTE_STREAM_ENDIAN_LITTLE ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported byte order.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size < GUID_STRING_SIZE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: string too small.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Create the GUID string
	 * It is stored as uint32 - uint16 - uint16 - 8 byte array
	 */
	if( byte_order == _BYTE_STREAM_ENDIAN_BIG )
	{
		print_count = libcstring_system_string_sprintf(
			       string,
			       string_size,
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "-%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "-%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "-%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "-%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 ),
			       guid[ 0 ], guid[ 1 ], guid[ 2 ], guid[ 3 ],
			       guid[ 4 ], guid[ 5 ],
			       guid[ 6 ], guid[ 7 ],
			       guid[ 8 ], guid[ 9 ],
			       guid[ 10 ], guid[ 11 ], guid[ 12 ], guid[ 13 ], guid[ 14 ], guid[ 15 ] );
	}
	else if( byte_order == _BYTE_STREAM_ENDIAN_LITTLE )
	{
		print_count = libcstring_system_string_sprintf(
			       string,
			       string_size,
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "-%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "-%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "-%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "-%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 )
			       _LIBCSTRING_SYSTEM_STRING( "%.2" ) _LIBCSTRING_SYSTEM_STRING( PRIx8 ),
			       guid[ 3 ], guid[ 2 ], guid[ 1 ], guid[ 0 ],
			       guid[ 5 ], guid[ 4 ],
			       guid[ 7 ], guid[ 6 ],
			       guid[ 8 ], guid[ 9 ],
			       guid[ 10 ], guid[ 11 ], guid[ 12 ], guid[ 13 ], guid[ 14 ], guid[ 15 ] );
	}
	if( ( print_count < 0 )
	 || ( (size_t) print_count > string_size ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

