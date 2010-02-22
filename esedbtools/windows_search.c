/* 
 * Windows Search database export functions
 *
 * Copyright (C) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
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
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>

/* Define HAVE_LOCAL_LIBUNA for local use of libuna
 */
#if defined( HAVE_LOCAL_LIBUNA )

#include <libuna_base16_stream.h>
#include <libuna_base64_stream.h>
#include <libuna_base64url_stream.h>
#include <libuna_byte_stream.h>
#include <libuna_unicode_character.h>
#include <libuna_url_stream.h>
#include <libuna_utf16_stream.h>
#include <libuna_utf16_string.h>
#include <libuna_utf32_stream.h>
#include <libuna_utf32_string.h>
#include <libuna_utf8_stream.h>
#include <libuna_utf8_string.h>

#elif defined( HAVE_LIBUNA_H )

/* If libtool DLL support is enabled set LIBUNA_DLL_IMPORT
 * before including libuna.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBUNA_DLL_IMPORT
#endif

#include <libuna.h>

#else
#error Missing libuna.h
#endif

/* Define HAVE_LOCAL_LIBFDATETIME for local use of libfdatetime
 */
#if defined( HAVE_LOCAL_LIBFDATETIME )

#include <libfdatetime_date_time_values.h>
#include <libfdatetime_definitions.h>
#include <libfdatetime_error.h>
#include <libfdatetime_fat_date_time.h>
#include <libfdatetime_filetime.h>
#include <libfdatetime_types.h>

#elif defined( HAVE_LIBFDATETIME_H )

/* If libtool DLL support is enabled set LIBFDATETIME_DLL_IMPORT
 * before including libfdatetime.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBFDATETIME_DLL_IMPORT
#endif

#include <libfdatetime.h>

#else
#error Missing libfdatetime.h
#endif

/* If libtool DLL support is enabled set LIBESEDB_DLL_IMPORT
 * before including libesedb_extern.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBESEDB_DLL_EXPORT
#endif

#include <libesedb.h>

#include "export_handle.h"
#include "windows_search.h"

enum WINDOWS_SEARCH_KNOWN_COLUMN_TYPES
{
	WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_UNDEFINED,
	WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_INTEGER_32BIT,
	WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_INTEGER_64BIT,
	WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME,
	WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED,
	WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_UTF16_LITTLE_ENDIAN,
};

/* Decode data using MS Search encoding
 * Returns 1 on success or -1 on error
 */
int windows_search_decode(
     uint8_t *data,
     size_t data_size,
     uint8_t *encoded_data, 
     size_t encoded_data_size,
     liberror_error_t **error )
{
	static char *function        = "windows_search_decode";
	size_t encoded_data_iterator = 0;
	size_t data_iterator         = 0;
	uint32_t bitmask32           = 0;
	uint8_t bitmask              = 0;

	if( data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( encoded_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encoded data.",
		 function );

		return( -1 );
	}
	if( encoded_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid encoded data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( data_size < encoded_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: data size value too small.",
		 function );

		return( -1 );
	}
	bitmask32  = 0x05000113;

	bitmask32 ^= (uint32_t) encoded_data_size;

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

/* Determines the uncompressed data size using MS Search UTF-16 string compression
 * Returns 1 on success or -1 on error
 */
int windows_search_get_utf16_string_uncompressed_data_size(
     uint8_t *compressed_data,
     size_t compressed_data_size,
     size_t *uncompressed_data_size,
     liberror_error_t **error )
{
	static char *function           = "windows_search_decompress_utf16_string";
	size_t compressed_data_iterator = 0;
	uint8_t compression_size        = 0;

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
	if( uncompressed_data_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data size.",
		 function );

		return( -1 );
	}
	*uncompressed_data_size = 0;

	while( compressed_data_iterator < compressed_data_size )
	{
		if( compressed_data_iterator >= compressed_data_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: compressed data size value too small.",
			 function );

			return( -1 );
		}
		compression_size = compressed_data[ compressed_data_iterator++ ];

		*uncompressed_data_size  += compression_size * 2;
		compressed_data_iterator += 1 + compression_size;
	}
	if( compressed_data_iterator > compressed_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: compressed data size value too small.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Decompresses a UTF-16 string compressed string into a UTF-16 little-endian string
 * Returns 1 on success or -1 on error
 */
int windows_search_decompress_utf16_string(
     uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     uint8_t *compressed_data,
     size_t compressed_data_size,
     liberror_error_t **error )
{
	static char *function             = "windows_search_decompress_utf16_string";
	size_t compressed_data_iterator   = 0;
	size_t uncompressed_data_iterator = 0;
	uint8_t compression_size          = 0;
	uint8_t compression_byte          = 0;

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
	while( compressed_data_iterator < compressed_data_size )
	{
		if( ( compressed_data_iterator + 1 ) >= compressed_data_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: compressed data size value too small.",
			 function );

			return( -1 );
		}
		compression_size = compressed_data[ compressed_data_iterator++ ];
		compression_byte = compressed_data[ compressed_data_iterator++ ];

		while( compression_size > 0 )
		{
			if( compressed_data_iterator >= compressed_data_size )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: compressed data size value too small.",
				 function );

				return( -1 );
			}
			if( ( uncompressed_data_iterator + 1 ) >= uncompressed_data_size )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: uncompressed data size value too small.",
				 function );

				return( -1 );
			}
			uncompressed_data[ uncompressed_data_iterator++ ] = compressed_data[ compressed_data_iterator++ ];
			uncompressed_data[ uncompressed_data_iterator++ ] = compression_byte;

			compression_size--;
		}
	}
	return( 1 );
}

