/*
 * ASCII 7-bit (un)compression functions
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

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "ascii7.h"

/* Determines the uncompressed UTF-8 string size from the ASCII 7-bit compressed data
 * Return 1 on success or -1 on error
 */
int ascii7_decompress_get_utf8_string_size(
     uint8_t *compressed_data,
     size_t compressed_data_size,
     size_t *utf8_string_size,
     liberror_error_t **error )
{
	static char *function = "ascii7_decompress_get_utf8_string_size";

	if( compressed_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf8_string_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string size.",
		 function );

		return( -1 );
	}
	if( compressed_data_size <= 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: compressed data size value too small.",
		 function );

		return( -1 );
	}
	/* The uncompressed data size is: ( ( compressed data size - 1 ) * 2 ) + 2
	 * The first byte in the compressed data contains a bitmask no data.
	 * Every other compressed byte is uncompressed as an UTF-8 character.
	 * An additional end of string characters is added.
	 */
	*utf8_string_size = compressed_data_size;

	return( 1 );
}

/* Determines the uncompressed UTF-16 string size from the ASCII 7-bit compressed data
 * Return 1 on success or -1 on error
 */
int ascii7_decompress_get_utf16_string_size(
     uint8_t *compressed_data,
     size_t compressed_data_size,
     size_t *utf16_string_size,
     liberror_error_t **error )
{
	static char *function = "ascii7_decompress_get_utf8_string_size";

	if( compressed_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_string_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string size.",
		 function );

		return( -1 );
	}
	if( compressed_data_size <= 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: compressed data size value too small.",
		 function );

		return( -1 );
	}
	/* The uncompressed data size is: ( ( compressed data size - 1 ) * 2 ) + 2
	 * The first byte in the compressed data contains a bitmask no data.
	 * Every other compressed byte is uncompressed as an UTF-16 little-endian
	 * character. An additional end of string characters is added.
	 */
	*utf16_string_size = compressed_data_size;

	return( 1 );
}

/* Decompresses a ASCII 7-bit compressed string into a UTF-8 little-endian string
 * Returns 1 on success or -1 on error
 */
int ascii7_decompress_to_utf8_string(
     uint8_t *compressed_data, 
     size_t compressed_data_size,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     liberror_error_t **error )
{
	static char *function           = "ascii7_decompress_to_utf8_string";
	size_t compressed_data_iterator = 0;
	size_t utf8_string_iterator     = 0;
	uint8_t bitmask                 = 0;

	if( compressed_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( compressed_data_size <= 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: compressed data size value too small.",
		 function );

		return( -1 );
	}
	if( utf8_string_size < compressed_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string value too small.",
		 function );

		return( -1 );
	}
	for( compressed_data_iterator = 1;
	     compressed_data_iterator < compressed_data_size;
	     compressed_data_iterator++ )
	{
		if( ( compressed_data_iterator % 4 ) == 0 )
		{
			bitmask = compressed_data[ 0 ] ^ ( (uint8_t) compressed_data_iterator + 1 );
		}
		else if( ( compressed_data_iterator % 4 ) == 1 )
		{
			bitmask = (uint8_t) compressed_data_iterator;

			if( compressed_data_size <= 256 )
			{
				bitmask -= 1;
			}
		}
		else if( ( compressed_data_iterator % 4 ) == 2 )
		{
			bitmask = (uint8_t) compressed_data_iterator;
		}
		else if( ( compressed_data_iterator % 4 ) == 3 )
		{
			bitmask = (uint8_t) compressed_data_iterator ^ 0x05;
		}
		utf8_string[ utf8_string_iterator++ ] = compressed_data[ compressed_data_iterator ] ^ bitmask;
	}
	utf8_string[ utf8_string_iterator++ ] = 0;

	return( 1 );
}

/* Decompresses a ASCII 7-bit compressed string into a UTF-16 little-endian string
 * Returns 1 on success or -1 on error
 */
int ascii7_decompress_to_utf16_string(
     uint8_t *compressed_data, 
     size_t compressed_data_size,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     liberror_error_t **error )
{
	static char *function           = "ascii7_decompress_to_utf16_string";
	size_t compressed_data_iterator = 0;
	size_t utf16_string_iterator    = 0;
	uint8_t bitmask                 = 0;

	if( compressed_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( compressed_data_size <= 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: compressed data size value too small.",
		 function );

		return( -1 );
	}
	if( utf16_string_size < compressed_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string value too small.",
		 function );

		return( -1 );
	}
	for( compressed_data_iterator = 1;
	     compressed_data_iterator < compressed_data_size;
	     compressed_data_iterator++ )
	{
		if( ( compressed_data_iterator % 4 ) == 0 )
		{
			bitmask = compressed_data[ 0 ] ^ ( (uint8_t) compressed_data_iterator + 1 );
		}
		else if( ( compressed_data_iterator % 4 ) == 1 )
		{
			bitmask = (uint8_t) compressed_data_iterator;

			if( compressed_data_size <= 256 )
			{
				bitmask -= 1;
			}
		}
		else if( ( compressed_data_iterator % 4 ) == 2 )
		{
			bitmask = (uint8_t) compressed_data_iterator;
		}
		else if( ( compressed_data_iterator % 4 ) == 3 )
		{
			bitmask = (uint8_t) compressed_data_iterator ^ 0x05;
		}
		utf16_string[ utf16_string_iterator++ ] = compressed_data[ compressed_data_iterator ] ^ bitmask;
	}
	utf16_string[ utf16_string_iterator++ ] = 0;

	return( 1 );
}

