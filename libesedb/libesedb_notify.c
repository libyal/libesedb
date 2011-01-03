/*
 * Notification functions
 *
 * Copyright (c) 2009-2011, Joachim Metz <jbmetz@users.sourceforge.net>
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
#include <types.h>

#include <liberror.h>
#include <libnotify.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include "libesedb_notify.h"

#if !defined( HAVE_LOCAL_LIBESEDB )

/* Sets the verbose notification
 */
void libesedb_notify_set_verbose(
      int verbose )
{
	libnotify_set_verbose(
	 verbose );
}

/* Sets the notification stream
 * Returns 1 if successful or -1 on error
 */
int libesedb_notify_set_stream(
     FILE *stream,
     liberror_error_t **error )
{
	static char *function = "libesedb_notify_set_stream";

	if( libnotify_set_stream(
	     stream,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set stream.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Opens the notification stream using a filename
 * The stream is opened in append mode
 * Returns 1 if successful or -1 on error
 */
int libesedb_notify_stream_open(
     const char *filename,
     liberror_error_t **error )
{
	static char *function = "libesedb_notify_stream_open";

	if( libnotify_stream_open(
	     filename,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open stream.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Closes the notification stream if opened using a filename
 * Returns 0 if successful or -1 on error
 */
int libesedb_notify_stream_close(
     liberror_error_t **error )
{
	static char *function = "libesedb_notify_stream_close";

	if( libnotify_stream_close(
	     error ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open stream.",
		 function );

		return( -1 );
	}
	return( 0 );
}

#endif