/* Exports a compressed string
 * Returns 1 if successful or -1 on error
 */
int windows_search_export_compressed_string_value(
     uint8_t *value_data,
     size_t value_data_size,
     FILE *table_file_stream,
     liberror_error_t **error )
{
	uint8_t *decoded_value_data    = NULL;
	uint8_t *value_string          = NULL;
	uint8_t *value_utf16_stream    = NULL;
	static char *function          = "windows_search_export_compressed_string_value";
	size_t decoded_value_data_size = 0;
	size_t value_string_size       = 0;
	size_t value_utf16_stream_size = 0;

	if( value_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data.",
		 function );

		return( -1 );
	}
	if( value_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid value data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( table_file_stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table file stream.",
		 function );

		return( -1 );
	}
	decoded_value_data_size = value_data_size;

	decoded_value_data = (uint8_t *) memory_allocate(
					  sizeof( uint8_t ) * decoded_value_data_size );

	if( decoded_value_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to decode value data.",
		 function );

		memory_free(
		 decoded_value_data );

		return( -1 );
	}
	if( decoded_value_data[ 0 ] == 0 )
	{
		if( windows_search_get_utf16_string_uncompressed_data_size(
		     &( decoded_value_data[ 1 ] ),
		     decoded_value_data_size - 1,
		     &value_utf16_stream_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 string uncompressed data size.",
			 function );

			memory_free(
			 decoded_value_data );

			return( -1 );
		}
		value_utf16_stream = (uint8_t *) memory_allocate(
						  sizeof( uint8_t ) * value_utf16_stream_size );

		if( value_utf16_stream == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value UTF-16 stream.",
			 function );

			memory_free(
			 decoded_value_data );

			return( -1 );
		}
		if( windows_search_decompress_utf16_string(
		     value_utf16_stream,
		     value_utf16_stream_size,
		     &( decoded_value_data[ 1 ] ),
		     decoded_value_data_size - 1,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to decompress UTF-16 string compressed data size.",
			 function );

			memory_free(
			 value_utf16_stream );
			memory_free(
			 decoded_value_data );

			return( -1 );
		}
		memory_free(
		 decoded_value_data );

		if( libuna_utf8_string_size_from_utf16_stream(
		     value_utf16_stream,
		     value_utf16_stream_size,
		     LIBUNA_ENDIAN_LITTLE,
		     &value_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine size of value UTF-16 stream.",
			 function );

			memory_free(
			 value_utf16_stream );

			return( -1 );
		}
		value_string = (uint8_t *) memory_allocate(
					    sizeof( uint8_t ) * value_string_size );

		if( value_string == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value string.",
			 function );

			memory_free(
			 value_utf16_stream );

			return( -1 );
		}
		if( libuna_utf8_string_copy_from_utf16_stream(
		     value_string,
		     value_string_size,
		     value_utf16_stream,
		     value_utf16_stream_size,
		     LIBUNA_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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

		fprintf(
		 table_file_stream,
		 "%s",
		 value_string );

		memory_free(
		 value_string );
	}
	else if( decoded_value_data[ 0 ] == 1 )
	{
		if( libuna_utf8_string_size_from_byte_stream(
		     &( decoded_value_data[ 1 ] ),
		     decoded_value_data_size - 1,
		     LIBUNA_CODEPAGE_ASCII,
		     &value_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine size of value string.",
			 function );

			memory_free(
			 decoded_value_data );

			return( -1 );
		}
		value_string = (uint8_t *) memory_allocate(
					    sizeof( uint8_t ) * value_string_size );

		if( value_string == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value string.",
			 function );

			memory_free(
			 decoded_value_data );

			return( -1 );
		}
		if( libuna_utf8_string_copy_from_byte_stream(
		     value_string,
		     value_string_size,
		     &( decoded_value_data[ 1 ] ),
		     decoded_value_data_size - 1,
		     LIBUNA_CODEPAGE_ASCII,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value string.",
			 function );

			memory_free(
			 value_string );
			memory_free(
			 decoded_value_data );

			return( -1 );
		}
		memory_free(
		 decoded_value_data );

		fprintf(
		 table_file_stream,
		 "%s",
		 value_string );

		memory_free(
		 value_string );
	}
	else
	{
		fprintf(
		 table_file_stream,
		 "COMPRESSION TYPE: 0x%02" PRIx8 "",
		 decoded_value_data[ 0 ] );
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
     FILE *table_file_stream,
     liberror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "windows_search_export_record_value_32bit";
	size_t value_data_size = 0;
	uint32_t column_type   = 0;
	uint32_t value_32bit   = 0;
	uint8_t value_flags    = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
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
		 "%s: unsupported byte order: 0x%02" PRIx8 "",
		 function,
		 byte_order );

		return( -1 );
	}
	if( table_file_stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table file stream.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_column_type(
	     record,
	     record_value_entry,
	     &column_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column type of value: %d.",
		 function,
		 record_value_entry + 1 );

		return( -1 );
	}
	if( column_type != LIBESEDB_COLUMN_TYPE_BINARY_DATA )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 "",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_record_get_value(
	     record,
	     record_value_entry,
	     &value_data,
	     &value_data_size,
	     &value_flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d.",
		 function,
		 record_value_entry + 1 );

		return( -1 );
	}
	if( ( value_flags & ~LIBESEDB_VALUE_FLAG_VARIABLE_SIZE ) == 0 )
	{
		if( value_data != NULL )
		{
			if( value_data_size != 4 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported value data size: %" PRIzd "",
				 function,
				 value_data_size );

				return( -1 );
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
			 table_file_stream,
			 "%" PRIu32 "",
			 value_32bit );
		}
	}
	else
	{
		if( value_data != NULL )
		{
			while( value_data_size > 0 )
			{
				fprintf(
				 table_file_stream,
				 "%02" PRIx8 "",
				 *value_data );

				value_data      += 1;
				value_data_size -= 1;
			}
		}
	}
	return( 1 );
}

