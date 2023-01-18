/*
 * Root page header functions
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
#include "libesedb_root_page_header.h"

#include "esedb_page_values.h"

/* Creates a root page header
 * Make sure the value root_page_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_root_page_header_initialize(
     libesedb_root_page_header_t **root_page_header,
     libcerror_error_t **error )
{
	static char *function = "libesedb_root_page_header_initialize";

	if( root_page_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid root page header.",
		 function );

		return( -1 );
	}
	if( *root_page_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid root page header value already set.",
		 function );

		return( -1 );
	}
	*root_page_header = memory_allocate_structure(
	                     libesedb_root_page_header_t );

	if( *root_page_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create root page header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *root_page_header,
	     0,
	     sizeof( libesedb_root_page_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear root page header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *root_page_header != NULL )
	{
		memory_free(
		 *root_page_header );

		*root_page_header = NULL;
	}
	return( -1 );
}

/* Frees a root page header
 * Returns 1 if successful or -1 on error
 */
int libesedb_root_page_header_free(
     libesedb_root_page_header_t **root_page_header,
     libcerror_error_t **error )
{
	static char *function = "libesedb_root_page_header_free";

	if( root_page_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid root page header.",
		 function );

		return( -1 );
	}
	if( *root_page_header != NULL )
	{
		/* The data is referenced and freed elsewhere
		 */
		memory_free(
		 *root_page_header );

		*root_page_header = NULL;
	}
	return( 1 );
}

/* Reads a root page header
 * Returns 1 if successful or -1 on error
 */
int libesedb_root_page_header_read_data(
     libesedb_root_page_header_t *root_page_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libesedb_root_page_header_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
#endif

	if( root_page_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid root page header.",
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
	if( ( data_size != sizeof( esedb_root_page_header_t ) )
	 && ( data_size != sizeof( esedb_extended_root_page_header_t ) ) )
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
		 "%s: root page header:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_root_page_header_t *) data )->initial_number_of_pages,
	 root_page_header->initial_number_of_pages );

	if( data_size == sizeof( esedb_root_page_header_t ) )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_root_page_header_t *) data )->extent_space,
		 root_page_header->extent_space );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_root_page_header_t *) data )->space_tree_page_number,
		 root_page_header->space_tree_page_number );
	}
	else if( data_size == sizeof( esedb_extended_root_page_header_t ) )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_extended_root_page_header_t *) data )->extent_space,
		 root_page_header->extent_space );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_extended_root_page_header_t *) data )->space_tree_page_number,
		 root_page_header->space_tree_page_number );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: initial number of pages\t\t: %" PRIu32 "\n",
		 function,
		 root_page_header->initial_number_of_pages );

		if( data_size == sizeof( esedb_extended_root_page_header_t ) )
		{
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (esedb_extended_root_page_header_t *) data )->unknown1 );
		}
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_root_page_header_t *) data )->parent_father_data_page_number,
		 value_32bit );
		libcnotify_printf(
		 "%s: parent FDP number\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: extent space\t\t\t: %" PRIu32 "\n",
		 function,
		 root_page_header->extent_space );

		libcnotify_printf(
		 "%s: space tree page number\t\t: %" PRIu32 " (0x%08" PRIx32 ")\n",
		 function,
		 root_page_header->space_tree_page_number,
		 root_page_header->space_tree_page_number );

		libcnotify_printf(
		 "%s: primary extent\t\t\t: %" PRIu32 "-%c\n",
		 function,
		 root_page_header->initial_number_of_pages,
		 ( root_page_header->extent_space == 0 ? 's' : 'm' ) );

		if( data_size == sizeof( esedb_extended_root_page_header_t ) )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (esedb_extended_root_page_header_t *) data )->unknown2,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown2\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (esedb_extended_root_page_header_t *) data )->unknown3,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown3\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

