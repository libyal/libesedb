/*
 * Debug functions
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
#include <libnotify.h>

#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_libbfio.h"
#include "libesedb_string.h"

#if defined( HAVE_DEBUG_OUTPUT )

/* Prints the page flags
 * Returns 1 if successful or -1 on error
 */
int libesedb_debug_print_page_flags(
     uint32_t page_flags,
     liberror_error_t **error )
{
	static char *function = "libesedb_debug_print_page_flags";

	libnotify_verbose_printf(
	 "%s: page flags: 0x%08" PRIx32 "\n",
	 function,
	 page_flags );

	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_PARENT ) == LIBESEDB_PAGE_FLAG_IS_PARENT )
	{
		libnotify_verbose_printf(
		 "\tIs parent\n" );
	}
	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) == LIBESEDB_PAGE_FLAG_IS_LEAF )
	{
		libnotify_verbose_printf(
		 "\tIs leaf\n" );
	}
	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_ROOT ) == LIBESEDB_PAGE_FLAG_IS_ROOT )
	{
		libnotify_verbose_printf(
		 "\tIs root\n" );
	}
	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_EMPTY ) == LIBESEDB_PAGE_FLAG_IS_EMPTY )
	{
		libnotify_verbose_printf(
		 "\tIs empty\n" );
	}

	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_INDEX ) == LIBESEDB_PAGE_FLAG_IS_INDEX )
	{
		libnotify_verbose_printf(
		 "\tIs index\n" );
	}
	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_LONG_VALUE ) == LIBESEDB_PAGE_FLAG_IS_LONG_VALUE )
	{
		libnotify_verbose_printf(
		 "\tIs long value\n" );
	}

	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_PRIMARY ) == LIBESEDB_PAGE_FLAG_IS_PRIMARY )
	{
		libnotify_verbose_printf(
		 "\tIs primary\n" );
	}

	libnotify_verbose_printf(
	 "\n" );

	return( 1 );
}

/* Prints a log structure 
 * Returns 1 if successful or -1 on error
 */
int libesedb_debug_print_log_time(
     uint8_t *log_time,
     size_t log_time_size,
     const char *description,
     liberror_error_t **error )
{
	static char *function = "libesedb_debug_print_log_time";

	if( log_time == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid log time.",
		 function );

		return( -1 );
	}
	if( log_time_size < 8 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: log time too small.",
		 function );

		return( -1 );
	}
	if( log_time_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid log time size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( description == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid description.",
		 function );

		return( -1 );
	}
	libnotify_verbose_printf(
	 "%s: %s: %04d-%02d-%02d %02d:%02d:%02d (0x%02x 0x%02x)\n",
	 function,
	 description,
	 1900 + log_time[ 5 ],
	 log_time[ 4 ],
	 log_time[ 3 ],
	 log_time[ 2 ],
	 log_time[ 1 ],
	 log_time[ 0 ],
	 log_time[ 6 ],
	 log_time[ 7 ] );

	return( 1 );
}

/* Prints the read offsets
 * Returns 1 if successful or -1 on error
 */
int libesedb_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     liberror_error_t **error )
{
	static char *function = "libesedb_debug_print_read_offsets";
	off64_t offset        = 0;
	size64_t size         = 0;
	int amount_of_offsets = 0;
	int offset_iterator   = 0;

	if( file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io handle.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_get_amount_of_offsets_read(
	     file_io_handle,
	     &amount_of_offsets,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of offsets read.",
		 function );

		return( -1 );
	}
	libnotify_verbose_printf(
	 "Offsets read:\n" );

	for( offset_iterator = 0;
	     offset_iterator < amount_of_offsets;
	     offset_iterator++ )
	{
		if( libbfio_handle_get_offset_read(
		     file_io_handle,
		     offset_iterator,
		     &offset,
		     &size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset: %d.",
			 function,
			 offset_iterator );

			return( -1 );
		}
		libnotify_verbose_printf(
		 "%08" PRIu64 " ( 0x%08" PRIx64 " ) - %08" PRIu64 " ( 0x%08" PRIx64 " ) size: %" PRIu64 "\n",
		 offset,
		 offset,
		 offset + size,
		 offset + size,
		 size );
	}
	libnotify_verbose_printf(
	 "\n" );

	return( 1 );
}

#endif

