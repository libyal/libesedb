/*
 * Compression functions
 *
 * Copyright (C) 2009-2023, Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libesedb_compression.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_libfwnt.h"
#include "libesedb_libuna.h"

/* Retrieves the uncompressed size of the 7-bit compressed data
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

/* Decompresses 7-bit compressed data
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

/* Retrieves the uncompressed size of LZXPRESS compressed data
 * Returns 1 on success or -1 on error
 */
int libesedb_compression_lzxpress_decompress_get_size(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     size_t *uncompressed_data_size,
     libcerror_error_t **error )
{
	static char *function = "libesedb_compression_lzxpress_decompress_get_size";

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

/* Decompresses LZXPRESS compressed data
 * Returns 1 on success or -1 on error
 */
int libesedb_compression_lzxpress_decompress(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     libcerror_error_t **error )
{
	static char *function = "libesedb_compression_lzxpress_decompress";

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
	if( libfwnt_lzxpress_decompress(
	     &( compressed_data[ 3 ] ),
	     compressed_data_size - 3,
	     uncompressed_data,
	     &uncompressed_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
		 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
		 "%s: unable to decompress LZXPRESS compressed data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the uncompressed size of the compressed data
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
		result = libesedb_compression_lzxpress_decompress_get_size(
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

/* Decompresses compressed data
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
		result = libesedb_compression_lzxpress_decompress(
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

/* Retrieves the UTF-8 string size of compressed data
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
		result = libesedb_compression_lzxpress_decompress_get_size(
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
	if( ( uncompressed_data_size == 0 )
	 || ( uncompressed_data_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid uncompressed data size value out of bounds.",
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
		result = libesedb_compression_lzxpress_decompress(
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

/* Copies compressed data to an UTF-8 string
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
		result = libesedb_compression_lzxpress_decompress_get_size(
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
	if( ( uncompressed_data_size == 0 )
	 || ( uncompressed_data_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid uncompressed data size value out of bounds.",
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
		result = libesedb_compression_lzxpress_decompress(
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

/* Retrieves the UTF-16 string size of compressed data
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
		result = libesedb_compression_lzxpress_decompress_get_size(
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
	if( ( uncompressed_data_size == 0 )
	 || ( uncompressed_data_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid uncompressed data size value out of bounds.",
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
		result = libesedb_compression_lzxpress_decompress(
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

/* Copies compressed data to an UTF-16 string
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
		result = libesedb_compression_lzxpress_decompress_get_size(
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
	if( ( uncompressed_data_size == 0 )
	 || ( uncompressed_data_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid uncompressed data size value out of bounds.",
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
		result = libesedb_compression_lzxpress_decompress(
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

