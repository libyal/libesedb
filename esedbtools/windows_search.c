/*
 * Windows Search database export functions
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
#include <file_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "esedbtools_libcerror.h"
#include "esedbtools_libcnotify.h"
#include "esedbtools_libesedb.h"
#include "esedbtools_libfdatetime.h"
#include "esedbtools_libfwnt.h"
#include "esedbtools_libuna.h"
#include "export.h"
#include "export_handle.h"
#include "windows_search.h"
#include "windows_search_compression.h"

/* Define this if the debug output is not verbose enough
#define HAVE_EXTRA_DEBUG_OUTPUT
*/

enum WINDOWS_SEARCH_KNOWN_COLUMN_TYPES
{
	WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_UNDEFINED,
	WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_INTEGER_32BIT,
	WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_INTEGER_64BIT,
	WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME,
	WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED,
	WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_UTF16_LITTLE_ENDIAN,
};

enum WINDOWS_SEARCH_FORMATS
{
	WINDOWS_SEARCH_FORMAT_DECIMAL,
	WINDOWS_SEARCH_FORMAT_HEXADECIMAL,
};

/* Decode data using Windows Search encoding
 * Returns 1 on success or -1 on error
 */
int windows_search_decode(
     uint8_t *data,
     size_t data_size,
     uint8_t *encoded_data,
     size_t encoded_data_size,
     libcerror_error_t **error )
{
	static char *function        = "windows_search_decode";
	size_t data_iterator         = 0;
	size_t encoded_data_iterator = 0;
	uint32_t bitmask32           = 0;
	uint8_t bitmask              = 0;

	if( encoded_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encoded data.",
		 function );

		return( -1 );
	}
	if( encoded_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid encoded data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( data_size < encoded_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: data size value too small.",
		 function );

		return( -1 );
	}
	bitmask32 = 0x05000113 ^ (uint32_t) encoded_data_size;

	for( encoded_data_iterator = 0;
	     encoded_data_iterator < encoded_data_size;
	     encoded_data_iterator++ )
	{
		switch( encoded_data_iterator & 0x03 )
		{
			case 3:
				bitmask = (uint8_t) ( ( bitmask32 >> 24 ) & 0xff );
				break;
			case 2:
				bitmask = (uint8_t) ( ( bitmask32 >> 16 ) & 0xff );
				break;
			case 1:
				bitmask = (uint8_t) ( ( bitmask32 >> 8 ) & 0xff );
				break;
			default:
				bitmask = (uint8_t) ( bitmask32 & 0xff );
				break;
		}
		bitmask ^= encoded_data_iterator;

		data[ data_iterator++ ] = encoded_data[ encoded_data_iterator ]
		                        ^ bitmask;
	}
	return( 1 );
}

/* Exports a compressed string
 * Returns 1 if successful or -1 on error
 */
int windows_search_export_compressed_string_value(
     uint8_t *value_data,
     size_t value_data_size,
     int ascii_codepage,
     FILE *record_file_stream,
     libcerror_error_t **error )
{
	system_character_t *value_string    = NULL;
	uint8_t *decoded_value_data         = NULL;
	uint8_t *decompressed_value_data    = NULL;
	uint8_t *narrow_value_string        = NULL;
	uint8_t *value_utf16_stream         = NULL;
	static char *function               = "windows_search_export_compressed_string_value";
	size_t decoded_value_data_size      = 0;
	size_t decompressed_value_data_size = 0;
	size_t uncompressed_value_data_size = 0;
	size_t value_string_size            = 0;
	size_t value_utf16_stream_size      = 0;
	uint8_t compression_type            = 0;
	int result                          = 0;

	if( value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data.",
		 function );

		return( -1 );
	}
	if( ( value_data_size == 0 )
	 || ( value_data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid value data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( record_file_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record file stream.",
		 function );

		return( -1 );
	}
	decoded_value_data_size = value_data_size;

	decoded_value_data = (uint8_t *) memory_allocate(
					  sizeof( uint8_t ) * decoded_value_data_size );

	if( decoded_value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create decoded value data.",
		 function );

		return( -1 );
	}
	if( windows_search_decode(
	     decoded_value_data,
	     decoded_value_data_size,
	     value_data,
	     value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to decode value data.",
		 function );

		memory_free(
		 decoded_value_data );

		return( -1 );
	}
/* TODO test purposes
#if defined( HAVE_DEBUG_OUTPUT ) && defined( HAVE_EXTRA_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: value data:\n",
		 function );
		libcnotify_print_data(
		 value_data,
		 value_data_size,
		 0 );
	}
#endif
*/
#if defined( HAVE_DEBUG_OUTPUT ) && defined( HAVE_EXTRA_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: decoded data:\n",
		 function );
		libcnotify_print_data(
		 decoded_value_data,
		 decoded_value_data_size,
		 0 );
	}
