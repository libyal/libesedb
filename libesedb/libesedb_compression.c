/*
 * Compression functions
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <liberror.h>

#include "libesedb_compression.h"

/* Decompressed 7-bit ASCII compressed data
 * Returns 1 on success or -1 on error
 */
int libesedb_compression_7bit_ascii_decompress(
     uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     uint8_t *compressed_data, 
     size_t compressed_data_size,
     liberror_error_t **error )
{
	static char *function             = "libesedb_compression_7bit_ascii_decompress";
	size_t compressed_data_iterator   = 0;
	size_t uncompressed_data_iterator = 0;
	uint16_t value_16bit              = 0;
	uint8_t bit_index                 = 0;

	if( uncompressed_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data.",
		 function );

		return( -1 );
	}
	if( uncompressed_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid uncompressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
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
	if( uncompressed_data_size < 1 + ( ( ( compressed_data_size - 1 ) * 8 ) / 7 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: uncompressed data size value too small.",
		 function );

		return( -1 );
	}
	uncompressed_data[ uncompressed_data_iterator++ ] = compressed_data[ 0 ];

	compressed_data_iterator = 1;

	for( compressed_data_iterator = 1;
	     compressed_data_iterator < compressed_data_size;
	     compressed_data_iterator++ )
	{
		value_16bit |= (uint16_t) compressed_data[ compressed_data_iterator ] << bit_index;

		uncompressed_data[ uncompressed_data_iterator++ ] = (uint8_t) ( value_16bit & 0x7f );

		value_16bit >>= 7;

		bit_index++;

		if( bit_index == 7 )
		{
			uncompressed_data[ uncompressed_data_iterator++ ] = value_16bit & 0x7f;

			value_16bit >>= 7;

			bit_index = 0;
		}
	}
	if( value_16bit != 0 )
	{
		uncompressed_data[ uncompressed_data_iterator++ ] = value_16bit & 0x7f;
	}
	return( 1 );
}

