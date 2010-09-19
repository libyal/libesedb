/* 
 * Windows Security database export functions
 *
 * Copyright (c) 2009-2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <libcstring.h>
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

#include <libsystem.h>

#include "export_handle.h"
#include "windows_security.h"

enum WINDOWS_SECURITY_KNOWN_COLUMN_TYPES
{
	WINDOWS_SECURITY_KNOWN_COLUMN_TYPE_UNDEFINED,
	WINDOWS_SECURITY_KNOWN_COLUMN_TYPE_FILETIME,
	WINDOWS_SECURITY_KNOWN_COLUMN_TYPE_STRING_UTF16_LITTLE_ENDIAN,
};

/* Exports a filetime value in a binary data table record value
 * Returns 1 if successful or -1 on error
 */
int windows_security_export_record_value_filetime(
     libesedb_record_t *record,
     int record_value_entry,
     uint8_t byte_order,
     FILE *record_file_stream,
     liberror_error_t **error )
{
	libcstring_system_character_t filetime_string[ 24 ];

	libfdatetime_filetime_t *filetime = NULL;
	uint8_t *value_data               = NULL;
	static char *function             = "windows_security_export_record_value_filetime";
	size_t value_data_size            = 0;
	uint32_t column_type              = 0;
	uint8_t value_flags               = 0;
	int result                        = 0;

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
	if( record_file_stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column type of value: %d.",
		 function,
		 record_value_entry );

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
		 record_value_entry );

		return( -1 );
	}
	if( ( value_flags & ~( LIBESEDB_VALUE_FLAG_VARIABLE_SIZE ) ) == 0 )
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
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy byte stream to filetime.",
				 function );

				libfdatetime_filetime_free(
				 &filetime,
				 NULL );

				return( -1 );
			}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libfdatetime_filetime_copy_to_utf16_string(
			          filetime,
			          (uint16_t *) filetime_string,
			          24,
			          LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			          LIBFDATETIME_DATE_TIME_FORMAT_CTIME,
			          error );
#else
			result = libfdatetime_filetime_copy_to_utf8_string(
			          filetime,
			          (uint8_t *) filetime_string,
			          24,
			          LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			          LIBFDATETIME_DATE_TIME_FORMAT_CTIME,
			          error );
#endif
			if( result != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy filetime to string.",
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
			 record_file_stream,
			 "%" PRIs_LIBCSTRING_SYSTEM "",
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
				 record_file_stream,
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
int windows_security_export_record_value_utf16_string(
     libesedb_record_t *record,
     int record_value_entry,
     uint8_t byte_order,
     FILE *record_file_stream,
     liberror_error_t **error )
{
	uint8_t *value_data      = NULL;
	uint8_t *value_string    = NULL;
	static char *function    = "windows_security_export_record_value_utf16_string";
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
	if( record_file_stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column type of value: %d.",
		 function,
		 record_value_entry );

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
		 record_value_entry );

		return( -1 );
	}
	if( ( value_flags & ~( LIBESEDB_VALUE_FLAG_VARIABLE_SIZE ) ) == 0 )
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
				 record_value_entry );

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
				 record_value_entry );

				memory_free(
				 value_string );

				return( -1 );
			}
			fprintf(
			 record_file_stream,
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
				 record_file_stream,
				 "%02" PRIx8 "",
				 *value_data );

				value_data      += 1;
				value_data_size -= 1;
			}
		}
	}
	return( 1 );
}

/* Exports the values in a SmTblVersion table record
 * Returns 1 if successful or -1 on error
 */