#endif
	compression_type = decoded_value_data[ 0 ];

	/* Byte-index compressed data
	 */
	if( ( compression_type & 0x02 ) != 0 )
	{
		if( decoded_value_data_size < 3 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: invalid value data size value too small.",
			 function );

			return( -1 );
		}
		/* Bytes 1 - 2 contain the uncompressed data size
		 */
		byte_stream_copy_to_uint16_little_endian(
		 &( decoded_value_data[ 1 ] ),
		 decompressed_value_data_size );

		/* Add room for a leading byte for chained decompression
		 */
		decompressed_value_data_size += 1;

		decompressed_value_data = (uint8_t *) memory_allocate(
		                                       sizeof( uint8_t ) * decompressed_value_data_size );

		if( decompressed_value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create decompressed value data.",
			 function );

			memory_free(
			 decoded_value_data );

			return( -1 );
		}
		/* Add the first byte of the decoded data - 2 to have the
		 * decompressed data look like decoded data for chained decompression
		 */
		decompressed_value_data[ 0 ] = decoded_value_data[ 0 ] - 2;
		uncompressed_value_data_size = decompressed_value_data_size - 1;

		result = libfwnt_lzxpress_huffman_decompress(
		          &( decoded_value_data[ 3 ] ),
		          decoded_value_data_size - 3,
		          &( decompressed_value_data[ 1 ] ),
		          &uncompressed_value_data_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to decompress byte-index compressed data.",
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

			memory_free(
			 decompressed_value_data );
		}
		else
		{
#if defined( HAVE_DEBUG_OUTPUT ) && defined( HAVE_EXTRA_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: decompressed data:\n",
				 function );
				libcnotify_print_data(
				 decompressed_value_data,
				 decompressed_value_data_size,
				 0 );
			}
#endif
			memory_free(
			 decoded_value_data );

			decoded_value_data      = decompressed_value_data;
			decoded_value_data_size = decompressed_value_data_size;

			compression_type &= ~( 0x02 );
		}
	}
	/* Run-length compressed UTF-16 little-endian string
	 */
	if( compression_type == 0 )
	{
		if( windows_search_utf16_run_length_compression_get_size(
		     &( decoded_value_data[ 1 ] ),
		     decoded_value_data_size - 1,
		     &value_utf16_stream_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine uncompressed size of UTF-16 run-length compressed data.",
			 function );

			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			memory_free(
			 decoded_value_data );
		}
		if( value_utf16_stream_size > 0 )
		{
			value_utf16_stream = (uint8_t *) memory_allocate(
							  sizeof( uint8_t ) * value_utf16_stream_size );

			if( value_utf16_stream == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create value UTF-16 stream.",
				 function );

				memory_free(
				 decoded_value_data );

				return( -1 );
			}
			if( windows_search_utf16_run_length_compression_decompress(
			     &( decoded_value_data[ 1 ] ),
			     decoded_value_data_size - 1,
			     value_utf16_stream,
			     value_utf16_stream_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to decompress UTF-16 run-length compressed data.",
				 function );

				memory_free(
				 value_utf16_stream );
				memory_free(
				 decoded_value_data );

				return( -1 );
			}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( HAVE_EXTRA_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: decompressed data:\n",
				 function );
				libcnotify_print_data(
				 value_utf16_stream,
				 value_utf16_stream_size,
				 0 );
			}
#endif
			/* Sometimes the UTF-16 stream is cut-off in the surrogate high range
			 * The last 2 bytes are ignored otherwise libuna will not convert
			 * the stream to a string
			 */
			if( ( ( value_utf16_stream[ value_utf16_stream_size - 1 ] ) >= 0xd8 )
			 && ( ( value_utf16_stream[ value_utf16_stream_size - 1 ] ) <= 0xdb ) )
			{
				value_utf16_stream_size -= 2;
			}
			memory_free(
			 decoded_value_data );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libuna_utf16_string_size_from_utf16_stream(
				  value_utf16_stream,
				  value_utf16_stream_size,
				  LIBUNA_ENDIAN_LITTLE,
				  &value_string_size,
				  error );
#else
			result = libuna_utf8_string_size_from_utf16_stream(
				  value_utf16_stream,
				  value_utf16_stream_size,
				  LIBUNA_ENDIAN_LITTLE,
				  &value_string_size,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine size of value UTF-16 stream.",
				 function );

				memory_free(
				 value_utf16_stream );

				return( -1 );
			}
			value_string = system_string_allocate(
					value_string_size );

			if( value_string == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create value string.",
				 function );

				memory_free(
				 value_utf16_stream );

				return( -1 );
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libuna_utf16_string_copy_from_utf16_stream(
				  (uint16_t *) value_string,
				  value_string_size,
				  value_utf16_stream,
				  value_utf16_stream_size,
				  LIBUNA_ENDIAN_LITTLE,
				  error );
#else
			result = libuna_utf8_string_copy_from_utf16_stream(
				  (uint8_t *) value_string,
				  value_string_size,
				  value_utf16_stream,
				  value_utf16_stream_size,
				  LIBUNA_ENDIAN_LITTLE,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value string.",
				 function );

				memory_free(
				 value_string );
				memory_free(
				 value_utf16_stream );

				return( -1 );
			}
			memory_free(
			 value_utf16_stream );

			export_text(
			 value_string,
			 value_string_size,
			 record_file_stream );

			memory_free(
			 value_string );
		}
	}
	/* 8-bit compressed UTF-16 little-endian string
	 */
	else if( compression_type == 1 )
	{
		if( libuna_utf8_string_size_from_byte_stream(
		     &( decoded_value_data[ 1 ] ),
		     decoded_value_data_size - 1,
		     ascii_codepage,
		     &value_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine size of value string.",
			 function );

			memory_free(
			 decoded_value_data );

			return( -1 );
		}
		narrow_value_string = (uint8_t *) memory_allocate(
		                                   sizeof( uint8_t ) * value_string_size );

		if( narrow_value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value string.",
			 function );

			memory_free(
			 decoded_value_data );

			return( -1 );
		}
		if( libuna_utf8_string_copy_from_byte_stream(
		     narrow_value_string,
		     value_string_size,
		     &( decoded_value_data[ 1 ] ),
		     decoded_value_data_size - 1,
		     ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value string.",
			 function );

			memory_free(
			 narrow_value_string );
			memory_free(
			 decoded_value_data );

			return( -1 );
		}
		memory_free(
		 decoded_value_data );

		export_narrow_text(
		 (char *) narrow_value_string,
		 value_string_size,
		 record_file_stream );

		memory_free(
		 narrow_value_string );
	}
	/* uncompressed data
	 */
	else if( compression_type == 4 )
	{
		export_narrow_text(
		 (char *) &( decoded_value_data[ 1 ] ),
		 decoded_value_data_size - 1,
		 record_file_stream );

		memory_free(
		 decoded_value_data );
	}
	else
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "unsupported compression type: 0x%02" PRIx8 "\n",
			 compression_type );