/* Exports a 64-bit value in a binary data table record value
 * Returns 1 if successful or -1 on error
 */
int windows_search_export_record_value_64bit(
     libesedb_record_t *record,
     int record_value_entry,
     uint8_t byte_order,
     FILE *table_file_stream,
     liberror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "windows_search_export_record_value_64bit";
	size_t value_data_size = 0;
	uint64_t value_64bit   = 0;
	uint32_t column_type   = 0;
	uint8_t value_flags    = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
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
		 "%s: unsupported byte order: 0x%02" PRIx8 "",
		 function,
		 byte_order );

		return( -1 );
	}
	if( table_file_stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table file stream.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_column_type(
	     record,
	     record_value_entry,
	     &column_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column type of value: %d.",
		 function,
		 record_value_entry + 1 );

		return( -1 );
	}
	if( column_type != LIBESEDB_COLUMN_TYPE_BINARY_DATA )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 "",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_record_get_value(
	     record,
	     record_value_entry,
	     &value_data,
	     &value_data_size,
	     &value_flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d.",
		 function,
		 record_value_entry + 1 );

		return( -1 );
	}
	if( ( value_flags & ~LIBESEDB_VALUE_FLAG_VARIABLE_SIZE ) == 0 )
	{
		if( value_data != NULL )
		{
			if( value_data_size != 8 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported value data size: %" PRIzd "",
				 function,
				 value_data_size );

				return( -1 );
			}
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
			fprintf(
			 table_file_stream,
			 "%" PRIu64 "",
			 value_64bit );
		}
	}
	else
	{
		if( value_data != NULL )
		{
			while( value_data_size > 0 )
			{
				fprintf(
				 table_file_stream,
				 "%02" PRIx8 "",
				 *value_data );

				value_data      += 1;
				value_data_size -= 1;
			}
		}
	}
	return( 1 );
}

