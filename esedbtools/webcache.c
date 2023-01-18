/*
 * MSIE web cache database export functions
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
#include "esedbtools_libuna.h"
#include "export.h"
#include "export_handle.h"
#include "webcache.h"

enum WEBCACHE_KNOWN_COLUMN_TYPES
{
	WEBCACHE_KNOWN_COLUMN_TYPE_UNDEFINED,
	WEBCACHE_KNOWN_COLUMN_TYPE_FILETIME
};

/* Exports the values in a Container_ table record
 * Returns 1 if successful or -1 on error
 */
int webcache_export_record_container(
     libesedb_record_t *record,
     FILE *record_file_stream,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	system_character_t column_name[ 256 ];

	static char *function   = "webcache_export_record_container";
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
		known_column_type = WEBCACHE_KNOWN_COLUMN_TYPE_UNDEFINED;

		if( column_type == LIBESEDB_COLUMN_TYPE_INTEGER_64BIT_SIGNED )
		{
			if( column_name_size == 9 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "SyncTime" ),
				     8 ) == 0 )
				{
					known_column_type = WEBCACHE_KNOWN_COLUMN_TYPE_FILETIME;
				}
			}
			else if( column_name_size == 11 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "ExpiryTime" ),
				     10 ) == 0 )
				{
					known_column_type = WEBCACHE_KNOWN_COLUMN_TYPE_FILETIME;
				}
			}
			else if( column_name_size == 13 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "AccessedTime" ),
				     12 ) == 0 )
				{
					known_column_type = WEBCACHE_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( system_string_compare(
				          column_name,
				        _SYSTEM_STRING( "CreationTime" ),
				        12 ) == 0 )
				{
					known_column_type = WEBCACHE_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( system_string_compare(
				        column_name,
				        _SYSTEM_STRING( "ModifiedTime" ),
				        12 ) == 0 )
				{
					known_column_type = WEBCACHE_KNOWN_COLUMN_TYPE_FILETIME;
				}
			}
		}
		if( known_column_type == WEBCACHE_KNOWN_COLUMN_TYPE_FILETIME )
		{
			result = export_filetime(
				  record,
				  value_iterator,
				  byte_order,
				  record_file_stream,
				  error );
		}
		else if( known_column_type == WEBCACHE_KNOWN_COLUMN_TYPE_UNDEFINED )
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

/* Exports the values in a Containers table record
 * Returns 1 if successful or -1 on error
 */
int webcache_export_record_containers(
     libesedb_record_t *record,
     FILE *record_file_stream,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	system_character_t column_name[ 256 ];

	static char *function   = "webcache_export_record_containers";
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
		known_column_type = WEBCACHE_KNOWN_COLUMN_TYPE_UNDEFINED;

		if( column_type == LIBESEDB_COLUMN_TYPE_INTEGER_64BIT_SIGNED )
		{
			if( column_name_size == 15 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "LastAccessTime" ),
				     14 ) == 0 )
				{
					known_column_type = WEBCACHE_KNOWN_COLUMN_TYPE_FILETIME;
				}
				else if( system_string_compare(
				          column_name,
				          _SYSTEM_STRING( "RequestHeaders" ),
				          14 ) == 0 )
				{
/* TODO */
				}
			}
			else if( column_name_size == 16 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "ResponseHeaders" ),
				     15 ) == 0 )
				{
/* TODO contains property sheets for History table */
/* TODO contains an ASCII string for the Content table */
				}
			}
			else if( column_name_size == 17 )
			{
				if( system_string_compare(
				     column_name,
				     _SYSTEM_STRING( "LastScavengeTime" ),
				     16 ) == 0 )
				{
					known_column_type = WEBCACHE_KNOWN_COLUMN_TYPE_FILETIME;
				}
			}
		}
		if( known_column_type == WEBCACHE_KNOWN_COLUMN_TYPE_FILETIME )
		{
			result = export_filetime(
				  record,
				  value_iterator,
				  byte_order,
				  record_file_stream,
				  error );
		}
		else if( known_column_type == WEBCACHE_KNOWN_COLUMN_TYPE_UNDEFINED )
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