#if defined( HAVE_DEBUG_OUTPUT )
			libcnotify_print_data(
			 value_data,
			 value_data_size,
			 0 );
#endif
		}
		memory_free(
		 decoded_value_data );
	}
	return( 1 );
}

/* Exports a 32-bit value in a binary data table record value
 * Returns 1 if successful or -1 on error
 */
int windows_search_export_record_value_32bit(
     libesedb_record_t *record,
     int record_value_entry,
     uint8_t byte_order,
     FILE *record_file_stream,
     libcerror_error_t **error )
{
	uint8_t *value_data      = NULL;
	static char *function    = "windows_search_export_record_value_32bit";
	size_t value_data_size   = 0;
	uint32_t column_type     = 0;
	uint32_t value_32bit     = 0;
	uint8_t value_data_flags = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( ( byte_order != _BYTE_STREAM_ENDIAN_BIG )
	 && ( byte_order != _BYTE_STREAM_ENDIAN_LITTLE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported byte order: 0x%02" PRIx8 "",
		 function,
		 byte_order );

		return( -1 );
	}
	if( record_file_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record file stream.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_column_type(
	     record,
	     record_value_entry,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column type of value: %d.",
		 function,
		 record_value_entry );

		goto on_error;
	}
	if( column_type != LIBESEDB_COLUMN_TYPE_BINARY_DATA )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 "",
		 function,
		 column_type );

		goto on_error;
	}
	if( libesedb_record_get_value_data_size(
	     record,
	     record_value_entry,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d data size.",
		 function,
		 record_value_entry );

		goto on_error;
	}
	if( value_data_size > 0 )
	{
		value_data = (uint8_t *) memory_allocate(
		                          sizeof( uint8_t ) * value_data_size );

		if( value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value data.",
			 function );

			goto on_error;
		}
		if( libesedb_record_get_value_data(
		     record,
		     record_value_entry,
		     value_data,
		     value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d data.",
			 function,
			 record_value_entry );

			goto on_error;
		}
	}
	if( libesedb_record_get_value_data_flags(
	     record,
	     record_value_entry,
	     &value_data_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d data flags.",
		 function,
		 record_value_entry );

		goto on_error;
	}
	if( ( value_data_flags & ~( LIBESEDB_VALUE_FLAG_VARIABLE_SIZE ) ) == 0 )
	{
		if( value_data != NULL )
		{
			if( value_data_size != 4 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported value data size: %" PRIzd "",
				 function,
				 value_data_size );

				goto on_error;
			}
			if( byte_order == _BYTE_STREAM_ENDIAN_BIG )
			{
				byte_stream_copy_to_uint32_big_endian(
				 value_data,
				 value_32bit );
			}
			else
			{
				byte_stream_copy_to_uint32_little_endian(
				 value_data,
				 value_32bit );
			}
			fprintf(
			 record_file_stream,
			 "%" PRIu32 "",
			 value_32bit );
		}
	}
	else
	{
		export_binary_data(
		 value_data,
		 value_data_size,
		 record_file_stream );
	}
	memory_free(
	 value_data );

	return( 1 );

on_error:
	if( value_data != NULL )
	{
		memory_free(
		 value_data );
	}
	return( -1 );
}

/* Exports a 64-bit value in a binary data table record value
 * Returns 1 if successful or -1 on error
 */
