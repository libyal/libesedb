/*
 * Compression functions
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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libesedb_compression.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_libuna.h"

/* Retrieves the size of the decompressed 7-bit compressed-data
 * Returns 1 on success or -1 on error
 */
int libesedb_compression_7bit_decompress_get_size(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     size_t *uncompressed_data_size,
     libcerror_error_t **error )
{
	static char *function = "libesedb_compression_7bit_decompress_get_size";

	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( compressed_data_size < 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: compressed data size value too small.",
		 function );

		return( -1 );
	}
	if( uncompressed_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data size.",
		 function );

		return( -1 );
	}
	if( compressed_data[ 0 ] == 0x18 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported leading byte: 0x%02" PRIx8 ".",
		 function,
		 compressed_data[ 0 ] );

		return( -1 );
	}
	*uncompressed_data_size = ( ( compressed_data_size - 1 ) * 8 ) / 7;

	return( 1 );
}

/* Decompresses 7-bit compressed-data
 * Returns 1 on success or -1 on error
 */
int libesedb_compression_7bit_decompress(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     libcerror_error_t **error )
{
	static char *function          = "libesedb_compression_7bit_decompress";
	size_t compressed_data_index   = 0;
	size_t uncompressed_data_index = 0;
	uint16_t value_16bit           = 0;
	uint8_t bit_index              = 0;

	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( compressed_data_size < 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: compressed data size value too small.",
		 function );

		return( -1 );
	}
	if( uncompressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data.",
		 function );

		return( -1 );
	}
	if( uncompressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid uncompressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( uncompressed_data_size < ( ( ( compressed_data_size - 1 ) * 8 ) / 7 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: uncompressed data size value too small.",
		 function );

		return( -1 );
	}
	if( compressed_data[ 0 ] == 0x18 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported leading byte: 0x%02" PRIx8 ".",
		 function,
		 compressed_data[ 0 ] );

		return( -1 );
	}
	for( compressed_data_index = 1;
	     compressed_data_index < compressed_data_size;
	     compressed_data_index++ )
	{
		value_16bit |= (uint16_t) compressed_data[ compressed_data_index ] << bit_index;

		uncompressed_data[ uncompressed_data_index++ ] = (uint8_t) ( value_16bit & 0x7f );

		value_16bit >>= 7;

		bit_index++;

		if( bit_index == 7 )
		{
			uncompressed_data[ uncompressed_data_index++ ] = (uint8_t) ( value_16bit & 0x7f );

			value_16bit >>= 7;

			bit_index = 0;
		}
	}
	if( value_16bit != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported remainder: 0x%02" PRIx16 ".",
		 function,
		 value_16bit );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the decompressed XPRESS compressed-data
 * Returns 1 on success or -1 on error
 */
int libesedb_compression_xpress_decompress_get_size(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     size_t *uncompressed_data_size,
     libcerror_error_t **error )
{
	static char *function = "libesedb_compression_xpress_decompress_get_size";

	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( compressed_data_size < 3 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: compressed data size value too small.",
		 function );

		return( -1 );
	}
	if( uncompressed_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data size.",
		 function );

		return( -1 );
	}
	if( compressed_data[ 0 ] != 0x18 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported leading byte: 0x%02" PRIx8 ".",
		 function,
		 compressed_data[ 0 ] );

		return( -1 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 &( compressed_data[ 1 ] ),
	 *uncompressed_data_size );

	return( 1 );
}

/* Decompresses XPRESS compressed-data
 * Returns 1 on success or -1 on error
 */
