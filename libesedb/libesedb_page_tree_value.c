/*
 * Page tree value functions
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

#include "libesedb_definitions.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_page_tree_value.h"

/* Creates a page tree value
 * Make sure the value page_tree_value is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_value_initialize(
     libesedb_page_tree_value_t **page_tree_value,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_tree_value_initialize";

	if( page_tree_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree value.",
		 function );

		return( -1 );
	}
	if( *page_tree_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid page tree value value already set.",
		 function );

		return( -1 );
	}
	*page_tree_value = memory_allocate_structure(
	                    libesedb_page_tree_value_t );

	if( *page_tree_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create page tree value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *page_tree_value,
	     0,
	     sizeof( libesedb_page_tree_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear page tree value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *page_tree_value != NULL )
	{
		memory_free(
		 *page_tree_value );

		*page_tree_value = NULL;
	}
	return( -1 );
}

/* Frees a page tree value
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_value_free(
     libesedb_page_tree_value_t **page_tree_value,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_tree_value_free";

	if( page_tree_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree value.",
		 function );

		return( -1 );
	}
	if( *page_tree_value != NULL )
	{
		memory_free(
		 *page_tree_value );

		*page_tree_value = NULL;
	}
	return( 1 );
}

/* Reads a page tree value
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_value_read_data(
     libesedb_page_tree_value_t *page_tree_value,
     const uint8_t *data,
     size_t data_size,
     uint8_t page_value_flags,
     libcerror_error_t **error )
{
	static char *function        = "libesedb_page_tree_value_read_data";
	size_t data_offset           = 0;
	size_t minimum_data_size     = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	const uint8_t *page_key_data = NULL;
	size_t page_key_size         = 0;
#endif

	if( page_tree_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree value.",
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
	if( ( page_value_flags & LIBESEDB_PAGE_TAG_FLAG_HAS_COMMON_KEY_SIZE ) != 0 )
	{
		minimum_data_size = 4;
	}
	else
	{
		minimum_data_size = 2;
	}
	if( ( data_size < minimum_data_size )
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
		 "%s: page tree value:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( ( page_value_flags & LIBESEDB_PAGE_TAG_FLAG_HAS_COMMON_KEY_SIZE ) != 0 )
	{
		byte_stream_copy_to_uint16_little_endian(
		 data,
		 page_tree_value->common_key_size );

		data_offset += 2;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: common key size\t\t\t: %" PRIu16 "\n",
			 function,
			 page_tree_value->common_key_size );
		}
#endif
	}
	byte_stream_copy_to_uint16_little_endian(
	 &( data[ data_offset ] ),
	 page_tree_value->local_key_size );

	data_offset += 2;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: local key size\t\t\t: %" PRIu16 "\n",
		 function,
		 page_tree_value->local_key_size );
	}
#endif
	if( (size_t) page_tree_value->local_key_size > data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid local key size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: local key value\t\t\t: ",
		 function );

		page_key_data = &( data[ data_offset ] );
		page_key_size = page_tree_value->local_key_size;

		while( page_key_size > 0 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%02" PRIx8 " ",
				 *page_key_data );
			}
			page_key_data++;
			page_key_size--;
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	page_tree_value->local_key = &( data[ data_offset ] );

	data_offset += page_tree_value->local_key_size;

	page_tree_value->data      = &( data[ data_offset ] );
	page_tree_value->data_size = data_size - data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: page tree value data:\n",
		 function );
		libcnotify_print_data(
		 page_tree_value->data,
		 page_tree_value->data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	return( 1 );
}