int windows_search_export_record_value_64bit(
     libesedb_record_t *record,
     int record_value_entry,
     uint8_t byte_order,
     uint8_t format,
     FILE *record_file_stream,
     libcerror_error_t **error )
{
	uint8_t *value_data      = NULL;
	static char *function    = "windows_search_export_record_value_64bit";
	size_t value_data_size   = 0;
	uint64_t value_64bit     = 0;
	uint32_t column_type     = 0;
	uint8_t value_data_flags = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( ( byte_order != _BYTE_STREAM_ENDIAN_BIG )
	 && ( byte_order != _BYTE_STREAM_ENDIAN_LITTLE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported byte order: 0x%02" PRIx8 "",
		 function,
		 byte_order );

		return( -1 );
	}
	if( ( format != WINDOWS_SEARCH_FORMAT_DECIMAL )
	 && ( format != WINDOWS_SEARCH_FORMAT_HEXADECIMAL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format: 0x%02" PRIx8 "",
		 function,
		 format );

		return( -1 );
	}
	if( record_file_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record file stream.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_column_type(
	     record,
	     record_value_entry,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column type of value: %d.",
		 function,
		 record_value_entry );

		goto on_error;
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_CURRENCY )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_BINARY_DATA )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 "",
		 function,
		 column_type );

		goto on_error;
	}
	if( libesedb_record_get_value_data_size(
	     record,
	     record_value_entry,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d data size.",
		 function,
		 record_value_entry );

		goto on_error;
	}
	if( value_data_size > 0 )
	{
		value_data = (uint8_t *) memory_allocate(
		                          sizeof( uint8_t ) * value_data_size );

		if( value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value data.",
			 function );

			goto on_error;
		}
		if( libesedb_record_get_value_data(
		     record,
		     record_value_entry,
		     value_data,
		     value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d data.",
			 function,
			 record_value_entry );

			goto on_error;
		}
	}
	if( libesedb_record_get_value_data_flags(
	     record,
	     record_value_entry,
	     &value_data_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d data flags.",
		 function,
		 record_value_entry );

		goto on_error;
	}
	if( ( value_data_flags & ~( LIBESEDB_VALUE_FLAG_VARIABLE_SIZE ) ) == 0 )
	{
		if( value_data != NULL )
		{
			if( value_data_size != 8 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported value data size: %" PRIzd "",
				 function,
				 value_data_size );

				goto on_error;
			}
			if( memory_compare(
			     value_data,
			     "********",
			     8 ) == 0 )
			{
				fprintf(
				 record_file_stream,
				 "********" );
			}
			else
			{
				if( byte_order == _BYTE_STREAM_ENDIAN_BIG )
				{
					byte_stream_copy_to_uint64_big_endian(
					 value_data,
					 value_64bit );
				}
				else
				{
					byte_stream_copy_to_uint64_little_endian(
					 value_data,
					 value_64bit );
				}
				if( format == WINDOWS_SEARCH_FORMAT_DECIMAL )
				{
					fprintf(
					 record_file_stream,
					 "%" PRIu64 "",
					 value_64bit );
				}
				else if( format == WINDOWS_SEARCH_FORMAT_HEXADECIMAL )
				{
					export_binary_data(
					 value_data,
					 value_data_size,
					 record_file_stream );
				}
			}
		}
	}
	else
	{
		export_binary_data(
		 value_data,
		 value_data_size,
		 record_file_stream );
	}
	memory_free(
	 value_data );

	return( 1 );

on_error:
	if( value_data != NULL )
	{
		memory_free(
		 value_data );
	}
	return( -1 );
}

/* Exports a filetime value in a binary data table record value
 * Returns 1 if successful or -1 on error
 */
int windows_search_export_record_value_filetime(
     libesedb_record_t *record,
     int record_value_entry,
     uint8_t byte_order,
     FILE *record_file_stream,
     libcerror_error_t **error )
{
	system_character_t filetime_string[ 32 ];

	libfdatetime_filetime_t *filetime = NULL;
	uint8_t *value_data               = NULL;
	static char *function             = "windows_search_export_record_value_filetime";
	size_t value_data_size            = 0;
	uint32_t column_type              = 0;
	uint8_t value_data_flags          = 0;
	int result                        = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( record_file_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record file stream.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_column_type(
	     record,
	     record_value_entry,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column type of value: %d.",
		 function,
		 record_value_entry );

		goto on_error;
	}
	if( column_type != LIBESEDB_COLUMN_TYPE_BINARY_DATA )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 "",
		 function,
		 column_type );

		goto on_error;
	}
	if( libesedb_record_get_value_data_size(
	     record,
	     record_value_entry,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d data size.",
		 function,
		 record_value_entry );

		goto on_error;
	}
	if( value_data_size > 0 )
	{
		value_data = (uint8_t *) memory_allocate(
		                          sizeof( uint8_t ) * value_data_size );

		if( value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value data.",
			 function );

			goto on_error;
		}
		if( libesedb_record_get_value_data(
		     record,
		     record_value_entry,
		     value_data,
		     value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d data.",
			 function,
			 record_value_entry );

			goto on_error;
		}
	}
	if( libesedb_record_get_value_data_flags(
	     record,
	     record_value_entry,
	     &value_data_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d data flags.",
		 function,
		 record_value_entry );

		goto on_error;
	}
	if( ( value_data_flags & ~( LIBESEDB_VALUE_FLAG_VARIABLE_SIZE ) ) == 0 )
	{
		if( value_data != NULL )
		{
			if( value_data_size != 8 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported value data size: %" PRIzd "",
				 function,
				 value_data_size );

				goto on_error;
			}
			if( libfdatetime_filetime_initialize(
			     &filetime,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create filetime.",
				 function );

				goto on_error;
			}
			if( libfdatetime_filetime_copy_from_byte_stream(
			     filetime,
			     value_data,
			     value_data_size,
			     byte_order,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy byte stream to filetime.",
				 function );

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libfdatetime_filetime_copy_to_utf16_string(
			          filetime,
			          (uint16_t *) filetime_string,
			          32,
			          LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
			          error );
#else
			result = libfdatetime_filetime_copy_to_utf8_string(
			          filetime,
			          (uint8_t *) filetime_string,
			          32,
			          LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy filetime to string.",
				 function );

				goto on_error;
			}
			if( libfdatetime_filetime_free(
			     &filetime,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free filetime.",
				 function );

				goto on_error;
			}
			fprintf(
			 record_file_stream,
			 "%" PRIs_SYSTEM "",
			 filetime_string );
		}
	}
	else
	{
		export_binary_data(
		 value_data,
		 value_data_size,
		 record_file_stream );
	}
	memory_free(
	 value_data );

	return( 1 );

on_error:
	if( value_data != NULL )
	{
		memory_free(
		 value_data );
	}
	if( filetime != NULL )
	{
		libfdatetime_filetime_free(
		 &filetime,
		 NULL );
	}
	return( -1 );
}

/* Exports a compressed string in a binary data table record value
 * Returns 1 if successful or -1 on error
 */