int libesedb_compression_xpress_decompress(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     libcerror_error_t **error )
{
	static char *function                  = "libesedb_compression_xpress_decompress";
	size_t compressed_data_iterator        = 0;
	size_t compression_iterator            = 0;
	size_t compression_shared_byte_index   = 0;
	size_t uncompressed_data_iterator      = 0;
	uint32_t compression_indicator         = 0;
	uint32_t compression_indicator_bitmask = 0;
	int16_t compression_offset             = 0;
	uint16_t compression_size              = 0;
	uint16_t compression_tuple             = 0;

	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( compressed_data_size < 3 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: compressed data size value too small.",
		 function );

		return( -1 );
	}
	if( uncompressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data.",
		 function );

		return( -1 );
	}
	if( uncompressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid uncompressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( compressed_data[ 0 ] != 0x18 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported leading byte: 0x%02" PRIx8 ".",
		 function,
		 compressed_data[ 0 ] );

		return( -1 );
	}
	compressed_data_iterator = 3;

	while( compressed_data_iterator < compressed_data_size )
	{
                byte_stream_copy_to_uint32_little_endian(
                 &( compressed_data[ compressed_data_iterator ] ),
                 compression_indicator );

		compressed_data_iterator += 4;

		for( compression_indicator_bitmask = 0x80000000UL;
		     compression_indicator_bitmask > 0;
		     compression_indicator_bitmask >>= 1 )
		{
			if( compressed_data_iterator >= compressed_data_size )
			{
				break;
			}
			/* If the indicator bit is 0 the data is uncompressed
			 * or 1 if the data is compressed
			 */
			if( ( compression_indicator & compression_indicator_bitmask ) != 0 )
			{
				if( ( compressed_data_iterator + 1 ) >= compressed_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: compressed data too small.",
					 function );

					return( -1 );
				}
		                byte_stream_copy_to_uint16_little_endian(
		                 &( compressed_data[ compressed_data_iterator ] ),
		                 compression_tuple );

				compressed_data_iterator += 2;

				/* The compression tuple contains:
				 * 0 - 2	the size
				 * 3 - 15	the offset - 1
				 */
				compression_size   = ( compression_tuple & 0x0007 );
				compression_offset = ( compression_tuple >> 3 ) + 1;

				/* Check for a first level extended size
				 * stored in the 4-bits of a shared extended compression size byte
				 * the size is added to the previous size
				 */
				if( compression_size == 0x07 )
				{
					if( compression_shared_byte_index == 0 )
					{
						if( compressed_data_iterator >= compressed_data_size )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
							 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
							 "%s: compressed data too small.",
							 function );

							return( -1 );
						}
						compression_size += compressed_data[ compressed_data_iterator ] & 0x0f;

						compression_shared_byte_index = compressed_data_iterator++;
					}
					else
					{
						compression_size += compressed_data[ compression_shared_byte_index ] >> 4;

						compression_shared_byte_index = 0;
					}
				}
				/* Check for a second level extended size
				 * stored in the 8-bits of the next byte
				 * the size is added to the previous size
				 */
				if( compression_size == ( 0x07 + 0x0f ) )
				{
					if( compressed_data_iterator >= compressed_data_size )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
						 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
						 "%s: compressed data too small.",
						 function );

						return( -1 );
					}
					compression_size += compressed_data[ compressed_data_iterator++ ];
				}
				/* Check for a third level extended size
				 * stored in the 16-bits of the next two bytes
				 * the previous size is ignored
				 */
				if( compression_size == ( 0x07 + 0x0f + 0xff ) )
				{
					if( ( compressed_data_iterator + 1 ) >= compressed_data_size )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
						 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
						 "%s: compressed data too small.",
						 function );

						return( -1 );
					}
			                byte_stream_copy_to_uint16_little_endian(
			                 &( compressed_data[ compressed_data_iterator ] ),
			                 compression_size );
		
					compressed_data_iterator += 2;
				}
				/* The size value is stored as
				 * size - 3
				 */
				compression_size += 3;

				if( compression_size > 32771 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: compression size value out of bounds.",
					 function );

					return( -1 );
				}
				compression_iterator = uncompressed_data_iterator - compression_offset;

				while( compression_size > 0 )
				{
					if( compression_iterator > uncompressed_data_iterator )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
						 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
						 "compression iterator: %" PRIzd " out of range: %" PRIzd ".",
						 function,
						 compression_iterator,
						 uncompressed_data_iterator );

						return( -1 );
					}
					if( uncompressed_data_iterator > uncompressed_data_size )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
						 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
						 "%s: uncompressed data too small.",
						 function );

						return( -1 );
					}
					uncompressed_data[ uncompressed_data_iterator++ ] = uncompressed_data[ compression_iterator++ ];

					compression_size--;
				}
			}
			else
			{
				if( compressed_data_iterator >= compressed_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: compressed data too small.",
					 function );

					return( -1 );
				}
				if( uncompressed_data_iterator > uncompressed_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: uncompressed data too small.",
					 function );

					return( -1 );
				}
				uncompressed_data[ uncompressed_data_iterator++ ] = compressed_data[ compressed_data_iterator++ ];
			}
		}
	}
	return( 1 );
}

