/*
 * Log handle
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
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDARG_H ) || defined( WINAPI )
#include <stdarg.h>
#elif defined( HAVE_VARARGS_H )
#include <varargs.h>
#else
#error Missing headers stdarg.h and varargs.h
#endif

#include "esedbtools_libcerror.h"
#include "log_handle.h"

/* Creates a log handle
 * Make sure the value log_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int log_handle_initialize(
     log_handle_t **log_handle,
     libcerror_error_t **error )
{
	static char *function = "log_handle_initialize";

	if( log_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid log handle.",
		 function );

		return( -1 );
	}
	if( *log_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid log handle value already set.",
		 function );

		return( -1 );
	}
	*log_handle = memory_allocate_structure(
	               log_handle_t );

	if( *log_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create log handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *log_handle,
	     0,
	     sizeof( log_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear log handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *log_handle != NULL )
	{
		memory_free(
		 *log_handle );

		*log_handle = NULL;
	}
	return( -1 );
}

/* Frees a log handle
 * Returns 1 if successful or -1 on error
 */
int log_handle_free(
     log_handle_t **log_handle,
     libcerror_error_t **error )
{
	static char *function = "log_handle_free";

	if( log_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid log handle.",
		 function );

		return( -1 );
	}
	if( *log_handle != NULL )
	{
		memory_free(
		 *log_handle );

		*log_handle = NULL;
	}
	return( 1 );
}

/* Opens the log handle
 * Returns 1 if successful or -1 on error
 */
int log_handle_open(
     log_handle_t *log_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function = "log_handle_open";

	if( log_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid log handle.",
		 function );

		return( -1 );
	}
	if( filename != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		log_handle->log_stream = file_stream_open_wide(
		                          filename,
		                          _WIDE_STRING( FILE_STREAM_OPEN_APPEND ) );
#else
		log_handle->log_stream = file_stream_open(
		                          filename,
		                          FILE_STREAM_OPEN_APPEND );
#endif
		if( log_handle->log_stream == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Closes the log handle
 * Returns the 0 if succesful or -1 on error
 */
int log_handle_close(
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	static char *function = "log_handle_close";

	if( log_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid log handle.",
		 function );

		return( -1 );
	}
	if( log_handle->log_stream != NULL )
	{
		if( file_stream_close(
		     log_handle->log_stream ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close log stream.",
			 function );

			return( -1 );
		}
		log_handle->log_stream = NULL;
	}
	return( 0 );
}

#if defined( HAVE_STDARG_H ) || defined( WINAPI )
#define VARARGS( function, type, argument ) \
	function( log_handle_t *log_handle, type argument, ... )
#define VASTART( argument_list, type, name ) \
	va_start( argument_list, name )
#define VAEND( argument_list ) \
	va_end( argument_list )

#elif defined( HAVE_VARARGS_H )
#define VARARGS( function, type, argument ) \
	function( log_handle_t *log_handle, va_alist ) va_dcl
#define VASTART( argument_list, type, name ) \
	{ type name; va_start( argument_list ); name = va_arg( argument_list, type )
#define VAEND( argument_list ) \
	va_end( argument_list ); }

#endif

/* Print a formatted string on the notify stream
 */
void VARARGS(
      log_handle_printf,
      char *,
      format )
{
	va_list argument_list;

	if( ( log_handle != NULL )
	 && ( log_handle->log_stream != NULL ) )
	{
		VASTART(
		 argument_list,
		 char *,
		 format );

		vfprintf(
		 log_handle->log_stream,
		 format,
		 argument_list );

		VAEND(
		 argument_list );
	}
}

#undef VARARGS
#undef VASTART
#undef VAEND