int windows_search_export_record_value_compressed_string(
     libesedb_record_t *record,
     int record_value_entry,
     int ascii_codepage,
     FILE *record_file_stream,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	libesedb_long_value_t *long_value   = NULL;
	libesedb_multi_value_t *multi_value = NULL;
	uint8_t *long_value_data            = NULL;
	uint8_t *multi_value_data           = NULL;
	uint8_t *value_data                 = NULL;
	static char *function               = "windows_search_export_record_value_compressed_string";
	size_t long_value_data_size         = 0;
	size_t multi_value_data_size        = 0;
	size_t value_data_size              = 0;
	uint32_t column_type                = 0;
	uint8_t value_data_flags            = 0;
	int multi_value_iterator            = 0;
	int number_of_multi_values          = 0;
	int result                          = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( record_file_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record file stream.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_column_type(
	     record,
	     record_value_entry,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column type of value: %d.",
		 function,
		 record_value_entry );

		goto on_error;
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_BINARY_DATA )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 "",
		 function,
		 column_type );

		goto on_error;
	}
	if( libesedb_record_get_value_data_size(
	     record,
	     record_value_entry,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d data size.",
		 function,
		 record_value_entry );

		goto on_error;
	}
	if( value_data_size > 0 )
	{
		value_data = (uint8_t *) memory_allocate(
		                          sizeof( uint8_t ) * value_data_size );

		if( value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value data.",
			 function );

			goto on_error;
		}
		if( libesedb_record_get_value_data(
		     record,
		     record_value_entry,
		     value_data,
		     value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d data.",
			 function,
			 record_value_entry );

			goto on_error;
		}
	}
	if( libesedb_record_get_value_data_flags(
	     record,
	     record_value_entry,
	     &value_data_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d data flags.",
		 function,
		 record_value_entry );

		goto on_error;
	}
	if( ( value_data_flags & ~( LIBESEDB_VALUE_FLAG_VARIABLE_SIZE ) ) == 0 )
	{
		if( value_data != NULL )
		{
			if( windows_search_export_compressed_string_value(
			     value_data,
			     value_data_size,
			     ascii_codepage,
			     record_file_stream,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to export compressed string value of record entry: %d.",
				 function,
				 record_value_entry );

				goto on_error;
			}
		}
	}
	else if( ( value_data_flags & LIBESEDB_VALUE_FLAG_LONG_VALUE ) != 0 )
	{
		result = libesedb_record_get_long_value(
		          record,
		          record_value_entry,
		          &long_value,
		          error );

		if( result != 1 )
		{
			log_handle_printf(
			 log_handle,
			 "Unable to retrieve long value of record entry: %d.\n",
			 record_value_entry );

			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to retrieve long value of record entry: %d.\n",
				 function,
				 record_value_entry );
			}
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve long value of record entry: %d.",
			 function,
			 record_value_entry );

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
		else
		{
			if( export_get_long_value_data(
			     long_value,
			     &long_value_data,
			     &long_value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve long value data.",
				 function );

				goto on_error;
			}
			if( long_value_data_size > 0 )
			{
				if( windows_search_export_compressed_string_value(
				     long_value_data,
				     long_value_data_size,
				     ascii_codepage,
				     record_file_stream,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GENERIC,
					 "%s: unable to export compressed string of long value data of record entry: %d.",
					 function,
					 record_value_entry );

					goto on_error;
				}
				memory_free(
				 long_value_data );

				long_value_data = NULL;
			}
			if( libesedb_long_value_free(
			     &long_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free long value.",
				 function );

				goto on_error;
			}
		}
	}
/* TODO handle 0x10 flag */
	else if( ( ( value_data_flags & LIBESEDB_VALUE_FLAG_MULTI_VALUE ) != 0 )
	      && ( ( value_data_flags & 0x10 ) == 0 ) )
	{
		if( libesedb_record_get_multi_value(
		     record,
		     record_value_entry,
		     &multi_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve multi value of record entry: %d.",
			 function,
			 record_value_entry );

			goto on_error;
		}
		if( libesedb_multi_value_get_number_of_values(
		     multi_value,
		     &number_of_multi_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of multi values.",
			 function );

			goto on_error;
		}
		for( multi_value_iterator = 0;
	 	     multi_value_iterator < number_of_multi_values;
		     multi_value_iterator++ )
		{
			if( libesedb_multi_value_get_value_data_size(
			     multi_value,
			     multi_value_iterator,
			     &multi_value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve multi value: %d data size of record entry: %d.",
				 function,
				 multi_value_iterator,
				 record_value_entry );

				goto on_error;
			}
			if( multi_value_data_size > 0 )
			{
				multi_value_data = (uint8_t *) memory_allocate(
				                                sizeof( uint8_t ) * multi_value_data_size );

				if( multi_value_data == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create multi value data.",
					 function );

					goto on_error;
				}
				if( libesedb_multi_value_get_value_data(
				     multi_value,
				     multi_value_iterator,
				     multi_value_data,
				     multi_value_data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve multi value: %d of record entry: %d.",
					 function,
					 multi_value_iterator,
					 record_value_entry );

					goto on_error;
				}
/* TODO what about non string multi values ? */
				if( windows_search_export_compressed_string_value(
				     multi_value_data,
				     multi_value_data_size,
				     ascii_codepage,
				     record_file_stream,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GENERIC,
					 "%s: unable to export compressed string of multi value: %d of record entry: %d.",
					 function,
					 multi_value_iterator,
					 record_value_entry );

					goto on_error;
				}
				memory_free(
				 multi_value_data );

				multi_value_data = NULL;

				if( multi_value_iterator < ( number_of_multi_values - 1 ) )
				{
					fprintf(
					 record_file_stream,
					 "; " );
				}
			}
		}
		if( libesedb_multi_value_free(
		     &multi_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free multi value: %d.",
			 function,
			 multi_value_iterator );

			goto on_error;
		}
	}
	else
	{
		export_binary_data(
		 value_data,
		 value_data_size,
		 record_file_stream );
	}
	if( value_data != NULL )
	{
		memory_free(
		 value_data );
	}
	return( 1 );

