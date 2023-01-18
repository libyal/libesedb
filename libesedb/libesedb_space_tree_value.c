/*
 * Space tree value functions
 *
 * Copyright (C) 2009-2023, Joachim Metz <joachim.metz@gmail.com>
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_space_tree_value.h"

/* Creates a space tree value
 * Make sure the value space_tree_value is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_space_tree_value_initialize(
     libesedb_space_tree_value_t **space_tree_value,
     libcerror_error_t **error )
{
	static char *function = "libesedb_space_tree_value_initialize";

	if( space_tree_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid space tree value.",
		 function );

		return( -1 );
	}
	if( *space_tree_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid space tree value value already set.",
		 function );

		return( -1 );
	}
	*space_tree_value = memory_allocate_structure(
	                     libesedb_space_tree_value_t );

	if( *space_tree_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create space tree value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *space_tree_value,
	     0,
	     sizeof( libesedb_space_tree_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear space tree value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *space_tree_value != NULL )
	{
		memory_free(
		 *space_tree_value );

		*space_tree_value = NULL;
	}
	return( -1 );
}

/* Frees a space tree value
 * Returns 1 if successful or -1 on error
 */
int libesedb_space_tree_value_free(
     libesedb_space_tree_value_t **space_tree_value,
     libcerror_error_t **error )
{
	static char *function = "libesedb_space_tree_value_free";

	if( space_tree_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid space tree value.",
		 function );

		return( -1 );
	}
	if( *space_tree_value != NULL )
	{
		memory_free(
		 *space_tree_value );

		*space_tree_value = NULL;
	}
	return( 1 );
}

/* Reads a space tree value
 * Returns 1 if successful or -1 on error
 */
int libesedb_space_tree_value_read_data(
     libesedb_space_tree_value_t *space_tree_value,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libesedb_space_tree_value_read_data";

	if( space_tree_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid space tree value.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < 4 )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: space tree value:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 data,
	 space_tree_value->number_of_pages );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of pages\t\t\t: %" PRIu32 "\n",
		 function,
		 space_tree_value->number_of_pages );

		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

