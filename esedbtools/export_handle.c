/* 
 * Export handle
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
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
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

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
#include "esedbcommon.h"
#include "windows_search.h"

/* Initializes the export handle
 * Returns 1 if successful or -1 on error
 */
int export_handle_initialize(
     export_handle_t **export_handle,
     liberror_error_t **error )
{
	static char *function = "export_handle_initialize";

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( *export_handle == NULL )
	{
		*export_handle = (export_handle_t *) memory_allocate(
		                                      sizeof( export_handle_t ) );

		if( *export_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create export handle.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *export_handle,
		     0,
		     sizeof( export_handle_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear export handle.",
			 function );

			memory_free(
			 *export_handle );

			*export_handle = NULL;

			return( -1 );
		}
		if( libesedb_file_initialize(
		     &( ( *export_handle )->input_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize input handle.",
			 function );

			memory_free(
			 *export_handle );

			*export_handle = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees the export handle and its elements
 * Returns 1 if successful or -1 on error
 */
int export_handle_free(
     export_handle_t **export_handle,
     liberror_error_t **error )
{
	static char *function = "export_handle_free";

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( *export_handle != NULL )
	{
		if( ( ( *export_handle )->input_handle != NULL )
		 && ( libesedb_file_free(
		       &( ( *export_handle )->input_handle ),
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free input handle.",
			 function );
		}
		memory_free(
		 *export_handle );

		*export_handle = NULL;
	}
	return( 1 );
}

/* Opens the export handle
 * Returns 1 if successful or -1 on error
 */
int export_handle_open(
     export_handle_t *export_handle,
     const libcstring_system_character_t *filename,
     liberror_error_t **error )
{
	static char *function = "export_handle_open";

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid export handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libesedb_file_open_wide(
	     export_handle->input_handle,
	     filename,
	     LIBESEDB_OPEN_READ,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		return( -1 );
	}
#else
	if( libesedb_file_open(
	     export_handle->input_handle,
	     filename,
	     LIBESEDB_OPEN_READ,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Closes the export handle
 * Returns the 0 if succesful or -1 on error
 */
int export_handle_close(
     export_handle_t *export_handle,
     liberror_error_t **error )
{
	static char *function = "export_handle_close";

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->input_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid export handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( libesedb_file_close(
	     export_handle->input_handle,
	     error ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close input handle.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Create a directory
 * Return 1 if successful or -1 on error
 */
int export_handle_make_directory(
     libcstring_system_character_t *directory_name,
     log_handle_t *log_handle,
     liberror_error_t **error )
{
	static char *function = "export_handle_make_directory";

	if( directory_name == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory name.",
		 function );

		return( -1 );
	}
	if( libsystem_directory_make(
	     directory_name ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to make directory: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 directory_name );

		return( -1 );
	}
	log_handle_printf(
	 log_handle,
	 "Created directory: %" PRIs_LIBCSTRING_SYSTEM ".\n",
	 directory_name );

	return( 1 );
}

/* Sanitizes the filename
 * Return 1 if successful or -1 on error
 */
int export_handle_sanitize_filename(
     libcstring_system_character_t *filename,
     size_t filename_size,
     liberror_error_t **error )
{
	static char *function = "export_handle_sanitize_filename";
	size_t iterator       = 0;

	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( filename_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid filename size value exceeds maximum.",
		 function );

		return( -1 );
	}
	for( iterator = 0; iterator < filename_size; iterator++ )
	{
		if( ( ( filename[ iterator ] >= 0x01 )
		  && ( filename[ iterator ] <= 0x1f ) )
		 || ( filename[ iterator ] == (libcstring_system_character_t) '!' )
		 || ( filename[ iterator ] == (libcstring_system_character_t) '$' )
		 || ( filename[ iterator ] == (libcstring_system_character_t) '%' )
		 || ( filename[ iterator ] == (libcstring_system_character_t) '&' )
		 || ( filename[ iterator ] == (libcstring_system_character_t) '*' )
		 || ( filename[ iterator ] == (libcstring_system_character_t) '+' )
		 || ( filename[ iterator ] == (libcstring_system_character_t) '/' )
		 || ( filename[ iterator ] == (libcstring_system_character_t) ':' )
		 || ( filename[ iterator ] == (libcstring_system_character_t) ';' )
		 || ( filename[ iterator ] == (libcstring_system_character_t) '<' )
		 || ( filename[ iterator ] == (libcstring_system_character_t) '>' )
		 || ( filename[ iterator ] == (libcstring_system_character_t) '?' )
		 || ( filename[ iterator ] == (libcstring_system_character_t) '@' )
		 || ( filename[ iterator ] == (libcstring_system_character_t) '\\' )
		 || ( filename[ iterator ] == (libcstring_system_character_t) '~' )
		 || ( filename[ iterator ] == 0x7e ) )
		{
			filename[ iterator ] = (libcstring_system_character_t) '_';
		}
	}
	return( 1 );
}

/* Creates the target path
 * Returns 1 if successful or -1 on error
 */
int export_handle_create_target_path(
     libcstring_system_character_t *export_path,
     size_t export_path_size,
     uint8_t *utf8_filename,
     size_t utf8_filename_size,
     libcstring_system_character_t **target_path,
     size_t *target_path_size,
     liberror_error_t **error )
{
	static char *function = "export_handle_create_target_path";
	size_t filename_size  = 0;

	if( export_path == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	if( utf8_filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 filename.",
		 function );

		return( -1 );
	}
	if( utf8_filename_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 filename size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( target_path == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid target path.",
		 function );

		return( -1 );
	}
	if( *target_path != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid target path already set.",
		 function );

		return( -1 );
	}
	if( target_path_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid target path size.",
		 function );

		return( -1 );
	}
	/* Make sure to check the UTF-8 filename length
	 * the conversion routines are very strict about the string size
	 */
	utf8_filename_size = 1 + libcstring_narrow_string_length(
	                          (char *) utf8_filename );

	if( libsystem_string_size_from_utf8_string(
	     utf8_filename,
	     utf8_filename_size,
	     &filename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine UTF-8 filename size.",
		 function );

		return( -1 );
	}
	/* Include space for the separator and the end of string character
	 */
	*target_path_size = export_path_size + filename_size;

	*target_path = (libcstring_system_character_t *) memory_allocate(
	                                                  sizeof( libcstring_system_character_t ) * *target_path_size );

	if( *target_path == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create target path.",
		 function );

		*target_path_size = 0;

		return( -1 );
	}
	if( libcstring_system_string_copy(
	     *target_path,
	     export_path,
	     export_path_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set export path in target path.",
		 function );

		memory_free(
		 target_path );

		*target_path      = NULL;
		*target_path_size = 0;

		return( -1 );
	}
	( *target_path )[ export_path_size - 1 ] = (libcstring_system_character_t) ESEDBCOMMON_PATH_SEPARATOR;

	if( libsystem_string_copy_from_utf8_string(
	     &( ( *target_path )[ export_path_size ] ),
	     filename_size,
	     utf8_filename,
	     utf8_filename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set filename in target path.",
		 function );

		memory_free(
		 target_path );

		*target_path      = NULL;
		*target_path_size = 0;

		return( -1 );
	}
	if( export_handle_sanitize_filename(
	     &( ( *target_path )[ export_path_size ] ),
	     filename_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable sanitize filename in target path.",
		 function );

		memory_free(
		 target_path );

		*target_path      = NULL;
		*target_path_size = 0;

		return( -1 );
	}
	return( 1 );
}

/* Exports the table
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_table(
     export_handle_t *export_handle,
     libesedb_table_t *table,
     libcstring_system_character_t *export_path,
     size_t export_path_size,
     log_handle_t *log_handle,
     liberror_error_t **error )
{
	libcstring_system_character_t *target_path = NULL;
	libesedb_column_t *column                  = NULL;
	libesedb_record_t *record                  = NULL;
	FILE *table_file_stream                    = NULL;
	uint8_t *table_name                        = NULL;
	uint8_t *value_string                      = NULL;
	static char *function                      = "export_handle_export_table";
	size_t target_path_size                    = 0;
	size_t table_name_size                     = 0;
	size_t value_string_size                   = 0;
	uint32_t table_identifier                  = 0;
	int column_iterator                        = 0;
	int known_table                            = 0;
	int number_of_columns                      = 0;
	int number_of_records                      = 0;
	int record_iterator                        = 0;
	int result                                 = 0;

	if( table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( export_path == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	if( libesedb_table_get_identifier(
	     table,
	     &table_identifier,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the table identifier.",
		 function );

		return( -1 );
	}
	if( libesedb_table_get_utf8_name_size(
	     table,
	     &table_name_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the size of the table name.",
		 function );

		return( -1 );
	}
	table_name = (uint8_t *) memory_allocate(
	                          sizeof( uint8_t ) * table_name_size );

	if( table_name == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create table name string.",
		 function );

		return( -1 );
	}
	if( libesedb_table_get_utf8_name(
	     table,
	     table_name,
	     table_name_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the table name.",
		 function );

		memory_free(
		 table_name );

		return( -1 );
	}
	/* Create the item value file
	 */
	if( export_handle_create_target_path(
	     export_path,
	     export_path_size,
	     table_name,
	     table_name_size,
	     &target_path,
	     &target_path_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create target path.",
		 function );

		memory_free(
		 table_name );

		return( -1 );
	}
	if( target_path == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid target path.",
		 function );

		memory_free(
		 table_name );

		return( -1 );
	}
	result = libsystem_file_exists(
	          target_path,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_GENERIC,
		 "%s: unable to determine if %" PRIs_LIBCSTRING_SYSTEM " exists.",
		 function,
		 target_path );

		memory_free(
		 target_path );
		memory_free(
		 table_name );

		return( -1 );
	}
	else if( result == 1 )
	{
		log_handle_printf(
		 log_handle,
		 "Skipping item values it already exists.\n" );

		memory_free(
		 target_path );
		memory_free(
		 table_name );

		return( 1 );
	}
	table_file_stream = libsystem_file_stream_open(
	                     target_path,
	                     _LIBCSTRING_SYSTEM_STRING( "w" ) );

	if( table_file_stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 target_path );

		memory_free(
		 target_path );
		memory_free(
		 table_name );

		return( -1 );
	}
	memory_free(
	 target_path );

	/* Write the column names to the table file
	 */
	if( libesedb_table_get_number_of_columns(
	     table,
	     &number_of_columns,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of columns.",
		 function );

		libsystem_file_stream_close(
		 table_file_stream );
		memory_free(
		 table_name );

		return( -1 );
	}
	for( column_iterator = 0;
	     column_iterator < number_of_columns;
	     column_iterator++ )
	{
		if( libesedb_table_get_column(
		     table,
		     column_iterator,
		     &column,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column: %d.",
			 function,
			 column_iterator );

			libsystem_file_stream_close(
			 table_file_stream );
			memory_free(
			 table_name );

			return( -1 );
		}
		if( libesedb_column_get_utf8_name_size(
		     column,
		     &value_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the size of the column name.",
			 function );

			libesedb_column_free(
			 &column,
			 NULL );
			libsystem_file_stream_close(
			 table_file_stream );
			memory_free(
			 table_name );

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
			 "%s: unable to create column name string.",
			 function );

			libesedb_column_free(
			 &column,
			 NULL );
			libsystem_file_stream_close(
			 table_file_stream );
			memory_free(
			 table_name );

			return( -1 );
		}
		if( libesedb_column_get_utf8_name(
		     column,
		     value_string,
		     value_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the column name.",
			 function );

			memory_free(
			 value_string );
			libesedb_column_free(
			 &column,
			 NULL );
			libsystem_file_stream_close(
			 table_file_stream );
			memory_free(
			 table_name );

			return( -1 );
		}
		fprintf(
		 table_file_stream,
		 "%s",
		 value_string );

		memory_free(
		 value_string );

		if( libesedb_column_free(
		     &column,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free column.",
			 function );

			libsystem_file_stream_close(
			 table_file_stream );
			memory_free(
			 table_name );

			return( -1 );
		}
		if( column_iterator == ( number_of_columns - 1 ) )
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
	/* Write the record (row) values to the table file
	 */
	if( libesedb_table_get_number_of_records(
	     table,
	     &number_of_records,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of records.",
		 function );

		libsystem_file_stream_close(
		 table_file_stream );
		memory_free(
		 table_name );

		return( -1 );
	}
	for( record_iterator = 0;
	     record_iterator < number_of_records;
	     record_iterator++ )
	{
		if( libesedb_table_get_record(
		     table,
		     record_iterator,
		     &record,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record: %d.",
			 function,
			 record_iterator );

			libsystem_file_stream_close(
			 table_file_stream );
			memory_free(
			 table_name );

			return( -1 );
		}
		known_table = 0;

		if( table_name_size == 15 )
		{
			if( libcstring_narrow_string_compare(
			     (char *) table_name,
			     "SystemIndex_0A",
			     14 ) == 0 )
			{
				known_table = 1;

				result = windows_search_export_record_systemindex_0a(
				          record,
				          table_file_stream,
				          error );
			}
		}
		else if( table_name_size == 17 )
		{
			if( libcstring_narrow_string_compare(
			     (char *) table_name,
			     "SystemIndex_Gthr",
			     16 ) == 0 )
			{
				known_table = 1;

				result = windows_search_export_record_systemindex_gthr(
				          record,
				          table_file_stream,
				          error );
			}
		}
		if( known_table == 0 )
		{
			result = export_handle_export_record(
			          record,
			          table_file_stream,
			          error );
		}
		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export record.",
			 function );

			libesedb_record_free(
			 &record,
			 NULL );
			libsystem_file_stream_close(
			 table_file_stream );
			memory_free(
			 table_name );

			return( -1 );
		}
		if( libesedb_record_free(
		     &record,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record.",
			 function );

			libsystem_file_stream_close(
			 table_file_stream );
			memory_free(
			 table_name );

			return( -1 );
		}
	}
	if( libsystem_file_stream_close(
	     table_file_stream ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close table file.",
		 function );

		memory_free(
		 table_name );

		return( -1 );
	}
	memory_free(
	 table_name );

	return( 1 );
}