on_error:
	if( multi_value_data != NULL )
	{
		memory_free(
		 multi_value_data );
	}
	if( multi_value != NULL )
	{
		libesedb_multi_value_free(
		 &multi_value,
		 NULL );
	}
	if( long_value_data != NULL )
	{
		memory_free(
		 long_value_data );
	}
	if( long_value != NULL )
	{
		libesedb_long_value_free(
		 &long_value,
		 NULL );
	}
	if( value_data != NULL )
	{
		memory_free(
		 value_data );
	}
	return( -1 );
}

/* Exports an UTF-16 string in a binary data table record value
 * Returns 1 if successful or -1 on error
 */
int windows_search_export_record_value_utf16_string(
     libesedb_record_t *record,
     int record_value_entry,
     uint8_t byte_order,
     FILE *record_file_stream,
     libcerror_error_t **error )
{
	system_character_t *value_string = NULL;
	uint8_t *value_data              = NULL;
	static char *function            = "windows_search_export_record_value_utf16_string";
	size_t value_data_size           = 0;
	size_t value_string_size         = 0;
	uint32_t column_type             = 0;
	uint8_t value_data_flags         = 0;
	int result                       = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( record_file_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record file stream.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_column_type(
	     record,
	     record_value_entry,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column type of value: %d.",
		 function,
		 record_value_entry );

		goto on_error;
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_BINARY_DATA )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 "",
		 function,
		 column_type );

		goto on_error;
	}
	if( libesedb_record_get_value_data_size(
	     record,
	     record_value_entry,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d data size.",
		 function,
		 record_value_entry );

		goto on_error;
	}
	if( value_data_size > 0 )
	{
		value_data = (uint8_t *) memory_allocate(
		                          sizeof( uint8_t ) * value_data_size );

		if( value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value data.",
			 function );

			goto on_error;
		}
		if( libesedb_record_get_value_data(
		     record,
		     record_value_entry,
		     value_data,
		     value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d data.",
			 function,
			 record_value_entry );

			goto on_error;
		}
	}
	if( libesedb_record_get_value_data_flags(
	     record,
	     record_value_entry,
	     &value_data_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d data flags.",
		 function,
		 record_value_entry );

		goto on_error;
	}
	if( ( value_data_flags & ~( LIBESEDB_VALUE_FLAG_VARIABLE_SIZE ) ) == 0 )
	{
		if( value_data != NULL )
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libuna_utf16_string_size_from_utf16_stream(
			          value_data,
			          value_data_size,
			          byte_order,
			          &value_string_size,
			          error );
#else
			result = libuna_utf8_string_size_from_utf16_stream(
			          value_data,
			          value_data_size,
			          byte_order,
			          &value_string_size,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine size of value string: %d.",
				 function,
				 record_value_entry );

				goto on_error;
			}
			value_string = system_string_allocate(
			                value_string_size );

			if( value_string == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create value string.",
				 function );

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libuna_utf16_string_copy_from_utf16_stream(
			          (uint16_t *) value_string,
			          value_string_size,
			          value_data,
			          value_data_size,
			          byte_order,
			          error );
#else
			result = libuna_utf8_string_copy_from_utf16_stream(
			          (uint8_t *) value_string,
			          value_string_size,
			          value_data,
			          value_data_size,
			          byte_order,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value string: %d.",
				 function,
				 record_value_entry );

				memory_free(
				 value_string );

				goto on_error;
			}
			export_text(
			 value_string,
			 value_string_size,
			 record_file_stream );

			memory_free(
			 value_string );
		}
	}
	else
	{
		export_binary_data(
		 value_data,
		 value_data_size,
		 record_file_stream );
	}
	memory_free(
	 value_data );

	return( 1 );

on_error:
	if( value_data != NULL )
	{
		memory_free(
		 value_data );
	}
	return( -1 );
}

/* Exports the values in a SystemIndex_0A table record
 * Returns 1 if successful or -1 on error
 */
