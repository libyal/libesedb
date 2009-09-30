/*
 * Extracts tables from an Extensible Storage Engine (ESE) Database (EDB) file
 *
 * Copyright (c) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
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
#include "log_handle.h"
#include "esedbcommon.h"
#include "esedbinput.h"
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
	fprintf( stream, "Use esedbexport to export items stored in an Extensible Storage Engine (ESE)\n"
	                 "Database (EDB) file\n\n" );

	fprintf( stream, "Usage: esedbexport [ -c codepage ] [ -l logfile ] [ -t target ] [ -hvV ]\n"
	                 "                   source\n\n" );

	fprintf( stream, "\tsource: the source file\n\n" );

	fprintf( stream, "\t-c:     codepage of ASCII strings, options: ascii, windows-1250 (default),\n"
	                 "\t        windows-1251, windows-1252, windows-1253, windows-1254,\n"
	                 "\t        windows-1255, windows-1256, windows-1257 or windows-1258\n" );
	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-l:     logs information about the exported items\n" );
	fprintf( stream, "\t-t:     specify the basename of the target directory to export to\n"
	                 "\t        (default is the source filename) esedbexport will add the suffix\n"
	                 "\t        .export to the basename\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
}

/* The main program
 */
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	export_handle_t *export_handle            = NULL;
	liberror_error_t *error                   = NULL;
	log_handle_t *log_handle                  = NULL;
	libsystem_character_t *export_path        = NULL;
	libsystem_character_t *log_filename       = NULL;
	libsystem_character_t *option_target_path = NULL;
	libsystem_character_t *path_separator     = NULL;
	libsystem_character_t *source             = NULL;
	libsystem_character_t *target_path        = NULL;
	char *program                             = "esedbexport";
	size_t export_path_length                 = 0;
	size_t source_length                      = 0;
	size_t target_path_length                 = 0;
	libsystem_integer_t option                = 0;
	int ascii_codepage                        = LIBESEDB_CODEPAGE_WINDOWS_1250;
	int result                                = 0;
	int verbose                               = 0;

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
	                   _LIBSYSTEM_CHARACTER_T_STRING( "c:hl:t:vV" ) ) ) != (libsystem_integer_t) -1 )
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

			case (libsystem_integer_t) 'c':
				if( esedbinput_determine_ascii_codepage(
				     optarg,
				     &ascii_codepage,
				     &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1250;

					fprintf(
					 stderr,
					 "Unsupported ASCII codepage defaulting to: windows-1250.\n" );
				}
				break;

			case (libsystem_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libsystem_integer_t) 'l':
				log_filename = optarg;

				break;

			case (libsystem_integer_t) 't':
				option_target_path = optarg;

				break;

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

	/* Determine if target path basename
	 */
	if( option_target_path != NULL )
	{
		target_path = option_target_path;
	}
	else
	{
		source_length = libsystem_string_length(
		                 source );

		path_separator = libsystem_string_search_reverse(
		                  source,
		                  (libsystem_character_t) ESEDBCOMMON_PATH_SEPARATOR,
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
	target_path_length = libsystem_string_length(
	                      target_path );

	/* Create the export path
	 */
	export_path_length = 7 + target_path_length;

	export_path = (libsystem_character_t *) memory_allocate(
	                                         sizeof( libsystem_character_t ) * ( export_path_length + 1 ) );

	if( export_path == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to create export path.\n" );

		return( EXIT_FAILURE );
	}
	if( libsystem_string_snprintf(
	     export_path,
	     export_path_length + 1,
	     _LIBSYSTEM_CHARACTER_T_STRING( "%" ) _LIBSYSTEM_CHARACTER_T_STRING( PRIs_LIBSYSTEM ) _LIBSYSTEM_CHARACTER_T_STRING( ".export" ),
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
		 "Unable to determine if %" PRIs_LIBSYSTEM " exists.\n",
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
		 "%" PRIs_LIBSYSTEM " already exists.\n",
		 export_path );
	}
	if( result != 0 )
	{
		memory_free(
		 export_path );

		return( EXIT_FAILURE );
	}
	if( log_handle_initialize(
	     &log_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize log handle.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		memory_free(
		 export_path );

		return( EXIT_FAILURE );
	}
	if( export_handle_initialize(
	     &export_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize export handle.\n" );

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
	if( log_handle_open(
	     log_handle,
	     log_filename,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open log file: %" PRIs_LIBSYSTEM ".\n",
		 log_filename );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		export_handle_free(
		 &export_handle,
		 NULL );
		log_handle_free(
		 &log_handle,
		 NULL );
		memory_free(
		 export_path );

		return( EXIT_FAILURE );
	}
	fprintf(
	 stdout,
	 "Opening file.\n" );

	if( export_handle_open(
	     export_handle,
	     source,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Error opening file: %" PRIs_LIBSYSTEM ".\n",
		 argv[ optind ] );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		export_handle_free(
		 &export_handle,
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
	     export_path,
	     export_path_length + 1,
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

		export_handle_free(
		 &export_handle,
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

	if( export_handle_close(
	     export_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Error closing file: %" PRIs_LIBSYSTEM "",
		 argv[ optind ] );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		export_handle_free(
		 &export_handle,
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
		 "Unable to close log file: %" PRIs_LIBSYSTEM ".\n",
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