/* Exports a filetime value in a binary data table record value
 * Returns 1 if successful or -1 on error
 */
int windows_search_export_record_value_filetime(
     libesedb_record_t *record,
     int record_value_entry,
     uint8_t byte_order,
     FILE *table_file_stream,
     liberror_error_t **error )
{
	uint8_t filetime_string[ 22 ];

	libfdatetime_filetime_t *filetime = NULL;
	uint8_t *value_data               = NULL;
	static char *function             = "windows_search_export_record_value_filetime";
	size_t value_data_size            = 0;
	uint32_t column_type              = 0;
	uint8_t value_flags               = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( table_file_stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table file stream.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_column_type(
	     record,
	     record_value_entry,
	     &column_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column type of value: %d.",
		 function,
		 record_value_entry + 1 );

		return( -1 );
	}
	if( column_type != LIBESEDB_COLUMN_TYPE_BINARY_DATA )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 "",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_record_get_value(
	     record,
	     record_value_entry,
	     &value_data,
	     &value_data_size,
	     &value_flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d.",
		 function,
		 record_value_entry + 1 );

		return( -1 );
	}
	if( ( value_flags & ~LIBESEDB_VALUE_FLAG_VARIABLE_SIZE ) == 0 )
	{
		if( value_data != NULL )
		{
			if( value_data_size != 8 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported value data size: %" PRIzd "",
				 function,
				 value_data_size );

				return( -1 );
			}
			if( libfdatetime_filetime_initialize(
			     &filetime,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create filetime.",
				 function );

				return( -1 );
			}
			if( libfdatetime_filetime_copy_from_byte_stream(
			     filetime,
			     value_data,
			     value_data_size,
			     byte_order,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_CONVERSION,
				 LIBERROR_CONVERSION_ERROR_GENERIC,
				 "%s: unable to create filetime.",
				 function );

				libfdatetime_filetime_free(
				 &filetime,
				 NULL );

				return( -1 );
			}
			if( libfdatetime_filetime_copy_to_utf8_string(
			     filetime,
			     filetime_string,
			     22,
			     LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			     LIBFDATETIME_DATE_TIME_FORMAT_CTIME,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_CONVERSION,
				 LIBERROR_CONVERSION_ERROR_GENERIC,
				 "%s: unable to create filetime string.",
				 function );

				libfdatetime_filetime_free(
				 &filetime,
				 NULL );

				return( -1 );
			}
			if( libfdatetime_filetime_free(
			     &filetime,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free filetime.",
				 function );

				return( -1 );
			}
			fprintf(
			 table_file_stream,
			 "%s",
			 filetime_string );
		}
	}
	else
	{
		if( value_data != NULL )
		{
			while( value_data_size > 0 )
			{
				fprintf(
				 table_file_stream,
				 "%02" PRIx8 "",
				 *value_data );

				value_data      += 1;
				value_data_size -= 1;
			}
		}
	}
	return( 1 );
}

