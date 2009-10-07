/*
 * Shows information obtained from an Extensible Storage Engine (ESE) Database (EDB) file
 *
 * Copyright (c) 2008-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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
#include <memory.h>
#include <types.h>

#include <liberror.h>

#include <stdio.h>

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

/* If libtool DLL support is enabled set LIBESEDB_DLL_IMPORT
 * before including libesedb.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBESEDB_DLL_IMPORT
#endif

#include <libesedb.h>

#include <libsystem.h>

#include "esedboutput.h"

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use esedbinfo to determine information about an Extensible Storage Engine (ESE) Database File (EDB).\n\n" );

	fprintf( stream, "Usage: esedbinfo [ -hvV ] source\n\n" );

	fprintf( stream, "\tsource: the source file\n\n" );

	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
}

/* Retrieve a description of the column type
 */
const char *esedbinfo_get_column_type_description(
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

/* Prints file information
 * Returns 1 if successful or -1 on error
 */
int esedbinfo_file_info_fprint(
     FILE *stream,
     libesedb_file_t *file,
     libesedb_error_t **error )
{
	libesedb_column_t *column  = NULL;
	libesedb_index_t *index    = NULL;
	libesedb_table_t *table    = NULL;
	uint8_t *value_string      = NULL;
	static char *function      = "esedbinfo_file_info_fprint";
	size_t value_string_size   = 0;
	uint32_t format_revision   = 0;
	uint32_t format_version    = 0;
	uint32_t column_identifier = 0;
	uint32_t column_type       = 0;
	uint32_t index_identifier  = 0;
	uint32_t table_identifier  = 0;
	uint32_t page_size         = 0;
	int amount_of_columns      = 0;
	int amount_of_indexes      = 0;
	int amount_of_tables       = 0;
	int column_iterator        = 0;
	int index_iterator         = 0;
	int table_iterator         = 0;

	if( stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	fprintf(
	 stream,
	 "Extensible Storage Engine Database information:\n" );

	if( libesedb_file_get_creation_format_version(
	     file,
	     &format_version,
	     &format_revision,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve creation format version.",
		 function );

		return( -1 );
	}
	fprintf(
	 stream,
	 "\tCreated in format:\t0x%" PRIx32 ",%" PRIu32 "\n",
	 format_version,
	 format_revision );

	if( libesedb_file_get_format_version(
	     file,
	     &format_version,
	     &format_revision,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve format version.",
		 function );

		return( -1 );
	}
	fprintf(
	 stream,
	 "\tCurrent format:\t\t0x%" PRIx32 ",%" PRIu32 "\n",
	 format_version,
	 format_revision );

	if( libesedb_file_get_page_size(
	     file,
	     &page_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page size.",
		 function );

		return( -1 );
	}
	fprintf(
	 stream,
	 "\tPage size:\t\t%" PRIu32 " bytes\n",
	 page_size );

	fprintf(
	 stream,
	 "\n" );

	fprintf(
	 stream,
	 "Catalog content:\n" );

	if( libesedb_file_get_amount_of_tables(
	     file,
	     &amount_of_tables,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of tables.",
		 function );

		return( -1 );
	}
	fprintf(
	 stream,
	 "\tAmount of tables:\t%d\n",
	 amount_of_tables );

	fprintf(
	 stream,
	 "\n" );

	for( table_iterator = 0;
	     table_iterator < amount_of_tables;
	     table_iterator++ )
	{
		if( libesedb_file_get_table(
		     file,
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
			 table_iterator + 1 );

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

			libesedb_table_free(
			 &table,
			 NULL );

			return( -1 );
		}
		if( libesedb_table_get_utf8_name_size(
		     table,
		     &value_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the size of the table name.",
			 function );

			libesedb_table_free(
			 &table,
			 NULL );

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
			 "%s: unable to create table name string.",
			 function );

			libesedb_table_free(
			 &table,
			 NULL );

			return( -1 );
		}
		if( libesedb_table_get_utf8_name(
		     table,
		     value_string,
		     value_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the table name.",
			 function );

			memory_free(
			 value_string );
			libesedb_table_free(
			 &table,
			 NULL );

			return( -1 );
		}
		fprintf(
		 stream,
		 "Table: %d\t\t\t%s (%d)\n",
		 table_iterator + 1,
		 value_string,
		 table_identifier );

		memory_free(
		 value_string );

		/* Print the columns of the table
		 */
		if( libesedb_table_get_amount_of_columns(
		     table,
		     &amount_of_columns,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve amount of columns.",
			 function );

			libesedb_table_free(
			 &table,
			 NULL );

			return( -1 );
		}
		fprintf(
		 stream,
		 "\tAmount of columns:\t%d\n",
		 amount_of_columns );
		fprintf(
		 stream,
		 "\tColumn\tIdentifier\tName\tType\n" );

		for( column_iterator = 0;
		     column_iterator < amount_of_columns;
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
				 column_iterator + 1 );

				libesedb_table_free(
				 &table,
				 NULL );

				return( -1 );
			}
			if( libesedb_column_get_identifier(
			     column,
			     &column_identifier,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the column identifier.",
				 function );

				libesedb_column_free(
				 &column,
				 NULL );
				libesedb_table_free(
				 &table,
				 NULL );

				return( -1 );
			}
			if( libesedb_column_get_type(
			     column,
			     &column_type,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the column type.",
				 function );

				libesedb_column_free(
				 &column,
				 NULL );
				libesedb_table_free(
				 &table,
				 NULL );

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
				libesedb_table_free(
				 &table,
				 NULL );

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
				libesedb_table_free(
				 &table,
				 NULL );

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
				libesedb_table_free(
				 &table,
				 NULL );

				return( -1 );
			}
			fprintf(
			 stream,
			 "\t%d\t%" PRIu32 "\t%s\t%s\n",
			 column_iterator + 1,
			 column_identifier,
			 value_string,
			 esedbinfo_get_column_type_description(
			  column_type ) );

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

				libesedb_table_free(
				 &table,
				 NULL );

				return( -1 );
			}
		}
		/* Print the indexes of the table
		 */
		if( libesedb_table_get_amount_of_indexes(
		     table,
		     &amount_of_indexes,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve amount of indexes.",
			 function );

			libesedb_table_free(
			 &table,
			 NULL );

			return( -1 );
		}
		fprintf(
		 stream,
		 "\n" );
		fprintf(
		 stream,
		 "\tAmount of indexes:\t%d\n",
		 amount_of_indexes );

		for( index_iterator = 0;
		     index_iterator < amount_of_indexes;
		     index_iterator++ )
		{
			if( libesedb_table_get_index(
			     table,
			     index_iterator,
			     &index,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve index: %d.",
				 function,
				 index_iterator + 1 );

				libesedb_table_free(
				 &table,
				 NULL );

				return( -1 );
			}
			if( libesedb_index_get_identifier(
			     index,
			     &index_identifier,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the index identifier.",
				 function );

				libesedb_index_free(
				 &index,
				 NULL );
				libesedb_table_free(
				 &table,
				 NULL );

				return( -1 );
			}
			if( libesedb_index_get_utf8_name_size(
			     index,
			     &value_string_size,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the size of the index name.",
				 function );

				libesedb_index_free(
				 &index,
				 NULL );
				libesedb_table_free(
				 &table,
				 NULL );

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
				 "%s: unable to create index name string.",
				 function );

				libesedb_index_free(
				 &index,
				 NULL );
				libesedb_table_free(
				 &table,
				 NULL );

				return( -1 );
			}
			if( libesedb_index_get_utf8_name(
			     index,
			     value_string,
			     value_string_size,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the index name.",
				 function );

				memory_free(
				 value_string );
				libesedb_index_free(
				 &index,
				 NULL );
				libesedb_table_free(
				 &table,
				 NULL );

				return( -1 );
			}
			fprintf(
			 stream,
			 "\tIndex: %d\t\t%s (%" PRIu32 ")\n",
			 index_iterator + 1,
			 value_string,
			 index_identifier );

			memory_free(
			 value_string );

			if( libesedb_index_free(
			     &index,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free index.",
				 function );

				libesedb_table_free(
				 &table,
				 NULL );

				return( -1 );
			}
		}
		if( libesedb_table_free(
		     &table,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free table.",
			 function );

			return( -1 );
		}
		fprintf(
		 stream,
		 "\n" );
	}
	fprintf(
	 stream,
	 "\n" );

	return( 1 );
}