/* Retrieves the size of the decompressed compressed-data
 * Returns 1 on success or -1 on error
 */
int libesedb_compression_decompress_get_size(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     size_t *uncompressed_data_size,
     libcerror_error_t **error )
{
	static char *function = "libesedb_compression_decompress_get_size";
	int result            = 0;

	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( compressed_data_size < 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: compressed data size value too small.",
		 function );

		return( -1 );
	}
	if( compressed_data[ 0 ] == 0x18 )
	{
		result = libesedb_compression_xpress_decompress_get_size(
		          compressed_data,
		          compressed_data_size,
		          uncompressed_data_size,
		          error );
	}
	else
	{
		result = libesedb_compression_7bit_decompress_get_size(
		          compressed_data,
		          compressed_data_size,
		          uncompressed_data_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable retrieve uncompressed data size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Decompresses compressed-data
 * Returns 1 on success or -1 on error
 */
int libesedb_compression_decompress(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     libcerror_error_t **error )
{
	static char *function = "libesedb_compression_decompress";
	int result            = 0;

	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( compressed_data_size < 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: compressed data size value too small.",
		 function );

		return( -1 );
	}
	if( compressed_data[ 0 ] == 0x18 )
	{
		result = libesedb_compression_xpress_decompress(
		          compressed_data,
		          compressed_data_size,
		          uncompressed_data,
		          uncompressed_data_size,
		          error );
	}
	else
	{
		result = libesedb_compression_7bit_decompress(
		          compressed_data,
		          compressed_data_size,
		          uncompressed_data,
		          uncompressed_data_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
		 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
		 "%s: unable decompressed data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string size of compressed-data
 * Returns 1 on success or -1 on error
 */
int libesedb_compression_get_utf8_string_size(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	uint8_t *uncompressed_data    = NULL;
	static char *function         = "libesedb_compression_get_utf8_string_size";
	size_t uncompressed_data_size = 0;
	int result                    = 0;

	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( compressed_data_size < 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: compressed data size value too small.",
		 function );

		return( -1 );
	}
	if( compressed_data[ 0 ] == 0x18 )
	{
		result = libesedb_compression_xpress_decompress_get_size(
		          compressed_data,
		          compressed_data_size,
		          &uncompressed_data_size,
		          error );
	}
	else
	{
		result = libesedb_compression_7bit_decompress_get_size(
		          compressed_data,
		          compressed_data_size,
		          &uncompressed_data_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable retrieve uncompressed data size.",
		 function );

		goto on_error;
	}
	uncompressed_data = (uint8_t *) memory_allocate(
					 sizeof( uint8_t ) * uncompressed_data_size );

	if( uncompressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create uncompressed data.",
		 function );

		goto on_error;
	}
	if( compressed_data[ 0 ] == 0x18 )
	{
		result = libesedb_compression_xpress_decompress(
		          compressed_data,
		          compressed_data_size,
		          uncompressed_data,
		          uncompressed_data_size,
		          error );
	}
	else
	{
		result = libesedb_compression_7bit_decompress(
		          compressed_data,
		          compressed_data_size,
		          uncompressed_data,
		          uncompressed_data_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
		 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
		 "%s: unable decompressed data.",
		 function );

		goto on_error;
	}
	result = 0;

	if( ( ( uncompressed_data_size % 2 ) == 0 )
	 && ( ( compressed_data[ 0 ] == 0x18 )
	  ||  ( ( compressed_data[ 0 ] & 0x10 ) == 0 ) ) )
	{
		result = libuna_utf8_string_size_from_utf16_stream(
			  uncompressed_data,
			  uncompressed_data_size,
			  LIBUNA_ENDIAN_LITTLE,
			  utf8_string_size,
			  error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine UTF-8 string size of UTF-16 stream.",
			 function );

#if defined( HAVE_DEBUG_OUTPUT )
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
#endif
			libcerror_error_free(
			 error );
		}
	}
	if( result != 1 )
	{
		result = libuna_utf8_string_size_from_utf8_stream(
			  uncompressed_data,
			  uncompressed_data_size,
			  utf8_string_size,
			  error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine UTF-8 string size of UTF-8 stream.",
			 function );

			goto on_error;
		}
	}
	memory_free(
	 uncompressed_data );

	uncompressed_data = NULL;

	return( 1 );

on_error:
	if( uncompressed_data != NULL )
	{
		memory_free(
		 uncompressed_data );
	}
	return( -1 );
}