/* Exports a compressed string in a binary data table record value
 * Returns 1 if successful or -1 on error
 */
int windows_search_export_record_value_compressed_string(
     libesedb_record_t *record,
     int record_value_entry,
     FILE *table_file_stream,
     liberror_error_t **error )
{
	libesedb_long_value_t *long_value   = NULL;
	libesedb_multi_value_t *multi_value = NULL;
	uint8_t *value_data                 = NULL;
	static char *function               = "windows_search_export_record_value_compressed_string";
	size_t value_data_size              = 0;
	uint32_t column_type                = 0;
	uint8_t value_flags                 = 0;
	int amount_of_multi_values          = 0;
	int multi_value_iterator            = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( table_file_stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table file stream.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_column_type(
	     record,
	     record_value_entry,
	     &column_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column type of value: %d.",
		 function,
		 record_value_entry + 1 );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_BINARY_DATA )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 "",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_record_get_value(
	     record,
	     record_value_entry,
	     &value_data,
	     &value_data_size,
	     &value_flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value of record entry: %d.",
		 function,
		 record_value_entry + 1 );

		return( -1 );
	}
	if( ( value_flags & ~LIBESEDB_VALUE_FLAG_VARIABLE_SIZE ) == 0 )
	{
		if( value_data != NULL )
		{
			if( windows_search_export_compressed_string_value(
			     value_data,
			     value_data_size,
			     table_file_stream,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to export compressed string value of record entry: %d.",
				 function,
				 record_value_entry + 1 );

				return( -1 );
			}
		}
	}
	else if( ( value_flags & LIBESEDB_VALUE_FLAG_LONG_VALUE ) == LIBESEDB_VALUE_FLAG_LONG_VALUE )
	{
		if( libesedb_record_get_long_value(
		     record,
		     record_value_entry,
		     &long_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve long value of record entry: %d.",
			 function,
			 record_value_entry + 1 );

			return( -1 );
		}
		/* TODO */

		if( libesedb_long_value_free(
		     &long_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free long value.",
			 function );

			return( -1 );
		}
	}
	else if( ( value_flags & LIBESEDB_VALUE_FLAG_MULTI_VALUE ) == LIBESEDB_VALUE_FLAG_MULTI_VALUE )
	{
		if( libesedb_record_get_multi_value(
		     record,
		     record_value_entry,
		     &multi_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve multi value of record entry: %d.",
			 function,
			 record_value_entry + 1 );

			return( -1 );
		}
		if( libesedb_multi_value_get_amount_of_values(
		     multi_value,
		     &amount_of_multi_values,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve amount of multi values.",
			 function );

			libesedb_multi_value_free(
			 &multi_value,
			 NULL );

			return( -1 );
		}
		for( multi_value_iterator = 0;
	 	     multi_value_iterator < amount_of_multi_values;
		     multi_value_iterator++ )
		{
			if( libesedb_multi_value_get_value(
			     multi_value,
			     multi_value_iterator,
			     &column_type,
			     &value_data,
			     &value_data_size,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve mulit value: %d of record entry: %d.",
				 function,
				 multi_value_iterator + 1,
				 record_value_entry + 1 );

				return( -1 );
			}
			if( value_data != NULL )
			{
				if( windows_search_export_compressed_string_value(
				     value_data,
				     value_data_size,
				     table_file_stream,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_GENERIC,
					 "%s: unable to export compressed string value of record entry: %d.",
					 function,
					 record_value_entry + 1 );

					return( -1 );
				}
				if( multi_value_iterator < ( amount_of_multi_values - 1 ) )
				{
					fprintf(
					 table_file_stream,
					 "; " );
				}
			}
		}
		if( libesedb_multi_value_free(
		     &multi_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free multi value.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( value_data != NULL )
		{
			while( value_data_size > 0 )
			{
				fprintf(
				 table_file_stream,
				 "%02" PRIx8 "",
				 *value_data );

				value_data      += 1;
				value_data_size -= 1;
			}
		}
	}
	return( 1 );
}

/* Exports an UTF-16 string in a binary data table record value
 * Returns 1 if successful or -1 on error
 */
int windows_search_export_record_value_utf16_string(
     libesedb_record_t *record,
     int record_value_entry,
     uint8_t byte_order,
     FILE *table_file_stream,
     liberror_error_t **error )
{
	uint8_t *value_data      = NULL;
	uint8_t *value_string    = NULL;
	static char *function    = "windows_search_export_record_value_utf16_string";
	size_t value_data_size   = 0;
	size_t value_string_size = 0;
	uint32_t column_type     = 0;
	uint8_t value_flags      = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( table_file_stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table file stream.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_column_type(
	     record,
	     record_value_entry,
	     &column_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column type of value: %d.",
		 function,
		 record_value_entry + 1 );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_BINARY_DATA )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 "",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_record_get_value(
	     record,
	     record_value_entry,
	     &value_data,
	     &value_data_size,
	     &value_flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d.",
		 function,
		 record_value_entry + 1 );

		return( -1 );
	}
	if( ( value_flags & ~LIBESEDB_VALUE_FLAG_VARIABLE_SIZE ) == 0 )
	{
		if( value_data != NULL )
		{
			if( libuna_utf8_string_size_from_utf16_stream(
			     value_data,
			     value_data_size,
			     byte_order,
			     &value_string_size,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine size of value string: %d.",
				 function,
				 record_value_entry + 1 );

				return( -1 );
			}
			value_string = (uint8_t *) memory_allocate(
						    sizeof( uint8_t ) * value_string_size );

			if( value_string == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create value string.",
				 function );

				return( -1 );
			}
			if( libuna_utf8_string_copy_from_utf16_stream(
			     value_string,
			     value_string_size,
			     value_data,
			     value_data_size,
			     byte_order,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value string: %d.",
				 function,
				 record_value_entry + 1 );

				memory_free(
				 value_string );

				return( -1 );
			}
			fprintf(
			 table_file_stream,
			 "%s",
			 value_string );

			memory_free(
			 value_string );
		}
	}
	else
	{
		if( value_data != NULL )
		{
			while( value_data_size > 0 )
			{
				fprintf(
				 table_file_stream,
				 "%02" PRIx8 "",
				 *value_data );

				value_data      += 1;
				value_data_size -= 1;
			}
		}
	}
	return( 1 );
}

