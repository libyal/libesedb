/* 
 * Windows Search database export functions
 *
 * Copyright (C) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>

/* If libtool DLL support is enabled set LIBESEDB_DLL_IMPORT
 * before including libesedb_extern.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBESEDB_DLL_EXPORT
#endif

#include <libesedb.h>

#include "export_handle.h"
#include "filetime.h"
#include "windows_search.h"

enum WINDOWS_SEARCH_KNOWN_COLUMN_TYPES
{
	WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_UNDEFINED,
	WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME_BIG_ENDIAN,
	WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME_LITTLE_ENDIAN,
	WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_UTF16_LITTLE_ENDIAN,
};

/* Exports the values in a SystemIndex_Gthr table record
 * Returns 1 if successful or -1 on error
 */
int windows_search_export_record_systemindex_gthr(
     libesedb_record_t *record,
     FILE *table_file_stream,
     liberror_error_t **error )
{
	uint8_t column_name[ 256 ];

	uint8_t *value_data         = NULL;
	uint8_t *value_string       = NULL;
	static char *function       = "windows_search_export_record_systemindex_gthr";
	size_t column_name_size     = 0;
	size_t value_data_size      = 0;
	size_t value_string_size    = 0;
	double value_floating_point = 0.0;
	uint64_t value_64bit        = 0;
	uint32_t column_type        = 0;
	uint32_t value_32bit        = 0;
	uint16_t value_16bit        = 0;
	uint8_t value_8bit          = 0;
	int amount_of_values        = 0;
	int known_column_type       = 0;
	int result                  = 0;
	int value_iterator          = 0;

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

		if( column_name_size == 10 )
		{
			if( narrow_string_compare(
			     (char *) column_name,
			     "Filename1",
			     9 ) == 0 )
			{
				known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_UTF16_LITTLE_ENDIAN;
			}
			else if( narrow_string_compare(
			         (char *) column_name,
			         "Filename2",
			         9 ) == 0 )
			{
				known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_UTF16_LITTLE_ENDIAN;
			}
		}
		else if( column_name_size == 11 )
		{
			if( narrow_string_compare(
			     (char *) column_name,
			     "LastAccess",
			     10 ) == 0 )
			{
				known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME_BIG_ENDIAN;
			}
		}
		else if( column_name_size == 12 )
		{
			if( narrow_string_compare(
			     (char *) column_name,
			     "FirstAccess",
			     11 ) == 0 )
			{
				known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME_BIG_ENDIAN;
			}
		}
		else if( column_name_size == 13 )
		{
			if( narrow_string_compare(
			     (char *) column_name,
			     "LastModified",
			     12 ) == 0 )
			{
				known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME_BIG_ENDIAN;
			}
		}
		else if( column_name_size == 15 )
		{
			if( narrow_string_compare(
			     (char *) column_name,
			     "TimeMD5Changed",
			     14 ) == 0 )
			{
				known_column_type = WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME_BIG_ENDIAN;
			}
		}
		if( known_column_type == WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_FILETIME_BIG_ENDIAN )
		{
		}
		else if( known_column_type == WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_STRING_UTF16_LITTLE_ENDIAN )
		{
		}
		else if( known_column_type == WINDOWS_SEARCH_KNOWN_COLUMN_TYPE_UNDEFINED )
		{
			if( export_handle_export_record_value(
			     record,
			     value_iterator,
			     table_file_stream,
			     error ) != 1 )
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

/* Exports a filetime in a binary data table record value
 * Returns 1 if successful or -1 on error
 */
int windows_search_export_record_value_filetime(
     libesedb_record_t *record,
     int record_value_entry,
     uint8_t byte_order,
     FILE *table_file_stream,
     liberror_error_t **error )
{
	uint8_t *value_data         = NULL;
	static char *function       = "windows_search_export_record_value_filetime";
	size_t value_data_size      = 0;
	size_t value_string_size    = 0;
	double value_floating_point = 0.0;
	uint64_t value_64bit        = 0;
	uint32_t column_type        = 0;
	uint32_t value_32bit        = 0;
	uint16_t value_16bit        = 0;
	uint8_t value_8bit          = 0;
	int result                  = 0;

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
	uint8_t *value_data         = NULL;
	uint8_t *value_string       = NULL;
	static char *function       = "windows_search_export_record_value_utf16_string";
	size_t value_data_size      = 0;
	size_t value_string_size    = 0;
	double value_floating_point = 0.0;
	uint64_t value_64bit        = 0;
	uint32_t column_type        = 0;
	uint32_t value_32bit        = 0;
	uint16_t value_16bit        = 0;
	uint8_t value_8bit          = 0;
	int result                  = 0;

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
	switch( column_type )
	{
		case LIBESEDB_COLUMN_TYPE_BOOLEAN:
			result = libesedb_record_get_value_boolean(
				  record,
				  record_value_entry,
				  &value_8bit,
				  error );

			if( result == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve boolean value: %d.",
				 function,
				 record_value_entry + 1 );

				return( -1 );
			}
			else if( result != 0 )
			{
				if( value_8bit == 0 )
				{
					fprintf(
					 table_file_stream,
					 "false" );
				}
				else
				{
					fprintf(
					 table_file_stream,
					 "true" );
				}
			}
			break;

		case LIBESEDB_COLUMN_TYPE_INTEGER_8BIT_UNSIGNED:
			result = libesedb_record_get_value_8bit(
				  record,
				  record_value_entry,
				  &value_8bit,
				  error );

			if( result == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve 8-bit value: %d.",
				 function,
				 record_value_entry + 1 );

				return( -1 );
			}
			else if( result != 0 )
			{
				fprintf(
				 table_file_stream,
				 "%" PRIu8 "",
				 value_8bit );
			}
			break;

		case LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_SIGNED:
		case LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_UNSIGNED:
			result = libesedb_record_get_value_16bit(
				  record,
				  record_value_entry,
				  &value_16bit,
				  error );

			if( result == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve 16-bit value: %d.",
				 function,
				 record_value_entry + 1 );

				return( -1 );
			}
			else if( result != 0 )
			{
				if( column_type == LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_SIGNED )
				{
					fprintf(
					 table_file_stream,
					 "%" PRIi16 "",
					 (int16_t) value_16bit );
				}
				else
				{
					fprintf(
					 table_file_stream,
					 "%" PRIu16 "",
					 value_16bit );
				}
			}
			break;

		case LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_SIGNED:
		case LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_UNSIGNED:
			result = libesedb_record_get_value_32bit(
				  record,
				  record_value_entry,
				  &value_32bit,
				  error );

			if( result == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve 32-bit value: %d.",
				 function,
				 record_value_entry + 1 );

				return( -1 );
			}
			else if( result != 0 )
			{
				if( column_type == LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_SIGNED )
				{
					fprintf(
					 table_file_stream,
					 "%" PRIi32 "",
					 (int32_t) value_32bit );
				}
				else
				{
					fprintf(
					 table_file_stream,
					 "%" PRIu32 "",
					 value_32bit );
				}
			}
			break;

		case LIBESEDB_COLUMN_TYPE_INTEGER_64BIT_SIGNED:
			result = libesedb_record_get_value_64bit(
				  record,
				  record_value_entry,
				  &value_64bit,
				  error );

			if( result == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve 64-bit value: %d.",
				 function,
				 record_value_entry + 1 );

				return( -1 );
			}
			else if( result != 0 )
			{
				fprintf(
				 table_file_stream,
				 "%" PRIi64 "",
				 (int64_t) value_64bit );
			}
			break;

		case LIBESEDB_COLUMN_TYPE_FLOAT_32BIT:
		case LIBESEDB_COLUMN_TYPE_DOUBLE_64BIT:
			if( libesedb_record_get_value_floating_point(
			     record,
			     record_value_entry,
			     &value_floating_point,
			     error ) != 1 )

			if( result == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve floating point value: %d.",
				 function,
				 record_value_entry + 1 );

				return( -1 );
			}
			fprintf(
			 table_file_stream,
			 "%f",
			 value_floating_point );

			break;

		case LIBESEDB_COLUMN_TYPE_TEXT:
		case LIBESEDB_COLUMN_TYPE_LARGE_TEXT:
			result = libesedb_record_get_value_utf8_string_size(
				  record,
				  record_value_entry,
				  &value_string_size,
				  error );

			if( result == -1 )
			{
#ifdef TODO
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve size of value string: %d.",
				 function,
				 record_value_entry + 1 );

				return( -1 );
#else
				/* TODO some string values seem to contain binary data
				 * is this an unknown feature of the EDB format or
				 * is ESE not strict about the data in text column types?
				 */
				liberror_error_free(
				 error );

				if( libesedb_record_get_value(
				     record,
				     record_value_entry,
				     &value_data,
				     &value_data_size,
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
#endif
			}
			else if( result != 0 )
			{
				value_string = (uint8_t *) memory_allocate(
							    sizeof( uint8_t ) * value_string_size );

				if( value_string == NULL )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_MEMORY,
					 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create column name string.",
					 function );

					libsystem_file_stream_close(
					 table_file_stream );

					return( -1 );
				}
				if( libesedb_record_get_value_utf8_string(
				     record,
				     record_value_entry,
				     value_string,
				     value_string_size,
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
			break;

		default:
			if( libesedb_record_get_value(
			     record,
			     record_value_entry,
			     &value_data,
			     &value_data_size,
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
			break;
	}
	return( 1 );
}