/* The main program
 */
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libesedb_error_t *error       = NULL;
	libesedb_file_t *esedb_file   = NULL;
	libsystem_character_t *source = NULL;
	char *program                 = "esedbinfo";
	libsystem_integer_t option    = 0;
	int verbose                   = 0;

	libsystem_notify_set_stream(
	 stderr,
	 NULL );
	libsystem_notify_set_verbose(
	 1 );

        if( libsystem_initialize(
             &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize system values.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	esedboutput_version_fprint(
	 stdout,
	 program );

	while( ( option = libsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBSYSTEM_CHARACTER_T_STRING( "hvV" ) ) ) != (libsystem_integer_t) -1 )
	{
		switch( option )
		{
			case (libsystem_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %s\n",
				 argv[ optind ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (libsystem_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libsystem_integer_t) 'v':
				verbose = 1;

				break;

			case (libsystem_integer_t) 'V':
				esedboutput_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	source = argv[ optind ];

	libsystem_notify_set_verbose(
	 verbose );
	libesedb_notify_set_stream(
	 stderr,
	 NULL );
	libesedb_notify_set_verbose(
	 verbose );

	if( libesedb_file_initialize(
	     &esedb_file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize libesedb file.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		libesedb_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
	if( libesedb_file_open_wide(
	     esedb_file,
	     source,
	     LIBESEDB_OPEN_READ,
	     &error ) != 1 )
#else
	if( libesedb_file_open(
	     esedb_file,
	     source,
	     LIBESEDB_OPEN_READ,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Error opening file: %" PRIs_LIBSYSTEM ".\n",
		 argv[ optind ] );

		libsystem_notify_print_error_backtrace(
		 error );
		libesedb_error_free(
		 &error );

		libesedb_file_free(
		 &esedb_file,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( esedbinfo_file_info_fprint(
	     stdout,
	     esedb_file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to print file information.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		libesedb_error_free(
		 &error );

		libesedb_file_free(
		 &esedb_file,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( libesedb_file_close(
	     esedb_file,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Error closing file: %" PRIs_LIBSYSTEM ".\n",
		 argv[ optind ] );

		libsystem_notify_print_error_backtrace(
		 error );
		libesedb_error_free(
		 &error );

		libesedb_file_free(
		 &esedb_file,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( libesedb_file_free(
	     &esedb_file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free libesedb file.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		libesedb_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}

