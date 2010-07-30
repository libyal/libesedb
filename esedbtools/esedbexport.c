/*
 * Extracts tables from an Extensible Storage Engine (ESE) Database (EDB) file
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
#include <memory.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

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

#include "export_handle.h"
#include "esedboutput.h"
#include "log_handle.h"

export_handle_t *esedbexport_handle = NULL;
libesedb_file_t *esedbexport_file   = NULL;
int esedbexport_abort               = 0;

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use esedbexport to export items stored in an Extensible Storage Engine (ESE)\n"
	                 "Database (EDB) file\n\n" );

	fprintf( stream, "Usage: esedbexport [ -l logfile ] [ -m mode ] [ -t target ] [ -T table_name ]\n"
	                 "                   [ -hvV ] source\n\n" );

	fprintf( stream, "\tsource: the source file\n\n" );

	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-l:     logs information about the exported items\n" );
	fprintf( stream, "\t-m:     export mode, option: all, tables (default)\n"
	                 "\t        'all' exports all the tables or a single specified table with indexes,\n"
	                 "\t        'tables' exports all the tables or a single specified table\n" );
	fprintf( stream, "\t-t:     specify the basename of the target directory to export to\n"
	                 "\t        (default is the source filename) esedbexport will add the suffix\n"
	                 "\t        .export to the basename\n" );
	fprintf( stream, "\t-T:     exports only a specific table\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
}

/* Determines the export mode
 * Returns 1 if successful or -1 on error
 */