/* Exports the values in a SystemIndex_0A table record
 * Returns 1 if successful or -1 on error
 */
int windows_search_export_record_systemindex_0a(
     libesedb_record_t *record,
     FILE *table_file_stream,
     liberror_error_t **error )
{
	uint8_t column_name[ 256 ];

	static char *function   = "windows_search_export_record_systemindex_0a";
	size_t column_name_size = 0;
	uint32_t column_type    = 0;
	int amount_of_values    = 0;
	int known_column_type   = 0;
	int result              = 0;
	int value_iterator      = 0;
	uint8_t byte_order      = _BYTE_STREAM_ENDIAN_LITTLE;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( table_file_stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table file stream.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_amount_of_values(
	     record,
	     &amount_of_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of values.",
		 function );

		return( -1 );
	}
	for( value_iterator = 0;
	     value_iterator < amount_of_values;
	     value_iterator++ )
	{
		if( libesedb_record_get_utf8_column_name_size(
		     record,
		     value_iterator,
		     &column_name_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column name size of value: %d.",
			 function,
			 value_iterator + 1 );

			return( -1 );
		}
		/* It is assumed that the column name cannot be larger than 255 characters
		 * otherwise using dynamic allocation is more appropriate
		 */
		if( column_name_size > 256 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: column name size value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( libesedb_record_get_utf8_column_name(
		     record,
		     value_iterator,
		     column_name,
		     column_name_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column name of value: %d.",
			 function,
			 value_iterator + 1 );

			return( -1 );
		}
		if( libesedb_record_get_column_type(
		     record,
		     value_iterator,
		     &column_type,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column type of value: %d.",
			 function,
			 value_iterator + 1 );

			return( -1 );
		}
		known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_UNDEFINED;

		/* Only check for known columns of the binary data type
		 * some columns get their type reassigned over time
		 */
		if( ( column_type == LIBESEDB_COLUMN_TYPE_BINARY_DATA )
		 || ( column_type == LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA ) )
		{
			if( column_name_size == 7 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "__SDID",
				     6 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_INTEGER_32BIT;

					/* The byte order is set because the SystemIndex_0A table in the
					 * Windows Search XP database contains binary values in big-endian
					 * In the Windows Search XP database the __SDID value is of type binary data.
					 * In the Windows Search Vista data base the __SDID value is of type
					 * signed integer 32-bit.
					 */
					byte_order = _BYTE_STREAM_ENDIAN_BIG;
				}
			}
			else if( column_name_size == 12 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_Kind",
				     11 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Size",
				          11 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_INTEGER_64BIT;
				}
			}
			else if( column_name_size == 13 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_Title",
				     12 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 14 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_Author",
				     13 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 15 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_Comment",
				     14 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_EndDate",
				          14 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_ItemUrl",
				          14 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Subject",
				          14 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 16 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_FileName",
				     15 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Identity",
				          15 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_ItemDate",
				          15 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_ItemName",
				          15 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_ItemType",
				          15 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_KindText",
				          15 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_MIMEType",
				          15 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 17 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_Copyright",
				     16 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_FileOwner",
				          16 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 18 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_Media_MCDI",
				     17 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_RatingText",
				          17 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 19 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_DateCreated",
				     18 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_ItemAuthors",
				          18 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Music_Genre",
				          18 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_ParsingName",
				          18 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 20 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_ComputerName",
				     19 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_DateAccessed",
				          19 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_DateAcquired",
				          19 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( narrow_string_compare(
					  (char *) column_name,
				          "System_DateImported",
					  19 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( narrow_string_compare(
					  (char *) column_name,
				          "System_DateModified",
					  19 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_ItemTypeText",
				          19 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Music_Artist",
				          19 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Search_Store",
				          19 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 21 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_FileExtension",
				     20 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Message_Store",
				          20 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 22 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_FlagStatusText",
				     21 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Media_SubTitle",
				          21 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Message_ToName",
				          21 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Music_Composer",
				          21 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 23 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_ItemNameDisplay",
				     22 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_ItemPathDisplay",
				          22 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Media_ContentID",
				          22 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Media_Publisher",
				          22 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Music_PartOfSet",
				          22 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Photo_DateTaken",
				          22 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
			}
			else if( column_name_size == 24 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_Image_Dimensions",
				     23 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_ItemParticipants",
				          23 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Message_DateSent",
				          23 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Message_FromName",
				          23 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Music_AlbumTitle",
				          23 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 25 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_Calendar_Location",
				     24 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Media_DateEncoded",
				          24 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Message_CcAddress",
				          24 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Message_ToAddress",
				          24 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Music_AlbumArtist",
				          24 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Search_GatherTime",
				          24 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
			}
			else if( column_name_size == 26 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_Document_DateSaved",
				     25 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Media_CollectionID",
				          25 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Media_DateReleased",
				          25 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Search_AutoSummary",
				          25 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 27 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_Message_FromAddress",
				     26 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 28 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_Document_DateCreated",
				     27 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Media_ClassPrimaryID",
				          27 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Message_DateReceived",
				          27 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Message_MessageClass",
				          27 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 29 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_ItemFolderNameDisplay",
				     28 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_ItemFolderPathDisplay",
				          28 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_ItemPathDisplayNarrow",
				          28 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Message_SenderAddress",
				          28 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 30 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_Link_TargetParsingPath",
				     29 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Media_ClassSecondaryID",
				          29 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_RecordedTV_EpisodeName",
				          29 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Message_ConversationID",
				          29 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 31 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_Calendar_ShowTimeAsText",
				     30 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Media_CollectionGroupID",
				          30 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 32 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_RecordedTV_RecordingTime",
				     31 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
			}
			else if( column_name_size == 35 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_ItemFolderPathDisplayNarrow",
				     34 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED;
				}
			}
			else if( column_name_size == 37 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_RecordedTV_ProgramDescription",
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
				  table_file_stream,
				  error );
		}
		else if( known_column_type == WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_INTEGER_64BIT )
		{
			result = windows_search_export_record_value_64bit(
				  record,
				  value_iterator,
				  byte_order,
				  table_file_stream,
				  error );
		}
		else if( known_column_type == WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME )
		{
			result = windows_search_export_record_value_filetime(
				  record,
				  value_iterator,
				  byte_order,
				  table_file_stream,
				  error );
		}
		else if( known_column_type == WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_COMPRESSED )
		{
			result = windows_search_export_record_value_compressed_string(
				  record,
				  value_iterator,
				  table_file_stream,
				  error );
		}
		else if( known_column_type == WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_UNDEFINED )
		{
			result = export_handle_export_record_value(
				  record,
				  value_iterator,
				  table_file_stream,
				  error );
		}
		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export record value: %d.",
			 function,
			 value_iterator + 1 );

			return( -1 );
		}
		if( value_iterator == ( amount_of_values - 1 ) )
		{
			fprintf(
			 table_file_stream,
			 "\n" );
		}
		else
		{
			fprintf(
			 table_file_stream,
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
     FILE *table_file_stream,
     liberror_error_t **error )
{
	uint8_t column_name[ 256 ];

	static char *function   = "windows_search_export_record_systemindex_gthr";
	size_t column_name_size = 0;
	uint32_t column_type    = 0;
	int amount_of_values    = 0;
	int known_column_type   = 0;
	int result              = 0;
	int value_iterator      = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( table_file_stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table file stream.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_amount_of_values(
	     record,
	     &amount_of_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of values.",
		 function );

		return( -1 );
	}
	for( value_iterator = 0;
	     value_iterator < amount_of_values;
	     value_iterator++ )
	{
		if( libesedb_record_get_utf8_column_name_size(
		     record,
		     value_iterator,
		     &column_name_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column name size of value: %d.",
			 function,
			 value_iterator + 1 );

			return( -1 );
		}
		/* It is assumed that the column name cannot be larger than 255 characters
		 * otherwise using dynamic allocation is more appropriate
		 */
		if( column_name_size > 256 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: column name size value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( libesedb_record_get_utf8_column_name(
		     record,
		     value_iterator,
		     column_name,
		     column_name_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column name of value: %d.",
			 function,
			 value_iterator + 1 );

			return( -1 );
		}
		if( libesedb_record_get_column_type(
		     record,
		     value_iterator,
		     &column_type,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column type of value: %d.",
			 function,
			 value_iterator + 1 );

			return( -1 );
		}
		known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_UNDEFINED;

		/* Only check for known columns of the binary data type
		 * some columns get their type reassigned over time
		 */
		if( column_type == LIBESEDB_COLUMN_TYPE_BINARY_DATA )
		{
			if( column_name_size == 10 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "FileName1",
				     9 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_UTF16_LITTLE_ENDIAN;
				}
				else if( narrow_string_compare(
					 (char *) column_name,
					 "FileName2",
					 9 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_UTF16_LITTLE_ENDIAN;
				}
			}
		}
		if( known_column_type == WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_UTF16_LITTLE_ENDIAN )
		{
			result = windows_search_export_record_value_utf16_string(
				  record,
				  value_iterator,
				  LIBUNA_ENDIAN_LITTLE,
				  table_file_stream,
				  error );
		}
		else if( known_column_type == WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_UNDEFINED )
		{
			result = export_handle_export_record_value(
				  record,
				  value_iterator,
				  table_file_stream,
				  error );
		}
		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export record value: %d.",
			 function,
			 value_iterator + 1 );

			return( -1 );
		}
		if( value_iterator == ( amount_of_values - 1 ) )
		{
			fprintf(
			 table_file_stream,
			 "\n" );
		}
		else
		{
			fprintf(
			 table_file_stream,
			 "\t" );
		}
	}
	return( 1 );
}

