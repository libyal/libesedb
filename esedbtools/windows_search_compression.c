/*
 * Windows Search database compression functions
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

#include "esedbtools_libcerror.h"
#include "esedbtools_libcnotify.h"
#include "windows_search_compression.h"

/* Define this if the debug output is not verbose enough
#define HAVE_EXTRA_DEBUG_OUTPUT
*/

/* Determines the uncompressed size of a run-length compressed UTF-16 string
 * Returns 1 on success or -1 on error
 */
int windows_search_utf16_run_length_compression_get_size(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     size_t *uncompressed_data_size,
     libcerror_error_t **error )
{
	static char *function              = "windows_search_utf16_run_length_compression_get_size";
	size_t compressed_data_offset      = 0;
	size_t safe_uncompressed_data_size = 0;
	uint8_t compression_size           = 0;

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
	if( ( compressed_data_size < 1 )
	 || ( compressed_data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid compressed data size value out of bounds.",
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
	*uncompressed_data_size = 0;

	while( compressed_data_offset < compressed_data_size )
	{
		if( compressed_data_offset >= compressed_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: compressed data size value too small.",
			 function );

			return( -1 );
		}
		compression_size = compressed_data[ compressed_data_offset++ ];

		/* Check if the last byte in the compressed string was the compression size
		 * or the run-length byte value
		 */
		if( compressed_data_offset >= ( compressed_data_size - 1 ) )
		{
			break;
		}
		/* Check if the compressed string was cut-short at the end
		 */
		if( ( compressed_data_offset + 1 + compression_size ) > compressed_data_size )
		{
#if defined( HAVE_DEBUG_OUTPUT )
fprintf( stderr, "MARKER: %zd, %d, %zd, %zd\n",
 compressed_data_offset,
 compression_size, compressed_data_size,
 compressed_data_size - compressed_data_offset - 1 );
#endif
			compression_size = (uint8_t) ( compressed_data_size - compressed_data_offset - 1 );
		}
		safe_uncompressed_data_size += compression_size * 2;
		compressed_data_offset      += compression_size + 1;
	}
	if( compressed_data_offset > compressed_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: compressed data size value too small.",
		 function );

		return( -1 );
	}
	*uncompressed_data_size = safe_uncompressed_data_size;

	return( 1 );
}

/* Decompresses a run-length compressed UTF-16 string
 * Returns 1 on success or -1 on error
 */
int windows_search_utf16_run_length_compression_decompress(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     libcerror_error_t **error )
{
	static char *function           = "windows_search_utf16_run_length_compression_decompress";
	size_t compressed_data_offset   = 0;
	size_t uncompressed_data_offset = 0;
	uint8_t compression_byte        = 0;
	uint8_t compression_size        = 0;

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
	if( ( compressed_data_size < 1 )
	 || ( compressed_data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid compressed data size value out of bounds.",
		 function );

		return( -1 );
	}
	while( compressed_data_offset < compressed_data_size )
	{
		if( compressed_data_offset >= compressed_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: compressed data size value too small.",
			 function );

			return( -1 );
		}
		compression_size = compressed_data[ compressed_data_offset++ ];

		/* Check if the last byte in the compressed string was the compression size
		 * or the run-length byte value
		 */
		if( compressed_data_offset >= ( compressed_data_size - 1 ) )
		{
			break;
		}
		/* Check if the compressed string was cut-short at the end
		 */
		if( ( compressed_data_offset + 1 + compression_size ) > compressed_data_size )
		{
			compression_size = (uint8_t) ( compressed_data_size - compressed_data_offset - 1 );
		}
		if( compressed_data_offset >= compressed_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: compressed data size value too small.",
			 function );

			return( -1 );
		}
		compression_byte = compressed_data[ compressed_data_offset++ ];

		while( compression_size > 0 )
		{
			if( compressed_data_offset >= compressed_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: compressed data size value too small.",
				 function );

				return( -1 );
			}
			if( uncompressed_data_offset >= ( uncompressed_data_size - 1 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: uncompressed data size value too small.",
				 function );

				return( -1 );
			}
			uncompressed_data[ uncompressed_data_offset++ ] = compressed_data[ compressed_data_offset++ ];
			uncompressed_data[ uncompressed_data_offset++ ] = compression_byte;

			compression_size--;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( HAVE_EXTRA_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: uncompressed data:\n",
		 function );
		libcnotify_print_data(
		 uncompressed_data,
		 uncompressed_data_offset,
		 0 );
	}
#endif
	return( 1 );
}