/* Copies compressed-data to an UTF-8 string
 * Returns 1 on success or -1 on error
 */
int libesedb_compression_copy_to_utf8_string(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	uint8_t *uncompressed_data    = NULL;
	static char *function         = "libesedb_compression_copy_to_utf8_string";
	size_t uncompressed_data_size = 0;
	int result                    = 0;

	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( compressed_data_size < 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: compressed data size value too small.",
		 function );

		return( -1 );
	}
	if( compressed_data[ 0 ] == 0x18 )
	{
		result = libesedb_compression_xpress_decompress_get_size(
		          compressed_data,
		          compressed_data_size,
		          &uncompressed_data_size,
		          error );
	}
	else
	{
		result = libesedb_compression_7bit_decompress_get_size(
		          compressed_data,
		          compressed_data_size,
		          &uncompressed_data_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable retrieve uncompressed data size.",
		 function );

		goto on_error;
	}
	uncompressed_data = (uint8_t *) memory_allocate(
					 sizeof( uint8_t ) * uncompressed_data_size );

	if( uncompressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create uncompressed data.",
		 function );

		goto on_error;
	}
	if( compressed_data[ 0 ] == 0x18 )
	{
		result = libesedb_compression_xpress_decompress(
		          compressed_data,
		          compressed_data_size,
		          uncompressed_data,
		          uncompressed_data_size,
		          error );
	}
	else
	{
		result = libesedb_compression_7bit_decompress(
		          compressed_data,
		          compressed_data_size,
		          uncompressed_data,
		          uncompressed_data_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
		 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
		 "%s: unable decompressed data.",
		 function );

		goto on_error;
	}
	result = 0;

	if( ( ( uncompressed_data_size % 2 ) == 0 )
	 && ( ( compressed_data[ 0 ] == 0x18 )
	  ||  ( ( compressed_data[ 0 ] & 0x10 ) == 0 ) ) )
	{
		result = libuna_utf8_string_copy_from_utf16_stream(
			  utf8_string,
			  utf8_string_size,
			  uncompressed_data,
			  uncompressed_data_size,
			  LIBUNA_ENDIAN_LITTLE,
			  error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UTF-16 stream to UTF-8 string.",
			 function );

#if defined( HAVE_DEBUG_OUTPUT )
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
#endif
			libcerror_error_free(
			 error );
		}
	}
	if( result != 1 )
	{
		result = libuna_utf8_string_copy_from_utf8_stream(
			  utf8_string,
			  utf8_string_size,
			  uncompressed_data,
			  uncompressed_data_size,
			  error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UTF-8 stream to UTF-8 string.",
			 function );

			return( -1 );
		}
	}
	memory_free(
	 uncompressed_data );

	uncompressed_data = NULL;

	return( 1 );

on_error:
	if( uncompressed_data != NULL )
	{
		memory_free(
		 uncompressed_data );
	}
	return( -1 );
}

/* Retrieves the UTF-16 string size of compressed-data
 * Returns 1 on success or -1 on error
 */