int windows_search_export_record_systemindex_0a(
     libesedb_record_t *record,
     int ascii_codepage,
     FILE *record_file_stream,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	system_character_t column_name[ 256 ];

	static char *function   = "windows_search_export_record_systemindex_0a";
	size_t column_name_size = 0;
	uint32_t column_type    = 0;
	uint8_t byte_order      = _BYTE_STREAM_ENDIAN_BIG;
	uint8_t format          = 0;
	int known_column_type   = 0;
	int number_of_values    = 0;
	int result              = 0;
	int value_iterator      = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( record_file_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record file stream.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_number_of_values(
	     record,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		return( -1 );
	}
	for( value_iterator = 0;
	     value_iterator < number_of_values;
	     value_iterator++ )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libesedb_record_get_utf16_column_name_size(
		          record,
		          value_iterator,
		          &column_name_size,
		          error );

#else
		result = libesedb_record_get_utf8_column_name_size(
		          record,
		          value_iterator,
		          &column_name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column name size of value: %d.",
			 function,
			 value_iterator );

			return( -1 );
		}
		/* It is assumed that the column name cannot be larger than 255 characters
		 * otherwise using dynamic allocation is more appropriate
		 */
		if( column_name_size > 256 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: column name size value exceeds maximum.",
			 function );

			return( -1 );
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libesedb_record_get_utf16_column_name(
		          record,
		          value_iterator,
		          (uint16_t *) column_name,
		          column_name_size,
		          error );
#else
		result = libesedb_record_get_utf8_column_name(
		          record,
		          value_iterator,
		          (uint8_t *) column_name,
		          column_name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column name of value: %d.",
			 function,
			 value_iterator );

			return( -1 );
		}
		if( libesedb_record_get_column_type(
		     record,
		     value_iterator,
		     &column_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column type of value: %d.",
			 function,
			 value_iterator );

			return( -1 );
		}
		known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_UNDEFINED;

		if( column_type == LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_SIGNED )
		{
			if( system_string_compare(
			     column_name,
			     _SYSTEM_STRING( "__SDID" ),
			     6 ) == 0 )
			{
				/* The byte order is set because the SystemIndex_0A table in the
				 * Windows Search XP and 7 database contains binary values in big-endian
				 * In the Windows Search XP database the __SDID column is of type binary data.
				 * In the Windows Search Vista data base the __SDID column is of type integer 32-bit signed.
				 * In Windows Search 7 the __SDID column is no longer present
				 */
				byte_order = _BYTE_STREAM_ENDIAN_LITTLE;
			}
		}
		if( ( column_type == LIBESEDB_COLUMN_TYPE_CURRENCY )
		 || ( column_type == LIBESEDB_COLUMN_TYPE_BINARY_DATA )
		 || ( column_type == LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA ) )
		{
			if( column_name_size == 24 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_ThumbnailCacheId" ),
				     23 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_INTEGER_64BIT;
					format            = WINDOWS_SEARCH_FORMAT_HEXADECIMAL;
				}
			}
		}
		/* Only check for known columns of the binary data type
		 * some columns get their type reassigned over time
		 */
		if( ( column_type == LIBESEDB_COLUMN_TYPE_BINARY_DATA )
		 || ( column_type == LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA ) )
		{
			if( column_name_size == 7 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "__SDID" ),
				     6 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_INTEGER_32BIT;
				}
			}
			else if( column_name_size == 12 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_Kind" ),
				     11 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Size" ),
				          11 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_INTEGER_64BIT;
					format            = WINDOWS_SEARCH_FORMAT_DECIMAL;
				}
			}
			else if( column_name_size == 13 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_Title" ),
				     12 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 14 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_Author" ),
				     13 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 15 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_Comment" ),
				     14 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_DueDate" ),
				          14 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_EndDate" ),
				          14 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_ItemUrl" ),
				          14 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Subject" ),
				          14 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 16 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_FileName" ),
				     15 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Identity" ),
				          15 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_ItemDate" ),
				          15 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_ItemName" ),
				          15 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_ItemType" ),
				          15 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING(  "System_KindText" ),
				          15 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_MIMEType" ),
				          15 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 17 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_Copyright" ),
				     16 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_FileOwner" ),
				          16 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 18 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_Media_MCDI" ),
				     17 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_RatingText" ),
				          17 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 19 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_DateCreated" ),
				     18 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_ItemAuthors" ),
				          18 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Music_Genre" ),
				          18 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_ParsingName" ),
				          18 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 20 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_ComputerName" ),
				     19 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_DateAccessed" ),
				          19 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_DateAcquired" ),
				          19 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( system_string_compare(
					  column_name,
				          _SYSTEM_STRING( "System_DateImported" ),
					  19 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( system_string_compare(
					  column_name,
				          _SYSTEM_STRING( "System_DateModified" ),
					  19 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_ItemTypeText" ),
				          19 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Music_Artist" ),
				          19 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Search_Store" ),
				          19 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 21 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_FileExtension" ),
				     20 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Message_Store" ),
				          20 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 22 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_FlagStatusText" ),
				     21 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_ImportanceText" ),
				          21 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_ItemNamePrefix" ),
				          21 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Media_SubTitle" ),
				          21 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Message_ToName" ),
				          21 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Music_Composer" ),
				          21 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 23 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_ItemNameDisplay" ),
				     22 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_ItemPathDisplay" ),
				          22 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Media_ContentID" ),
				          22 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Media_Publisher" ),
				          22 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Music_PartOfSet" ),
				          22 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Photo_DateTaken" ),
				          22 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
			}
			else if( column_name_size == 24 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_Image_Dimensions" ),
				     23 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_ItemParticipants" ),
				          23 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Message_DateSent" ),
				          23 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Message_FromName" ),
				          23 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Music_AlbumTitle" ),
				          23 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 25 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_Calendar_Location" ),
				     24 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Media_DateEncoded" ),
				          24 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Message_CcAddress" ),
				          24 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Message_ToAddress" ),
				          24 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Music_AlbumArtist" ),
				          24 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Search_GatherTime" ),
				          24 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
			}
			else if( column_name_size == 26 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_Document_DateSaved" ),
				     25 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Media_CollectionID" ),
				          25 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Media_DateReleased" ),
				          25 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Message_SenderName" ),
				          25 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Search_AutoSummary" ),
				          25 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 27 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_Message_FromAddress" ),
				     26 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 28 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_Document_DateCreated" ),
				     27 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Media_ClassPrimaryID" ),
				          27 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Message_DateReceived" ),
				          27 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Message_MessageClass" ),
				          27 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 29 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_ItemFolderNameDisplay" ),
				     28 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_ItemFolderPathDisplay" ),
				          28 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_ItemPathDisplayNarrow" ),
				          28 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Message_SenderAddress" ),
				          28 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 30 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_Link_TargetParsingPath" ),
				     29 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Media_ClassSecondaryID" ),
				          29 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Message_ConversationID" ),
				          29 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_RecordedTV_EpisodeName" ),
				          29 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 31 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_Calendar_ShowTimeAsText" ),
				     30 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "System_Media_CollectionGroupID" ),
				          30 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 32 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_RecordedTV_RecordingTime" ),
				     31 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
			}
			else if( column_name_size == 35 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_ItemFolderPathDisplayNarrow" ),
				     34 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 37 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "System_RecordedTV_ProgramDescription" ),
				     36 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
		}
		if( known_column_type == WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_INTEGER_32BIT )
		{
			result = windows_search_export_record_value_32bit(
				  record,
				  value_iterator,
				  byte_order,
				  record_file_stream,
				  error );
		}
		else if( known_column_type == WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_INTEGER_64BIT )
		{
			result = windows_search_export_record_value_64bit(
				  record,
				  value_iterator,
				  byte_order,
				  format,
				  record_file_stream,
				  error );
		}
		else if( known_column_type == WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME )
		{
			result = windows_search_export_record_value_filetime(
				  record,
				  value_iterator,
				  byte_order,
				  record_file_stream,
				  error );
		}
		else if( known_column_type == WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED )
		{
			result = windows_search_export_record_value_compressed_string(
				  record,
				  value_iterator,
				  ascii_codepage,
				  record_file_stream,
				  log_handle,
				  error );
		}
		else if( known_column_type == WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_UNDEFINED )
		{
			result = export_handle_export_record_value(
				  record,
				  value_iterator,
				  record_file_stream,
				  log_handle,
				  error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export record value: %d.",
			 function,
			 value_iterator );

			return( -1 );
		}
		if( value_iterator == ( number_of_values - 1 ) )
		{
			fprintf(
			 record_file_stream,
			 "\n" );
		}
		else
		{
			fprintf(
			 record_file_stream,
			 "\t" );
		}
	}
	return( 1 );
}

