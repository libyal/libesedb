/*
 * Info handle
 *
 * Copyright (C) 2009-2022, Joachim Metz <joachim.metz@gmail.com>
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
#include <file_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "esedbtools_libcerror.h"
#include "esedbtools_libclocale.h"
#include "esedbtools_libesedb.h"
#include "esedbtools_libfdatetime.h"
#include "info_handle.h"

#define INFO_HANDLE_NOTIFY_STREAM	stdout

/* Retrieve a description of the column type
 */
const char *info_handle_get_column_type_description(
             uint32_t column_type )
{
	switch( column_type )
	{
		case LIBESEDB_COLUMN_TYPE_NULL:
			return( "NULL" );

		case LIBESEDB_COLUMN_TYPE_BOOLEAN:
			return( "Boolean" );

		case LIBESEDB_COLUMN_TYPE_INTEGER_8BIT_UNSIGNED:
			return( "Integer 8-bit unsigned" );

		case LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_SIGNED:
			return( "Integer 16-bit signed" );

		case LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_SIGNED:
			return( "Integer 32-bit signed" );

		case LIBESEDB_COLUMN_TYPE_CURRENCY:
			return( "Currency (64-bit)" );

		case LIBESEDB_COLUMN_TYPE_FLOAT_32BIT:
			return( "Floating point single precision (32-bit)" );

		case LIBESEDB_COLUMN_TYPE_DOUBLE_64BIT:
			return( "Floating point double precision (64-bit)" );

		case LIBESEDB_COLUMN_TYPE_DATE_TIME:
			return( "Date and time" );

		case LIBESEDB_COLUMN_TYPE_BINARY_DATA:
			return( "Binary data" );

		case LIBESEDB_COLUMN_TYPE_TEXT:
			return( "Text" );

		case LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA:
			return( "Large binary data" );

		case LIBESEDB_COLUMN_TYPE_LARGE_TEXT:
			return( "Large text" );

		case LIBESEDB_COLUMN_TYPE_SUPER_LARGE_VALUE:
			return( "Super large value" );

		case LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_UNSIGNED:
			return( "Integer 32-bit unsigned" );

		case LIBESEDB_COLUMN_TYPE_INTEGER_64BIT_SIGNED:
			return( "Integer 64-bit signed" );

		case LIBESEDB_COLUMN_TYPE_GUID:
			return( "GUID" );

		case LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_UNSIGNED:
			return( "Integer 16-bit unsigned" );

		default:
			return( "Unknown" );
	}
}

/* Creates an info handle
 * Make sure the value info_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int info_handle_initialize(
     info_handle_t **info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_initialize";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle value already set.",
		 function );

		return( -1 );
	}
	*info_handle = memory_allocate_structure(
	                info_handle_t );

	if( *info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create info handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *info_handle,
	     0,
	     sizeof( info_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear info handle.",
		 function );

		goto on_error;
	}
	if( libesedb_file_initialize(
	     &( ( *info_handle )->input_file ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize input file.",
		 function );

		goto on_error;
	}
	( *info_handle )->notify_stream = INFO_HANDLE_NOTIFY_STREAM;

	return( 1 );

on_error:
	if( *info_handle != NULL )
	{
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( -1 );
}

/* Frees an info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_free(
     info_handle_t **info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_free";
	int result            = 1;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		if( libesedb_file_free(
		     &( ( *info_handle )->input_file ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free input file.",
			 function );

			result = -1;
		}
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( result );
}

/* Signals the info handle to abort
 * Returns 1 if successful or -1 on error
 */