int windows_security_export_record_smtblversion(
     libesedb_record_t *record,
     FILE *record_file_stream,
     liberror_error_t **error )
{
	libcstring_system_character_t column_name[ 256 ];

	static char *function   = "windows_security_export_record_smtblversion";
	size_t column_name_size = 0;
	uint32_t column_type    = 0;
	int known_column_type   = 0;
	int number_of_values    = 0;
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
	if( record_file_stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record file stream.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_number_of_values(
	     record,
	     &number_of_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		return( -1 );
	}
	for( value_iterator = 0;
	     value_iterator < number_of_values;
	     value_iterator++ )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: column name size value exceeds maximum.",
			 function );

			return( -1 );
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column type of value: %d.",
			 function,
			 value_iterator );

			return( -1 );
		}
		known_column_type = WINDOWS_SECURITY_KNOWN_COLUMN_TYPE_UNDEFINED;

		/* Only check for known columns of the binary data type
		 * some columns get their type reassigned over time
		 */
		if( ( column_type == LIBESEDB_COLUMN_TYPE_BINARY_DATA )
		 || ( column_type == LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA ) )
		{
			if( column_name_size == 16 )
			{
				if( libcstring_system_string_compare(
				     column_name,
				     _LIBCSTRING_SYSTEM_STRING( "ConfigTimeStamp" ),
				     15 ) == 0 )
				{
					known_column_type = WINDOWS_SECURITY_KNOWN_COLUMN_TYPE_FILETIME;
				}
			}
			else if( column_name_size == 17 )
			{
				if( libcstring_system_string_compare(
				     column_name,
				     _LIBCSTRING_SYSTEM_STRING( "AnalyzeTimeStamp" ),
				     16 ) == 0 )
				{
					known_column_type = WINDOWS_SECURITY_KNOWN_COLUMN_TYPE_FILETIME;
				}
			}
			else if( column_name_size == 19 )
			{
				if( libcstring_system_string_compare(
				     column_name,
				     _LIBCSTRING_SYSTEM_STRING( "ProfileDescription" ),
				     18 ) == 0 )
				{
					known_column_type = WINDOWS_SECURITY_KNOWN_COLUMN_TYPE_STRING_UTF16_LITTLE_ENDIAN;
				}
			}
		}
		if( known_column_type == WINDOWS_SECURITY_KNOWN_COLUMN_TYPE_FILETIME )
		{
			result = windows_security_export_record_value_filetime(
				  record,
				  value_iterator,
				  byte_order,
				  record_file_stream,
				  error );
		}
		else if( known_column_type == WINDOWS_SECURITY_KNOWN_COLUMN_TYPE_STRING_UTF16_LITTLE_ENDIAN )
		{
			result = windows_security_export_record_value_utf16_string(
				  record,
				  value_iterator,
				  LIBUNA_ENDIAN_LITTLE,
				  record_file_stream,
				  error );
		}
		else if( known_column_type == WINDOWS_SECURITY_KNOWN_COLUMN_TYPE_UNDEFINED )
		{
			result = export_handle_export_record_value(
				  record,
				  value_iterator,
				  record_file_stream,
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

/* Exports the values in a SmTblSection table record
 * Returns 1 if successful or -1 on error
 */
int windows_security_export_record_smtblsection(
     libesedb_record_t *record,
     FILE *record_file_stream,
     liberror_error_t **error )
{
	libcstring_system_character_t column_name[ 256 ];

	static char *function   = "windows_security_export_record_smtblsection";
	size_t column_name_size = 0;
	uint32_t column_type    = 0;
	int known_column_type   = 0;
	int number_of_values    = 0;
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
	if( record_file_stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record file stream.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_number_of_values(
	     record,
	     &number_of_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		return( -1 );
	}
	for( value_iterator = 0;
	     value_iterator < number_of_values;
	     value_iterator++ )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: column name size value exceeds maximum.",
			 function );

			return( -1 );
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column type of value: %d.",
			 function,
			 value_iterator );

			return( -1 );
		}
		known_column_type = WINDOWS_SECURITY_KNOWN_COLUMN_TYPE_UNDEFINED;

		/* Only check for known columns of the binary data type
		 * some columns get their type reassigned over time
		 */
		if( ( column_type == LIBESEDB_COLUMN_TYPE_BINARY_DATA )
		 || ( column_type == LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA ) )
		{
			if( column_name_size == 5 )
			{
				if( libcstring_system_string_compare(
				     column_name,
				     _LIBCSTRING_SYSTEM_STRING( "Name" ),
				     4 ) == 0 )
				{
					known_column_type = WINDOWS_SECURITY_KNOWN_COLUMN_TYPE_STRING_UTF16_LITTLE_ENDIAN;
				}
			}
		}
		if( known_column_type == WINDOWS_SECURITY_KNOWN_COLUMN_TYPE_FILETIME )
		{
			result = windows_security_export_record_value_filetime(
				  record,
				  value_iterator,
				  byte_order,
				  record_file_stream,
				  error );
		}
		else if( known_column_type == WINDOWS_SECURITY_KNOWN_COLUMN_TYPE_STRING_UTF16_LITTLE_ENDIAN )
		{
			result = windows_security_export_record_value_utf16_string(
				  record,
				  value_iterator,
				  LIBUNA_ENDIAN_LITTLE,
				  record_file_stream,
				  error );
		}
		else if( known_column_type == WINDOWS_SECURITY_KNOWN_COLUMN_TYPE_UNDEFINED )
		{
			result = export_handle_export_record_value(
				  record,
				  value_iterator,
				  record_file_stream,
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