int esedbexport_determine_export_mode(
     const libcstring_system_character_t *argument,
     int *export_mode,
     liberror_error_t **error )
{
	static char *function  = "esedbexport_determine_export_mode";
	size_t argument_length = 0;
	int result             = -1;

	if( argument == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid argument string.",
		 function );

		return( -1 );
	}
	if( export_mode == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export mode.",
		 function );

		return( -1 );
	}
	argument_length = libcstring_system_string_length(
	                   argument );

	if(  argument_length == 3 )
	{
		if( libcstring_system_string_compare(
		     argument,
		     _LIBCSTRING_SYSTEM_STRING( "all" ),
		     3 ) == 0 )
		{
			*export_mode = EXPORT_MODE_ALL;
			result       = 1;
		}
	}
	else if( argument_length == 6 )
	{
		if( libcstring_system_string_compare(
		     argument,
		     _LIBCSTRING_SYSTEM_STRING( "tables" ),
		     6 ) == 0 )
		{
			*export_mode = EXPORT_MODE_TABLES;
			result       = 1;
		}
	}
	return( result );
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	export_handle_t *export_handle                    = NULL;
	liberror_error_t *error                           = NULL;
	log_handle_t *log_handle                          = NULL;
	libcstring_system_character_t *export_path        = NULL;
	libcstring_system_character_t *log_filename       = NULL;
	libcstring_system_character_t *option_table_name  = NULL;
	libcstring_system_character_t *option_target_path = NULL;
	libcstring_system_character_t *path_separator     = NULL;
	libcstring_system_character_t *source             = NULL;
	libcstring_system_character_t *target_path        = NULL;
	char *program                                     = "esedbexport";
	size_t export_path_length                         = 0;
	size_t source_length                              = 0;
	size_t table_name_size                            = 0;
	size_t target_path_length                         = 0;
	libcstring_system_integer_t option                = 0;
	int export_mode                                   = EXPORT_MODE_TABLES;
	int result                                        = 0;
	int verbose                                       = 0;

	libsystem_notify_set_stream(
	 stderr,
	 NULL );
	libsystem_notify_set_verbose(
	 1 );

	if( libsystem_initialize(
             "esedbtools",
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
	                   _LIBCSTRING_SYSTEM_STRING( "c:hl:m:t:T:vV" ) ) ) != (libcstring_system_integer_t) -1 )
	{
		switch( option )
		{
			case (libcstring_system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_LIBCSTRING_SYSTEM "\n",
				 argv[ optind ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (libcstring_system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libcstring_system_integer_t) 'l':
				log_filename = optarg;

				break;

			case (libcstring_system_integer_t) 'm':
				if( esedbexport_determine_export_mode(
				     optarg,
				     &export_mode,
				     &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					export_mode = EXPORT_MODE_TABLES;

					fprintf(
					 stderr,
					 "Unsupported export mode defaulting to: tables.\n" );
				}
				break;

			case (libcstring_system_integer_t) 't':
				option_target_path = optarg;

				break;

			case (libcstring_system_integer_t) 'T':
				option_table_name = optarg;

				break;

			case (libcstring_system_integer_t) 'v':
				verbose = 1;

				break;

			case (libcstring_system_integer_t) 'V':
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

	/* Determine if target path basename
	 */
	if( option_target_path != NULL )
	{
		target_path = option_target_path;
	}
	else
	{
		source_length = libcstring_system_string_length(
		                 source );

		path_separator = libcstring_system_string_search_reverse(
		                  source,
		                  (libcstring_system_character_t) LIBSYSTEM_PATH_SEPARATOR,
		                  source_length );

		if( path_separator == NULL )
		{
			path_separator = source;
		}
		else
		{
			path_separator++;
		}
		target_path = path_separator;
	}
	target_path_length = libcstring_system_string_length(
	                      target_path );

	/* Create the export path
	 */
	export_path_length = 7 + target_path_length;

	export_path = (libcstring_system_character_t *) memory_allocate(
	                                                 sizeof( libcstring_system_character_t ) * ( export_path_length + 1 ) );

	if( export_path == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to create export path.\n" );

		return( EXIT_FAILURE );
	}
	if( libcstring_system_string_sprintf(
	     export_path,
	     export_path_length + 1,
	     _LIBCSTRING_SYSTEM_STRING( "%" ) _LIBCSTRING_SYSTEM_STRING( PRIs_LIBCSTRING_SYSTEM ) _LIBCSTRING_SYSTEM_STRING( ".export" ),
	     target_path ) == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to set export path.\n" );

		memory_free(
		 export_path );

		return( EXIT_FAILURE );
	}
	/* Determine if the export paths exists
	 */
	result = libsystem_file_exists(
	          export_path,
	          &error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to determine if %" PRIs_LIBCSTRING_SYSTEM " exists.\n",
		 export_path );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );
	}
	else if( result == 1 )
	{
		fprintf(
		 stderr,
		 "%" PRIs_LIBCSTRING_SYSTEM " already exists.\n",
		 export_path );
	}
	if( result != 0 )
	{
		memory_free(
		 export_path );

		return( EXIT_FAILURE );
	}
	if( option_table_name != NULL )
	{
		table_name_size = 1 + libcstring_system_string_length(
	        	               option_table_name );
	}
	if( log_handle_initialize(
	     &log_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create log handle.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		memory_free(
		 export_path );

		return( EXIT_FAILURE );
	}
	if( log_handle_open(
	     log_handle,
	     log_filename,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open log file: %" PRIs_LIBCSTRING_SYSTEM ".\n",
		 log_filename );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		log_handle_free(
		 &log_handle,
		 NULL );
		memory_free(
		 export_path );

		return( EXIT_FAILURE );
	}
	if( export_handle_initialize(
	     &export_handle,
	     export_mode,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create export handle.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		log_handle_close(
		 log_handle,
		 NULL );
		log_handle_free(
		 &log_handle,
		 NULL );
		memory_free(
		 export_path );

		return( EXIT_FAILURE );
	}
	if( libesedb_file_initialize(
	     &esedbexport_file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create libesedb file.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		export_handle_free(
		 &export_handle,
		 NULL );
		log_handle_close(
		 log_handle,
		 NULL );
		log_handle_free(
		 &log_handle,
		 NULL );
		memory_free(
		 export_path );

		return( -1 );
	}
	fprintf(
	 stdout,
	 "Opening file.\n" );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libesedb_file_open_wide(
	     esedbexport_file,
	     source,
	     LIBESEDB_OPEN_READ,
	     &error ) != 1 )
#else
	if( libesedb_file_open(
	     esedbexport_file,
	     source,
	     LIBESEDB_OPEN_READ,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Error opening file: %" PRIs_LIBCSTRING_SYSTEM ".\n",
		 source );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		libesedb_file_free(
		 &esedbexport_file,
		 NULL );
		export_handle_free(
		 &export_handle,
		 NULL );
		log_handle_close(
		 log_handle,
		 NULL );
		log_handle_free(
		 &log_handle,
		 NULL );
		memory_free(
		 export_path );

		return( EXIT_FAILURE );
	}
	if( export_handle_export_file(
	     export_handle,
	     esedbexport_file,
	     export_path,
	     export_path_length + 1,
	     option_table_name,
	     table_name_size,
	     log_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to export file.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		libesedb_file_close(
		 esedbexport_file,
		 NULL );
		libesedb_file_free(
		 &esedbexport_file,
		 NULL );
		export_handle_free(
		 &export_handle,
		 NULL );
		log_handle_close(
		 log_handle,
		 NULL );
		log_handle_free(
		 &log_handle,
		 NULL );
		memory_free(
		 export_path );

		return( EXIT_FAILURE );
	}
	memory_free(
	 export_path );

	if( libesedb_file_close(
	     esedbexport_file,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Error closing file: %" PRIs_LIBCSTRING_SYSTEM ".\n",
		 source );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		libesedb_file_free(
		 &esedbexport_file,
		 NULL );
		export_handle_free(
		 &export_handle,
		 NULL );
		log_handle_close(
		 log_handle,
		 NULL );
		log_handle_free(
		 &log_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( libesedb_file_free(
	     &esedbexport_file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free libesedb file.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		export_handle_free(
		 &export_handle,
		 NULL );
		log_handle_close(
		 log_handle,
		 NULL );
		log_handle_free(
		 &log_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( export_handle_free(
	     &export_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free export handle.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		log_handle_close(
		 log_handle,
		 NULL );
		log_handle_free(
		 &log_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( log_handle_close(
	     log_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close log file: %" PRIs_LIBCSTRING_SYSTEM ".\n",
		 log_filename );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		log_handle_free(
		 &log_handle,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( log_handle_free(
	     &log_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free log handle.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	fprintf(
	 stdout,
	 "Export completed.\n" );

	return( EXIT_SUCCESS );
}