/* Exports the values in a SystemIndex_Gthr table record
 * Returns 1 if successful or -1 on error
 */
int windows_search_export_record_systemindex_gthr(
     libesedb_record_t *record,
     FILE *record_file_stream,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	system_character_t column_name[ 256 ];

	static char *function   = "windows_search_export_record_systemindex_gthr";
	size_t column_name_size = 0;
	uint32_t column_type    = 0;
	int known_column_type   = 0;
	int number_of_values    = 0;
	int result              = 0;
	int value_iterator      = 0;
	uint8_t byte_order      = _BYTE_STREAM_ENDIAN_LITTLE;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( record_file_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record file stream.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_number_of_values(
	     record,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		return( -1 );
	}
	for( value_iterator = 0;
	     value_iterator < number_of_values;
	     value_iterator++ )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libesedb_record_get_utf16_column_name_size(
		          record,
		          value_iterator,
		          &column_name_size,
		          error );

#else
		result = libesedb_record_get_utf8_column_name_size(
		          record,
		          value_iterator,
		          &column_name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column name size of value: %d.",
			 function,
			 value_iterator );

			return( -1 );
		}
		/* It is assumed that the column name cannot be larger than 255 characters
		 * otherwise using dynamic allocation is more appropriate
		 */
		if( column_name_size > 256 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: column name size value exceeds maximum.",
			 function );

			return( -1 );
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libesedb_record_get_utf16_column_name(
		          record,
		          value_iterator,
		          (uint16_t *) column_name,
		          column_name_size,
		          error );
#else
		result = libesedb_record_get_utf8_column_name(
		          record,
		          value_iterator,
		          (uint8_t *) column_name,
		          column_name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column name of value: %d.",
			 function,
			 value_iterator );

			return( -1 );
		}
		if( libesedb_record_get_column_type(
		     record,
		     value_iterator,
		     &column_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column type of value: %d.",
			 function,
			 value_iterator );

			return( -1 );
		}
		known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_UNDEFINED;

		/* Only check for known columns of the binary data type
		 * some columns get their type reassigned over time
		 */
		if( ( column_type == LIBESEDB_COLUMN_TYPE_BINARY_DATA )
		 || ( column_type == LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA ) )
		{
			if( column_name_size == 10 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "FileName1" ),
				     9 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_UTF16_LITTLE_ENDIAN;
				}
				else if( system_string_compare(
					 column_name,
					 _SYSTEM_STRING( "FileName2" ),
					 9 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_UTF16_LITTLE_ENDIAN;
				}
			}
			else if( column_name_size == 13 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "LastModified" ),
				     12 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;

					byte_order = _BYTE_STREAM_ENDIAN_BIG;
				}
			}
		}
		if( known_column_type == WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME )
		{
			result = windows_search_export_record_value_filetime(
				  record,
				  value_iterator,
				  byte_order,
				  record_file_stream,
				  error );
		}
		else if( known_column_type == WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_UTF16_LITTLE_ENDIAN )
		{
			result = windows_search_export_record_value_utf16_string(
				  record,
				  value_iterator,
				  LIBUNA_ENDIAN_LITTLE,
				  record_file_stream,
				  error );
		}
		else if( known_column_type == WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_UNDEFINED )
		{
			result = export_handle_export_record_value(
				  record,
				  value_iterator,
				  record_file_stream,
				  log_handle,
				  error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export record value: %d.",
			 function,
			 value_iterator );

			return( -1 );
		}
		if( value_iterator == ( number_of_values - 1 ) )
		{
			fprintf(
			 record_file_stream,
			 "\n" );
		}
		else
		{
			fprintf(
			 record_file_stream,
			 "\t" );
		}
	}
	return( 1 );
}