int info_handle_signal_abort(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_signal_abort";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	info_handle->abort = 1;

	if( info_handle->input_file != NULL )
	{
		if( libesedb_file_signal_abort(
		     info_handle->input_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal input file to abort.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Sets the ascii codepage
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_ascii_codepage(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function  = "info_handle_set_ascii_codepage";
	size_t string_length   = 0;
	uint32_t feature_flags = 0;
	int result             = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	feature_flags = LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_KOI8
	              | LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_WINDOWS;

	string_length = system_string_length(
	                 string );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libclocale_codepage_copy_from_string_wide(
	          &( info_handle->ascii_codepage ),
	          string,
	          string_length,
	          feature_flags,
	          error );
#else
	result = libclocale_codepage_copy_from_string(
	          &( info_handle->ascii_codepage ),
	          string,
	          string_length,
	          feature_flags,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine ASCII codepage.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Opens the info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_open(
     info_handle_t *info_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function = "info_handle_open";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libesedb_file_open_wide(
	     info_handle->input_file,
	     filename,
	     LIBESEDB_OPEN_READ,
	     error ) != 1 )
#else
	if( libesedb_file_open(
	     info_handle->input_file,
	     filename,
	     LIBESEDB_OPEN_READ,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open input file.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Closes the info handle
 * Returns the 0 if succesful or -1 on error
 */
int info_handle_close(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_close";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libesedb_file_close(
	     info_handle->input_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close input file.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Prints the column information to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_column_fprint(
     info_handle_t *info_handle,
     int column_iterator,
     libesedb_column_t *column,
     libcerror_error_t **error )
{
	system_character_t *value_string = NULL;
	static char *function            = "info_handle_column_fprint";
	size_t value_string_size         = 0;
	uint32_t column_identifier       = 0;
	uint32_t column_type             = 0;
	int result                       = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libesedb_column_get_identifier(
	     column,
	     &column_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the column identifier.",
		 function );

		goto on_error;
	}
	if( libesedb_column_get_type(
	     column,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the column type.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libesedb_column_get_utf16_name_size(
	          column,
	          &value_string_size,
	          error );
#else
	result = libesedb_column_get_utf8_name_size(
	          column,
	          &value_string_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the size of the column name.",
		 function );

		goto on_error;
	}
	if( value_string_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing column name.",
		 function );

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
		 "%s: unable to create column name string.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libesedb_column_get_utf16_name(
	          column,
	          (uint16_t *) value_string,
	          value_string_size,
	          error );
#else
	result = libesedb_column_get_utf8_name(
	          column,
	          (uint8_t *) value_string,
	          value_string_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the column name.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\t%d\t%" PRIu32 "\t%" PRIs_SYSTEM "\t%s\n",
	 column_iterator + 1,
	 column_identifier,
	 value_string,
	 info_handle_get_column_type_description(
	  column_type ) );

	memory_free(
	 value_string );

	value_string = NULL;

	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

/* Prints the index information to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_index_fprint(
     info_handle_t *info_handle,
     int index_iterator,
     libesedb_index_t *index,
     libcerror_error_t **error )
{
	system_character_t *value_string = NULL;
	static char *function            = "info_handle_index_fprint";
	size_t value_string_size         = 0;
	uint32_t index_identifier        = 0;
	int result                       = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libesedb_index_get_identifier(
	     index,
	     &index_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the index identifier.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libesedb_index_get_utf16_name_size(
		  index,
		  &value_string_size,
		  error );
#else
	result = libesedb_index_get_utf8_name_size(
		  index,
		  &value_string_size,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the size of the index name.",
		 function );

		goto on_error;
	}
	if( value_string_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing index name.",
		 function );

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
		 "%s: unable to create index name string.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libesedb_index_get_utf16_name(
		  index,
		  (uint16_t *) value_string,
		  value_string_size,
		  error );
#else
	result = libesedb_index_get_utf8_name(
		  index,
		  (uint8_t *) value_string,
		  value_string_size,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the index name.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tIndex: %d\t\t%" PRIs_SYSTEM " (%" PRIu32 ")\n",
	 index_iterator + 1,
	 value_string,
	 index_identifier );

	memory_free(
	 value_string );

	value_string = NULL;

	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

/* Prints the table information to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_table_fprint(
     info_handle_t *info_handle,
     int table_iterator,
     libesedb_table_t *table,
     libcerror_error_t **error )
{
	libesedb_column_t *column        = NULL;
	libesedb_index_t *index          = NULL;
	system_character_t *value_string = NULL;
	static char *function            = "info_handle_table_fprint";
	size_t value_string_size         = 0;
	uint32_t index_identifier        = 0;
	uint32_t table_identifier        = 0;
	int column_iterator              = 0;
	int index_iterator               = 0;
	int number_of_columns            = 0;
	int number_of_indexes            = 0;
	int result                       = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libesedb_table_get_identifier(
	     table,
	     &table_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the table identifier.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libesedb_table_get_utf16_name_size(
	          table,
	          &value_string_size,
	          error );
#else
	result = libesedb_table_get_utf8_name_size(
	          table,
	          &value_string_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the size of the table name.",
		 function );

		goto on_error;
	}
	if( value_string_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing table name.",
		 function );

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
		 "%s: unable to create table name string.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libesedb_table_get_utf16_name(
	          table,
	          (uint16_t *) value_string,
	          value_string_size,
	          error );
#else
	result = libesedb_table_get_utf8_name(
	          table,
	          (uint8_t *) value_string,
	          value_string_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the table name.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "Table: %d\t\t\t%" PRIs_SYSTEM " (%d)\n",
	 table_iterator + 1,
	 value_string,
	 table_identifier );

	memory_free(
	 value_string );

	value_string = NULL;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libesedb_table_get_utf16_template_name_size(
		  table,
		  &value_string_size,
		  error );
#else
	result = libesedb_table_get_utf8_template_name_size(
		  table,
		  &value_string_size,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the size of the table template name.",
		 function );

		goto on_error;
	}
	if( value_string_size > 0 )
	{
		value_string = system_string_allocate(
				value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create template name string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libesedb_table_get_utf16_template_name(
			  table,
			  (uint16_t *) value_string,
			  value_string_size,
			  error );
#else
		result = libesedb_table_get_utf8_template_name(
			  table,
			  (uint8_t *) value_string,
			  value_string_size,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the template name.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tTemplate:\t\t%s\n",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	/* Print the columns of the table
	 */
	if( libesedb_table_get_number_of_columns(
	     table,
	     &number_of_columns,
	     LIBESEDB_GET_COLUMN_FLAG_IGNORE_TEMPLATE_TABLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of columns.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tNumber of columns:\t%d\n",
	 number_of_columns );
	fprintf(
	 info_handle->notify_stream,
	 "\tColumn\tIdentifier\tName\tType\n" );

	for( column_iterator = 0;
	     column_iterator < number_of_columns;
	     column_iterator++ )
	{
		if( libesedb_table_get_column(
		     table,
		     column_iterator,
		     &column,
		     LIBESEDB_GET_COLUMN_FLAG_IGNORE_TEMPLATE_TABLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column: %d.",
			 function,
			 column_iterator + 1 );

			goto on_error;
		}
		if( info_handle_column_fprint(
		     info_handle,
		     column_iterator,
		     column,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print column: %d.",
			 function,
			 column_iterator + 1 );

			goto on_error;
		}
		if( libesedb_column_free(
		     &column,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free column.",
			 function );

			goto on_error;
		}
	}
	/* Print the indexes of the table
	 */
	if( libesedb_table_get_number_of_indexes(
	     table,
	     &number_of_indexes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of indexes.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );
	fprintf(
	 info_handle->notify_stream,
	 "\tNumber of indexes:\t%d\n",
	 number_of_indexes );

	for( index_iterator = 0;
	     index_iterator < number_of_indexes;
	     index_iterator++ )
	{
		if( libesedb_table_get_index(
		     table,
		     index_iterator,
		     &index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve index: %d.",
			 function,
			 index_iterator + 1 );

			goto on_error;
		}
		if( info_handle_index_fprint(
		     info_handle,
		     index_iterator,
		     index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print index: %d.",
			 function,
			 index_iterator + 1 );

			goto on_error;
		}
		if( libesedb_index_free(
		     &index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free index.",
			 function );

			goto on_error;
		}
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

/* TODO refactor */
	for( index_iterator = 0;
	     index_iterator < number_of_indexes;
	     index_iterator++ )
	{
		if( libesedb_table_get_index(
		     table,
		     index_iterator,
		     &index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve index: %d.",
			 function,
			 index_iterator + 1 );

			goto on_error;
		}
		if( libesedb_index_get_identifier(
		     index,
		     &index_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the index identifier.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libesedb_index_get_utf16_name_size(
			  index,
			  &value_string_size,
			  error );
#else
		result = libesedb_index_get_utf8_name_size(
			  index,
			  &value_string_size,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the size of the index name.",
			 function );

			goto on_error;
		}
		if( value_string_size == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing index name.",
			 function );

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
			 "%s: unable to create index name string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libesedb_index_get_utf16_name(
			  index,
			  (uint16_t *) value_string,
			  value_string_size,
			  error );
#else
		result = libesedb_index_get_utf8_name(
			  index,
			  (uint8_t *) value_string,
			  value_string_size,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the index name.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "Index: %d\t\t\t%" PRIs_SYSTEM " (%d)\n",
		 index_iterator + 1,
		 value_string,
		 index_identifier );

		memory_free(
		 value_string );

		value_string = NULL;

		/* TODO print index columns */

		if( libesedb_index_free(
		     &index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free index.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\n" );
	}
	return( 1 );

on_error:
	if( index != NULL )
	{
		libesedb_index_free(
		 &index,
		 NULL );
	}
	if( column != NULL )
	{
		libesedb_column_free(
		 &column,
		 NULL );
	}
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

/* Prints the file information to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_file_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	libesedb_table_t *table  = NULL;
	static char *function    = "esedbinfo_file_info_fprint";
	uint32_t file_type       = 0;
	uint32_t format_revision = 0;
	uint32_t format_version  = 0;
	uint32_t page_size       = 0;
	int number_of_tables     = 0;
	int table_iterator       = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "Extensible Storage Engine Database information:\n" );

	if( libesedb_file_get_type(
	     info_handle->input_file,
	     &file_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file type.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tFile type:\t\t" );

	if( file_type == LIBESEDB_FILE_TYPE_DATABASE )
	{
		fprintf(
		 info_handle->notify_stream,
		 "Database" );
	}
	else if( file_type == LIBESEDB_FILE_TYPE_STREAMING_FILE )
	{
		fprintf(
		 info_handle->notify_stream,
		 "Streaming file" );
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 "Unknown" );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( libesedb_file_get_creation_format_version(
	     info_handle->input_file,
	     &format_version,
	     &format_revision,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve creation format version.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tCreated in format:\t0x%" PRIx32 ",%" PRIu32 "\n",
	 format_version,
	 format_revision );

	if( libesedb_file_get_format_version(
	     info_handle->input_file,
	     &format_version,
	     &format_revision,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve format version.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tCurrent format:\t\t0x%" PRIx32 ",%" PRIu32 "\n",
	 format_version,
	 format_revision );

	if( libesedb_file_get_page_size(
	     info_handle->input_file,
	     &page_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page size.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tPage size:\t\t%" PRIu32 " bytes\n",
	 page_size );

	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( file_type == LIBESEDB_FILE_TYPE_DATABASE )
	{
		fprintf(
		 info_handle->notify_stream,
		 "Catalog content:\n" );

		if( libesedb_file_get_number_of_tables(
		     info_handle->input_file,
		     &number_of_tables,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of tables.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tNumber of tables:\t%d\n",
		 number_of_tables );

		fprintf(
		 info_handle->notify_stream,
		 "\n" );

		for( table_iterator = 0;
		     table_iterator < number_of_tables;
		     table_iterator++ )
		{
			if( libesedb_file_get_table(
			     info_handle->input_file,
			     table_iterator,
			     &table,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve table: %d.",
				 function,
				 table_iterator + 1 );

				goto on_error;
			}
			if( info_handle_table_fprint(
			     info_handle,
			     table_iterator,
			     table,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print table: %d.",
				 function,
				 table_iterator + 1 );

				goto on_error;
			}
			if( libesedb_table_free(
			     &table,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free table.",
				 function );

				goto on_error;
			}
		}
		fprintf(
		 info_handle->notify_stream,
		 "\n" );
	}
	return( 1 );

on_error:
	if( table != NULL )
	{
		libesedb_table_free(
		 &table,
		 NULL );
	}
	return( -1 );
}