/* Exports the values in a table record
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_record(
     libesedb_record_t *record,
     FILE *table_file_stream,
     liberror_error_t **error )
{
	static char *function = "export_handle_export_record";
	int number_of_values  = 0;
	int value_iterator    = 0;

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
			 value_iterator );

			return( -1 );
		}
		if( value_iterator == ( number_of_values - 1 ) )
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

/* Exports a table record value
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_record_value(
     libesedb_record_t *record,
     int record_value_entry,
     FILE *table_file_stream,
     liberror_error_t **error )
{
	uint8_t filetime_string[ 24 ];

        libesedb_long_value_t *long_value   = NULL;
        libesedb_multi_value_t *multi_value = NULL;
	libfdatetime_filetime_t *filetime   = NULL;
	uint8_t *value_data                 = NULL;
	uint8_t *value_string               = NULL;
	static char *function               = "export_handle_export_record_value";
	size_t value_data_size              = 0;
	size_t value_string_size            = 0;
	double value_floating_point         = 0.0;
	uint64_t value_64bit                = 0;
	uint32_t column_identifier          = 0;
	uint32_t column_type                = 0;
	uint32_t value_32bit                = 0;
	uint16_t value_16bit                = 0;
	uint8_t value_8bit                  = 0;
	uint8_t value_flags                 = 0;
	int long_value_segment_iterator     = 0;
	int multi_value_iterator            = 0;
	int number_of_long_value_segments   = 0;
	int number_of_multi_values          = 0;
	int result                          = 0;

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
	if( libesedb_record_get_column_identifier(
	     record,
	     record_value_entry,
	     &column_identifier,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column identifier of value: %d.",
		 function,
		 record_value_entry );

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
					 record_value_entry );

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
					 record_value_entry );

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
					 record_value_entry );

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
					 record_value_entry );

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

			case LIBESEDB_COLUMN_TYPE_CURRENCY:
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
					 record_value_entry );

					return( -1 );
				}
				else if( result != 0 )
				{
					if( column_type == LIBESEDB_COLUMN_TYPE_INTEGER_64BIT_SIGNED )
					{ 
						fprintf(
						 table_file_stream,
						 "%" PRIi64 "",
						 (int64_t) value_64bit );
					}
					else
					{
						fprintf(
						 table_file_stream,
						 "%" PRIu64 "",
						 value_64bit );
					}
				}
				break;

			case LIBESEDB_COLUMN_TYPE_DATE_TIME:
				result = libesedb_record_get_value_filetime(
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
					 "%s: unable to retrieve filetime value: %d.",
					 function,
					 record_value_entry );

					return( -1 );
				}
				else if( result != 0 )
				{
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
					if( libfdatetime_filetime_copy_from_uint64(
					     filetime,
					     value_64bit,
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
					     24,
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
					 record_value_entry );

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
#ifndef TEST
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve size of value string: %d (%" PRIu32 ").",
					 function,
					 record_value_entry,
					 column_identifier );

					return( -1 );
#else
					liberror_error_free(
					 error );

					if( value_data != NULL )
					{
						while( value_data_size > 0 )
						{
							fprintf(
							 stderr,
							 "%02" PRIx8 "",
							 *value_data );

							value_data      += 1;
							value_data_size -= 1;
						}
					}
#endif /* TEST */
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
						 "%s: unable to create value string.",
						 function );

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
						 record_value_entry );

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
	}
	else if( ( value_flags & LIBESEDB_VALUE_FLAG_COMPRESSED ) != 0 )
	{
		switch( column_type )
		{
			case LIBESEDB_COLUMN_TYPE_LARGE_TEXT:
				if( value_data != NULL )
				{
					value_string_size = 1 + ( ( ( value_data_size - 1 ) * 8 ) / 7 ) + 1;

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
					if( decompress_7bit_ascii(
					     value_string,
					     value_string_size,
					     value_data,
					     value_data_size,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to decompress 7-bit ASCII compressed value string.",
						 function );

						memory_free(
						 value_string );

						return( -1 );
					}
					value_string[ value_string_size - 1 ] = 0;

					fprintf(
					 table_file_stream,
					 "0x%02x:%s",
					 value_string[ 0 ],
					 &( value_string[ 1 ] ) );

					memory_free(
					 value_string );
				}
				break;

			default:
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
	}
	else if( ( value_flags & LIBESEDB_VALUE_FLAG_LONG_VALUE ) != 0 )
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
			 record_value_entry );

			return( -1 );
		}
		if( libesedb_long_value_get_number_of_segments(
		     long_value,
		     &number_of_long_value_segments,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of long value segments.",
			 function );

			libesedb_long_value_free(
			 &long_value,
			 NULL );

			return( -1 );
		}
		for( long_value_segment_iterator = 0;
	 	     long_value_segment_iterator < number_of_long_value_segments;
		     long_value_segment_iterator++ )
		{
			if( libesedb_long_value_get_segment_data(
			     long_value,
			     long_value_segment_iterator,
			     &value_data,
			     &value_data_size,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve long value segment: %d of record entry: %d.",
				 function,
				 long_value_segment_iterator,
				 record_value_entry );

				libesedb_long_value_free(
				 &long_value,
				 NULL );

				return( -1 );
			}
			if( value_data != NULL )
			{
#if defined( HAVE_DEBUG_OUTPUT )
libsystem_notify_printf(
 "LONG VALUE DATA: %d out of %d\n",
 long_value_segment_iterator + 1,
 number_of_long_value_segments );
libsystem_notify_print_data(
 value_data,
 value_data_size );
#endif
			}
		}
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
	/* TODO handle 0x10 flag */
	else if( ( ( value_flags & LIBESEDB_VALUE_FLAG_MULTI_VALUE ) != 0 )
	      && ( ( value_flags & 0x10 ) == 0 ) )
	{
		/* TODO what about non string multi values ?
		 */
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
			 record_value_entry );

			return( -1 );
		}
		if( libesedb_multi_value_get_number_of_values(
		     multi_value,
		     &number_of_multi_values,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of multi values.",
			 function );

			libesedb_multi_value_free(
			 &multi_value,
			 NULL );

			return( -1 );
		}
		for( multi_value_iterator = 0;
	 	     multi_value_iterator < number_of_multi_values;
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
				 "%s: unable to retrieve multi value: %d of record entry: %d.",
				 function,
				 multi_value_iterator,
				 record_value_entry );

				return( -1 );
			}
			if( value_data != NULL )
			{
				if( ( column_type == LIBESEDB_COLUMN_TYPE_TEXT )
				 || ( column_type == LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
				{
					result = libesedb_multi_value_get_value_utf8_string_size(
						  multi_value,
						  multi_value_iterator,
						  &value_string_size,
						  error );

					if( result == -1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve size of string of multi value: %d of record entry: %d (%" PRIu32 ").",
						 function,
						 multi_value_iterator,
						 record_value_entry,
						 column_identifier );

						libesedb_multi_value_free(
						 &multi_value,
						 NULL );

						return( -1 );
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
							 "%s: unable to create value string.",
							 function );

							libesedb_multi_value_free(
							 &multi_value,
							 NULL );

							return( -1 );
						}
						if( libesedb_multi_value_get_value_utf8_string(
						     multi_value,
						     multi_value_iterator,
						     value_string,
						     value_string_size,
						     error ) != 1 )
						{
							liberror_error_set(
							 error,
							 LIBERROR_ERROR_DOMAIN_RUNTIME,
							 LIBERROR_RUNTIME_ERROR_GET_FAILED,
							 "%s: unable to retrieve string of multi value: %d of record entry: %d.",
							 function,
							 multi_value_iterator,
							 record_value_entry );

							memory_free(
							 value_string );
							libesedb_multi_value_free(
							 &multi_value,
							 NULL );

							return( -1 );
						}
						fprintf(
						 table_file_stream,
						 "%s",
						 value_string );

						memory_free(
						 value_string );
					}
					if( multi_value_iterator < ( number_of_multi_values - 1 ) )
					{
						fprintf(
						 table_file_stream,
						 "; " );
					}
				}
				else
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
		}
		if( libesedb_multi_value_free(
		     &multi_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free multi value: %d.",
			 function,
			 multi_value_iterator );

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

/* Exports the items in the file
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_file(
     export_handle_t *export_handle,
     libcstring_system_character_t *export_path,
     size_t export_path_size,
     log_handle_t *log_handle,
     liberror_error_t **error )
{
	libesedb_table_t *table = NULL;
	static char *function   = "export_handle_export_file";
	int number_of_tables    = 0;
	int table_iterator      = 0;

	if( export_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_path == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	if( libsystem_directory_make(
	     export_path ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to make directory: %" PRIs_LIBCSTRING_SYSTEM "",
		 function,
		 export_path );

		return( -1 );
	}
	if( libesedb_file_get_number_of_tables(
	     export_handle->input_handle,
	     &number_of_tables,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of tables.",
		 function );

		return( -1 );
	}
	for( table_iterator = 0;
	     table_iterator < number_of_tables;
	     table_iterator++ )
	{
		fprintf(
		 stdout,
		 "Exporting table %d out of %d.\n",
		 table_iterator + 1,
		 number_of_tables );

		if( libesedb_file_get_table(
		     export_handle->input_handle,
		     table_iterator,
		     &table,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve table: %d.",
			 function,
			 table_iterator );

			return( -1 );
		}
		if( export_handle_export_table(
		     export_handle,
		     table,
		     export_path,
		     export_path_size,
		     log_handle,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export table: %d.",
			 function,
			 table_iterator );

			libesedb_table_free(
			 &table,
			 NULL );

			return( -1 );
		}
		if( libesedb_table_free(
		     &table,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free table: %d.",
			 function,
			 table_iterator );

			return( -1 );
		}
	}
	return( 1 );
}

/* Decompresses 7-bit ASCII compressed data
 * Returns 1 on success or -1 on error
 */
int decompress_7bit_ascii(
     uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     uint8_t *compressed_data, 
     size_t compressed_data_size,
     liberror_error_t **error )
{
	static char *function             = "decompress_7bit_ascii";
	size_t compressed_data_iterator   = 0;
	size_t uncompressed_data_iterator = 0;
	uint16_t value_16bit              = 0;
	uint8_t bit_index                 = 0;

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
	if( uncompressed_data_size < ( 1 + ( ( compressed_data_size - 1 ) * 8 ) / 7 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: uncompressed data size value too small.",
		 function );

		return( -1 );
	}
	uncompressed_data[ uncompressed_data_iterator++ ] = compressed_data[ 0 ];

	compressed_data_iterator = 1;

	for( compressed_data_iterator = 1;
	     compressed_data_iterator < compressed_data_size;
	     compressed_data_iterator++ )
	{
		value_16bit |= (uint16_t) compressed_data[ compressed_data_iterator ] << bit_index;

		uncompressed_data[ uncompressed_data_iterator++ ] = (uint8_t) ( value_16bit & 0x7f );

		value_16bit >>= 7;

		bit_index++;

		if( bit_index == 7 )
		{
			uncompressed_data[ uncompressed_data_iterator++ ] = value_16bit & 0x7f;

			value_16bit >>= 7;

			bit_index = 0;
		}
	}
	if( value_16bit != 0 )
	{
		uncompressed_data[ uncompressed_data_iterator++ ] = value_16bit & 0x7f;
	}
	return( 1 );
}

