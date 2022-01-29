/*
 * Extracts tables from an Extensible Storage Engine (ESE) Database (EDB) file
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
#include <system_string.h>
#include <types.h>

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "esedbtools_getopt.h"
#include "esedbtools_libcerror.h"
#include "esedbtools_libclocale.h"
#include "esedbtools_libcnotify.h"
#include "esedbtools_libcpath.h"
#include "esedbtools_libesedb.h"
#include "esedbtools_output.h"
#include "esedbtools_signal.h"
#include "esedbtools_unused.h"
#include "export_handle.h"
#include "log_handle.h"

export_handle_t *esedbexport_export_handle = NULL;
int esedbexport_abort                      = 0;

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

	fprintf( stream, "Usage: esedbexport [ -c codepage ] [ -l logfile ] [ -m mode ] [ -t target ]\n"
	                 "                   [ -T table_name ] [ -hvV ] source\n\n" );

	fprintf( stream, "\tsource: the source file\n\n" );

	fprintf( stream, "\t-c:     codepage of ASCII strings, options: ascii, windows-874,\n"
	                 "\t        windows-932, windows-936, windows-1250, windows-1251,\n"
	                 "\t        windows-1252 (default), windows-1253, windows-1254\n"
	                 "\t        windows-1255, windows-1256, windows-1257 or windows-1258\n" );
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

/* Signal handler for esedbexport
 */
void esedbexport_signal_handler(
      esedbtools_signal_t signal ESEDBTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function   = "esedbexport_signal_handler";

	ESEDBTOOLS_UNREFERENCED_PARAMETER( signal )

	esedbexport_abort = 1;

	if( esedbexport_export_handle != NULL )
	{
		if( export_handle_signal_abort(
		     esedbexport_export_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal export handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	if( _close(
	     0 ) != 0 )
#else
	if( close(
	     0 ) != 0 )
#endif
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	system_character_t *log_filename          = NULL;
	system_character_t *option_ascii_codepage = NULL;
	system_character_t *option_export_mode    = NULL;
	system_character_t *option_table_name     = NULL;
	system_character_t *option_target_path    = NULL;
	system_character_t *path_separator        = NULL;
	system_character_t *source                = NULL;
	libcerror_error_t *error                  = NULL;
	log_handle_t *log_handle                  = NULL;
	char *program                             = "esedbexport";
	size_t source_length                      = 0;
	size_t option_table_name_length           = 0;
	system_integer_t option                   = 0;
	int result                                = 0;
	int verbose                               = 0;

	libcnotify_stream_set(
	 stderr,
	 NULL );
	libcnotify_verbose_set(
	 1 );

	if( libclocale_initialize(
             "esedbtools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize locale values.\n" );

		goto on_error;
	}
	if( esedbtools_output_initialize(
             _IONBF,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

		goto on_error;
	}
	esedboutput_version_fprint(
	 stdout,
	 program );

	while( ( option = esedbtools_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "c:hl:m:t:T:vV" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (system_integer_t) 'c':
				option_ascii_codepage = optarg;

				break;

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'l':
				log_filename = optarg;

				break;

			case (system_integer_t) 'm':
				option_export_mode = optarg;

				break;

			case (system_integer_t) 't':
				option_target_path = optarg;

				break;

			case (system_integer_t) 'T':
				option_table_name = optarg;

				break;

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
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

	if( option_target_path == NULL )
	{
		source_length = system_string_length(
		                 source );

		path_separator = system_string_search_character_reverse(
		                  source,
		                  (system_character_t) LIBCPATH_SEPARATOR,
		                  source_length );

		if( path_separator == NULL )
		{
			path_separator = source;
		}
		else
		{
			path_separator++;
		}
		option_target_path = path_separator;
	}
	if( option_table_name != NULL )
	{
		option_table_name_length = system_string_length(
		                            option_table_name );
	}
	libcnotify_verbose_set(
	 verbose );
	libesedb_notify_set_stream(
	 stderr,
	 NULL );
	libesedb_notify_set_verbose(
	 verbose );

	if( log_handle_initialize(
	     &log_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize log handle.\n" );

		goto on_error;
	}
	if( export_handle_initialize(
	     &esedbexport_export_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize export handle.\n" );

		goto on_error;
	}
/* TODO
	esedbexport_export_handle->print_status_information = print_status_information;
*/
	if( option_export_mode != NULL )
	{
		result = export_handle_set_export_mode(
			  esedbexport_export_handle,
			  option_export_mode,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set export mode.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported export mode defaulting to: tables.\n" );
		}
	}
	if( option_ascii_codepage != NULL )
	{
		result = export_handle_set_ascii_codepage(
		          esedbexport_export_handle,
		          option_ascii_codepage,
		          &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set ASCII codepage in export handle.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported ASCII codepage defaulting to: windows-1252.\n" );
		}
	}
	if( export_handle_set_target_path(
	     esedbexport_export_handle,
	     option_target_path,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set target path.\n" );

		goto on_error;
	}
	result = export_handle_create_items_export_path(
	          esedbexport_export_handle,
	          &error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to create items export path.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		fprintf(
		 stderr,
		 "%" PRIs_SYSTEM " already exists.\n",
		 esedbexport_export_handle->items_export_path );

		goto on_error;
	}
	if( log_handle_open(
	     log_handle,
	     log_filename,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open log file: %" PRIs_SYSTEM ".\n",
		 log_filename );

		goto on_error;
	}
	fprintf(
	 stdout,
	 "Opening file.\n" );

#ifdef TODO_SIGNAL_ABORT
	if( esedbtools_signal_attach(
	     esedbexport_signal_handler,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to attach signal handler.\n" );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
#endif
	if( export_handle_open(
	     esedbexport_export_handle,
	     source,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open: %" PRIs_SYSTEM ".\n",
		 source );

		goto on_error;
	}
	result = export_handle_export_file(
	          esedbexport_export_handle,
	          option_table_name,
	          option_table_name_length,
	          log_handle,
	          &error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to export file.\n" );

		goto on_error;
	}
#ifdef TODO_SIGNAL_ABORT
	if( esedbtools_signal_detach(
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to detach signal handler.\n" );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
#endif
	if( export_handle_close(
	     esedbexport_export_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close export handle.\n" );

		goto on_error;
	}
	if( export_handle_free(
	     &esedbexport_export_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free export handle.\n" );

		goto on_error;
	}
	if( log_handle_close(
	     log_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close log handle.\n" );

		goto on_error;
	}
	if( log_handle_free(
	     &log_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free log handle.\n" );

		goto on_error;
	}
	if( esedbexport_abort != 0 )
	{
		fprintf(
		 stdout,
		 "Export aborted.\n" );

		return( EXIT_FAILURE );
	}
	if( result == 0 )
	{
		if( option_table_name != NULL )
		{
			fprintf(
			 stdout,
			 "Export failed no such table: %" PRIs_SYSTEM ".\n",
                         option_table_name );
		}
		else
		{
			fprintf(
			 stdout,
			 "Export failed.\n" );
		}
		return( EXIT_FAILURE );
	}
	fprintf(
	 stdout,
	 "Export completed.\n" );

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( esedbexport_export_handle != NULL )
	{
		export_handle_free(
		 &esedbexport_export_handle,
		 NULL );
	}
	if( log_handle != NULL )
	{
		log_handle_free(
		 &log_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