int libesedb_compression_get_utf16_string_size(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	uint8_t *uncompressed_data    = NULL;
	static char *function         = "libesedb_compression_get_utf16_string_size";
	size_t uncompressed_data_size = 0;
	int result                    = 0;

	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( compressed_data_size < 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: compressed data size value too small.",
		 function );

		return( -1 );
	}
	if( compressed_data[ 0 ] == 0x18 )
	{
		result = libesedb_compression_xpress_decompress_get_size(
		          compressed_data,
		          compressed_data_size,
		          &uncompressed_data_size,
		          error );
	}
	else
	{
		result = libesedb_compression_7bit_decompress_get_size(
		          compressed_data,
		          compressed_data_size,
		          &uncompressed_data_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable retrieve uncompressed data size.",
		 function );

		goto on_error;
	}
	uncompressed_data = (uint8_t *) memory_allocate(
					 sizeof( uint8_t ) * uncompressed_data_size );

	if( uncompressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create uncompressed data.",
		 function );

		goto on_error;
	}
	if( compressed_data[ 0 ] == 0x18 )
	{
		result = libesedb_compression_xpress_decompress(
		          compressed_data,
		          compressed_data_size,
		          uncompressed_data,
		          uncompressed_data_size,
		          error );
	}
	else
	{
		result = libesedb_compression_7bit_decompress(
		          compressed_data,
		          compressed_data_size,
		          uncompressed_data,
		          uncompressed_data_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
		 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
		 "%s: unable decompressed data.",
		 function );

		goto on_error;
	}
	result = 0;

	if( ( ( uncompressed_data_size % 2 ) == 0 )
	 && ( ( compressed_data[ 0 ] == 0x18 )
	  ||  ( ( compressed_data[ 0 ] & 0x10 ) == 0 ) ) )
	{
		result = libuna_utf16_string_size_from_utf16_stream(
			  uncompressed_data,
			  uncompressed_data_size,
			  LIBUNA_ENDIAN_LITTLE,
			  utf16_string_size,
			  error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine UTF-16 string size of UTF-16 stream.",
			 function );

#if defined( HAVE_DEBUG_OUTPUT )
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
#endif
			libcerror_error_free(
			 error );
		}
	}
	if( result != 1 )
	{
		result = libuna_utf16_string_size_from_utf8_stream(
			  uncompressed_data,
			  uncompressed_data_size,
			  utf16_string_size,
			  error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine UTF-16 string size of UTF-8 stream.",
			 function );

			goto on_error;
		}
	}
	memory_free(
	 uncompressed_data );

	uncompressed_data = NULL;

	return( 1 );

on_error:
	if( uncompressed_data != NULL )
	{
		memory_free(
		 uncompressed_data );
	}
	return( -1 );
}

/* Copies compressed-data to an UTF-16 string
 * Returns 1 on success or -1 on error
 */
int libesedb_compression_copy_to_utf16_string(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	uint8_t *uncompressed_data    = NULL;
	static char *function         = "libesedb_compression_copy_to_utf16_string";
	size_t uncompressed_data_size = 0;
	int result                    = 0;

	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( compressed_data_size < 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: compressed data size value too small.",
		 function );

		return( -1 );
	}
	if( compressed_data[ 0 ] == 0x18 )
	{
		result = libesedb_compression_xpress_decompress_get_size(
		          compressed_data,
		          compressed_data_size,
		          &uncompressed_data_size,
		          error );
	}
	else
	{
		result = libesedb_compression_7bit_decompress_get_size(
		          compressed_data,
		          compressed_data_size,
		          &uncompressed_data_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable retrieve uncompressed data size.",
		 function );

		goto on_error;
	}
	uncompressed_data = (uint8_t *) memory_allocate(
					 sizeof( uint8_t ) * uncompressed_data_size );

	if( uncompressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create uncompressed data.",
		 function );

		goto on_error;
	}
	if( compressed_data[ 0 ] == 0x18 )
	{
		result = libesedb_compression_xpress_decompress(
		          compressed_data,
		          compressed_data_size,
		          uncompressed_data,
		          uncompressed_data_size,
		          error );
	}
	else
	{
		result = libesedb_compression_7bit_decompress(
		          compressed_data,
		          compressed_data_size,
		          uncompressed_data,
		          uncompressed_data_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
		 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
		 "%s: unable decompressed data.",
		 function );

		goto on_error;
	}
	result = 0;

	if( ( ( uncompressed_data_size % 2 ) == 0 )
	 && ( ( compressed_data[ 0 ] == 0x18 )
	  ||  ( ( compressed_data[ 0 ] & 0x10 ) == 0 ) ) )
	{
		result = libuna_utf16_string_copy_from_utf16_stream(
			  utf16_string,
			  utf16_string_size,
			  uncompressed_data,
			  uncompressed_data_size,
			  LIBUNA_ENDIAN_LITTLE,
			  error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UTF-16 stream to UTF-16 string.",
			 function );

#if defined( HAVE_DEBUG_OUTPUT )
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
#endif
			libcerror_error_free(
			 error );
		}
	}
	if( result != 1 )
	{
		result = libuna_utf16_string_copy_from_utf8_stream(
			  utf16_string,
			  utf16_string_size,
			  uncompressed_data,
			  uncompressed_data_size,
			  error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UTF-16 stream to UTF-8 string.",
			 function );

			return( -1 );
		}
	}
	memory_free(
	 uncompressed_data );

	uncompressed_data = NULL;

	return( 1 );

on_error:
	if( uncompressed_data != NULL )
	{
		memory_free(
		 uncompressed_data );
	}
	return( -1 );
}

