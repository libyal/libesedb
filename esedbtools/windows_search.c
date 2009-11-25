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

#include <libuna_byte_stream.h>
#include <libuna_compare.h>
#include <libuna_error.h>
#include <libuna_unicode_character.h>
#include <libuna_utf8_stream.h>
#include <libuna_utf8_string.h>
#include <libuna_utf16_stream.h>
#include <libuna_utf16_string.h>
#include <libuna_utf32_stream.h>
#include <libuna_utf32_string.h>
#include <libuna_types.h>

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

#include "ascii7.h"
#include "export_handle.h"
#include "windows_search.h"

enum WINDOWS_SEARCH_KNOWN_COLUMN_TYPES
{
	WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_UNDEFINED,
	WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_INTEGER_32BIT,
	WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_INTEGER_64BIT,
	WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME,
	WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED,
	WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_UTF16_LITTLE_ENDIAN,
};

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
	uint8_t value_tag_byte = 0;

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
	     &value_tag_byte,
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
	uint8_t value_tag_byte = 0;

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
	     &value_tag_byte,
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
	uint8_t value_tag_byte            = 0;

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
	     &value_tag_byte,
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
		if( libfdatetime_filetime_copy_to_string(
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
	return( 1 );
}

/* Exports an ASCII 7-bit compressed string in a binary data table record value
 * Returns 1 if successful or -1 on error
 */
int windows_search_export_record_value_ascii_7bit_compressed_string(
     libesedb_record_t *record,
     int record_value_entry,
     FILE *table_file_stream,
     liberror_error_t **error )
{
	uint8_t *ascii_value_string    = NULL;
	uint8_t *value_data            = NULL;
	uint8_t *value_string          = NULL;
	static char *function          = "windows_search_export_record_value_ascii_7bit_compressed_string";
	size_t ascii_value_string_size = 0;
	size_t value_data_size         = 0;
	size_t value_string_size       = 0;
	uint32_t column_type           = 0;
	uint8_t value_tag_byte         = 0;

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
	     &value_tag_byte,
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
	if( value_data != NULL )
	{
		if( ascii7_decompress_get_utf8_string_size(
		     value_data,
		     value_data_size,
		     &value_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine size of compressed value string: %d.",
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
		if( ascii7_decompress_to_utf8_string(
		     value_data,
		     value_data_size,
		     value_string,
		     value_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to decompress value string: %d.",
			 function,
			 record_value_entry + 1 );

			memory_free(
			 value_string );

			return( -1 );
		}
		if( value_string[ 0 ] == 0x01 )
		{
			ascii_value_string      = value_string;
			ascii_value_string_size = value_string_size;

			if( libuna_utf8_string_size_from_byte_stream(
			     &( ascii_value_string[ 1 ] ),
			     ascii_value_string_size - 1,
			     LIBUNA_CODEPAGE_WINDOWS_1252,
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

				memory_free(
				 ascii_value_string );

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
				 ascii_value_string );

				return( -1 );
			}
			if( libuna_utf8_string_copy_from_byte_stream(
			     value_string,
			     value_string_size,
			     &( ascii_value_string[ 1 ] ),
			     ascii_value_string_size - 1,
			     LIBUNA_CODEPAGE_WINDOWS_1252,
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
				 ascii_value_string );
				memory_free(
				 value_string );

				return( -1 );
			}
			memory_free(
			 ascii_value_string );
		}
		fprintf(
		 table_file_stream,
		 "%s",
		 value_string );

		memory_free(
		 value_string );
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
	uint8_t value_tag_byte   = 0;

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
	     &value_tag_byte,
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
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
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
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
			}
			else if( column_name_size == 14 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_Author",
				     13 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
			}
			else if( column_name_size == 15 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_Comment",
				     14 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
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
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Subject",
				          14 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
			}
			else if( column_name_size == 16 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_FileName",
				     15 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Identity",
				          15 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
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
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_ItemType",
				          15 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_KindText",
				          15 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_MIMEType",
				          15 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
			}
			else if( column_name_size == 17 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_Copyright",
				     16 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_FileOwner",
				          16 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
			}
			else if( column_name_size == 18 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_Media_MCDI",
				     17 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_RatingText",
				          17 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
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
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Music_Genre",
				          18 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_ParsingName",
				          18 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
			}
			else if( column_name_size == 20 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_ComputerName",
				     19 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
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
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Music_Artist",
				          19 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Search_Store",
				          19 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
			}
			else if( column_name_size == 21 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_FileExtension",
				     20 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Message_Store",
				          20 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
			}
			else if( column_name_size == 22 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_FlagStatusText",
				     21 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Media_SubTitle",
				          21 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Message_ToName",
				          21 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Music_Composer",
				          21 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
			}
			else if( column_name_size == 23 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_ItemNameDisplay",
				     22 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_ItemPathDisplay",
				          22 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Media_ContentID",
				          22 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Media_Publisher",
				          22 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Music_PartOfSet",
				          22 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
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
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_ItemParticipants",
				          23 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
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
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Music_AlbumTitle",
				          23 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
			}
			else if( column_name_size == 25 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_Calendar_Location",
				     24 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
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
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Message_ToAddress",
				          24 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Music_AlbumArtist",
				          24 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
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
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Media_DateReleased",
				          25 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Search_AutoSummary",
				          25 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
			}
			else if( column_name_size == 27 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_Message_FromAddress",
				     26 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
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
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
#ifdef IGNORE
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Message_DateReceived",
				          27 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME;
				}
#endif
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Message_MessageClass",
				          27 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
			}
			else if( column_name_size == 29 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_ItemFolderNameDisplay",
				     28 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_ItemFolderPathDisplay",
				          28 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_ItemPathDisplayNarrow",
				          28 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
#ifdef IGNORE
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Message_SenderAddress",
				          28 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
#endif
			}
			else if( column_name_size == 30 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_Link_TargetParsingPath",
				     29 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Media_ClassSecondaryID",
				          29 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_RecordedTV_EpisodeName",
				          29 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Message_ConversationID",
				          29 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
			}
			else if( column_name_size == 31 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_Calendar_ShowTimeAsText",
				     30 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
				else if( narrow_string_compare(
				          (char *) column_name,
				          "System_Media_CollectionGroupID",
				          30 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
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
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
				}
			}
			else if( column_name_size == 37 )
			{
				if( narrow_string_compare(
				     (char *) column_name,
				     "System_RecordedTV_ProgramDescription",
				     36 ) == 0 )
				{
					known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED;
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
		else if( known_column_type == WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_ASCII7_COMPRESSED )
		{
			result = windows_search_export_record_value_ascii_7bit_compressed_string(
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

