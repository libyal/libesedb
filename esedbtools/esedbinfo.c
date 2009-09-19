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

#include "filetime.h"
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

/* Prints file information
 * Returns 1 if successful or -1 on error
 */
int esedbinfo_file_info_fprint(
     FILE *stream,
     libesedb_file_t *file,
     libesedb_error_t **error )
{
	libesedb_index_t *index  = NULL;
	libesedb_table_t *table  = NULL;
	uint8_t *value_string    = NULL;
	static char *function    = "esedbinfo_file_info_fprint";
	size_t value_string_size = 0;
	uint32_t format_revision = 0;
	uint32_t format_version  = 0;
	uint32_t item_identifier = 0;
	uint32_t page_size       = 0;
	int amount_of_items      = 0;
	int item_iterator        = 0;

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
	     &amount_of_items,
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
	 amount_of_items );

	for( item_iterator = 0;
	     item_iterator < amount_of_items;
	     item_iterator++ )
	{
		if( libesedb_file_get_table(
		     file,
		     item_iterator,
		     &table,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve table: %d.",
			 function,
			 item_iterator + 1 );

			return( -1 );
		}
		if( libesedb_table_get_identifier(
		     table,
		     &item_identifier,
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
		 "\t(%" PRIu32 ")\t%s\n",
		 item_identifier,
		 value_string );

		memory_free(
		 value_string );

#if defined( HAVE_DEBUG_OUTPUT )
		if( item_identifier == 14 )
		{
			if( libesedb_table_test(
			     table,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read table.",
				 function );

				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libsystem_notify_print_error_backtrace(
					 *error );
				}
				liberror_error_free(
				 error );
			}
		}
#endif
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
	}
	fprintf(
	 stream,
	 "\n" );

	if( libesedb_file_get_amount_of_indexes(
	     file,
	     &amount_of_items,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of indexes.",
		 function );

		return( -1 );
	}
	fprintf(
	 stream,
	 "\tAmount of indexes:\t%d\n",
	 amount_of_items );

	for( item_iterator = 0;
	     item_iterator < amount_of_items;
	     item_iterator++ )
	{
		if( libesedb_file_get_index(
		     file,
		     item_iterator,
		     &index,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve index: %d.",
			 function,
			 item_iterator + 1 );

			return( -1 );
		}
		if( libesedb_index_get_identifier(
		     index,
		     &item_identifier,
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

			return( -1 );
		}
		fprintf(
		 stream,
		 "\t(%" PRIu32 ")\t%s\n",
		 item_identifier,
		 value_string );

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

			return( -1 );
		}
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
	libesedb_error_t *error         = NULL;
	libesedb_file_t *esedb_file       = NULL;
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

