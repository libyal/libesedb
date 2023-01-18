/*
 * Space tree functions
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
#include <memory.h>
#include <types.h>

#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_page.h"
#include "libesedb_page_value.h"
#include "libesedb_page_tree.h"
#include "libesedb_page_tree_value.h"
#include "libesedb_space_tree.h"
#include "libesedb_space_tree_value.h"

/* Creates a space tree
 * Make sure the value space_tree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_space_tree_initialize(
     libesedb_space_tree_t **space_tree,
     libesedb_io_handle_t *io_handle,
     uint32_t object_identifier,
     uint32_t root_page_number,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     libcerror_error_t **error )
{
	static char *function = "libesedb_space_tree_initialize";

	if( space_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid space tree.",
		 function );

		return( -1 );
	}
	if( *space_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid space tree value already set.",
		 function );

		return( -1 );
	}
	*space_tree = memory_allocate_structure(
	               libesedb_space_tree_t );

	if( *space_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create space tree.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     ( *space_tree ),
	     0,
	     sizeof( libesedb_space_tree_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear space tree.",
		 function );

		memory_free(
		 *space_tree );

		*space_tree = NULL;

		return( -1 );
	}
	if( libesedb_page_tree_initialize(
	     &( ( *space_tree )->page_tree ),
	     io_handle,
	     pages_vector,
	     pages_cache,
	     object_identifier,
	     root_page_number,
	     NULL,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create page tree.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *space_tree != NULL )
	{
		memory_free(
		 *space_tree );

		*space_tree = NULL;
	}
	return( -1 );
}

/* Frees a space_tree
 * Returns 1 if successful or -1 on error
 */
int libesedb_space_tree_free(
     libesedb_space_tree_t **space_tree,
     libcerror_error_t **error )
{
	static char *function = "libesedb_space_tree_free";
	int result            = 1;

	if( space_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid space tree.",
		 function );

		return( -1 );
	}
	if( *space_tree != NULL )
	{
		if( libesedb_page_tree_free(
		     &( ( *space_tree )->page_tree ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free page tree.",
			 function );

			result = -1;
		}
		memory_free(
		 *space_tree );

		*space_tree = NULL;
	}
	return( result );
}

/* Reads the space_tree values from a page
 * Returns 1 if successful or -1 on error
 */
int libesedb_space_tree_read_values_from_page(
     libesedb_space_tree_t *space_tree,
     libbfio_handle_t *file_io_handle,
     libesedb_page_t *page,
     int *current_leaf_value_index,
     int recursion_depth,
     libcerror_error_t **error )
{
	libesedb_page_t *child_page                   = NULL;
	libesedb_page_tree_value_t *page_tree_value   = NULL;
	libesedb_page_value_t *page_value             = NULL;
	libesedb_space_tree_value_t *space_tree_value = NULL;
	libfcache_cache_t *child_page_cache           = NULL;
	static char *function                         = "libesedb_space_tree_read_values_from_page";
	uint32_t child_page_number                    = 0;
	uint32_t page_flags                           = 0;
	uint32_t total_number_of_pages                = 0;
	uint16_t number_of_page_values                = 0;
	uint16_t page_value_index                     = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t father_data_page_object_identifier   = 0;
#endif

	if( space_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid space tree.",
		 function );

		return( -1 );
	}
	if( space_tree->page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid space tree - missing page tree.",
		 function );

		return( -1 );
	}
	if( space_tree->page_tree->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid space tree - invalid page tree - missing IO handle.",
		 function );

		return( -1 );
	}
	if( current_leaf_value_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid current leaf value index.",
		 function );

		return( -1 );
	}
	if( ( recursion_depth < 0 )
	 || ( recursion_depth > LIBESEDB_MAXIMUM_INDEX_NODE_RECURSION_DEPTH ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid recursion depth value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libesedb_page_get_father_data_page_object_identifier(
		     page,
		     &father_data_page_object_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve father data page object identifier.",
			 function );

			goto on_error;
		}
		if( space_tree->page_tree->object_identifier != father_data_page_object_identifier )
		{
			libcnotify_printf(
			 "%s: mismatch in father data page object identifier (tree: %" PRIu32 " != page: %" PRIu32 ").\n",
			 function,
			 space_tree->page_tree->object_identifier,
			 father_data_page_object_identifier );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( libesedb_page_get_number_of_values(
	     page,
	     &number_of_page_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of page values.",
		 function );

		goto on_error;
	}
	if( number_of_page_values == 0 )
	{
		return( 1 );
	}
	if( libesedb_page_get_flags(
	     page,
	     &page_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page flags.",
		 function );

		goto on_error;
	}
	if( libesedb_page_get_value_by_index(
	     page,
	     0,
	     &page_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: 0.",
		 function );

		goto on_error;
	}
	if( page_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page value.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: page value: 000 page tag flags\t: 0x%02" PRIx8 "",
		 function,
		 page_value->flags );
		libesedb_debug_print_page_tag_flags(
		 page_value->flags );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: page value: 000 data:\n",
		 function );
		libcnotify_print_data(
		 page_value->data,
		 (size_t) page_value->size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

        /* Use a local cache to prevent cache invalidation of the page
         * when reading child pages.
         */
	if( libfcache_cache_initialize(
	     &child_page_cache,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create child page cache.",
		 function );

		goto on_error;
	}
	for( page_value_index = 1;
	     page_value_index < number_of_page_values;
	     page_value_index++ )
	{
		if( libesedb_page_get_value_by_index(
		     page,
		     page_value_index,
		     &page_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page value: %" PRIu16 ".",
			 function,
			 page_value_index );

			goto on_error;
		}
		if( page_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing page value: %" PRIu16 ".",
			 function,
			 page_value_index );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: page value: %03" PRIu16 " page tag flags\t: 0x%02" PRIx8 "",
			 function,
			 page_value_index,
			 page_value->flags );
			libesedb_debug_print_page_tag_flags(
			 page_value->flags );
			libcnotify_printf(
			 "\n" );

			libcnotify_printf(
			 "\n" );
		}
#endif
		if( ( page_value->flags & LIBESEDB_PAGE_TAG_FLAG_IS_DEFUNCT ) != 0 )
		{
			continue;
		}
		if( libesedb_page_tree_value_initialize(
		     &page_tree_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create page tree value.",
			 function );

			goto on_error;
		}
		if( libesedb_page_tree_value_read_data(
		     page_tree_value,
		     page_value->data,
		     (size_t) page_value->size,
		     page_value->flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read page tree value: %" PRIu16 ".",
			 function,
			 page_value_index );

			goto on_error;
		}
		if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) != 0 )
		{
			if( libesedb_space_tree_value_initialize(
			     &space_tree_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create space tree value.",
				 function );

				goto on_error;
			}
			if( libesedb_space_tree_value_read_data(
			     space_tree_value,
			     page_tree_value->data,
			     (size_t) page_tree_value->data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read space tree value: %" PRIu16 ".",
				 function,
				 page_value_index );

				goto on_error;
			}
			total_number_of_pages += space_tree_value->number_of_pages;

			if( libesedb_space_tree_value_free(
			     &space_tree_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free space tree value.",
				 function );

				goto on_error;
			}
		}
		else
		{
			if( page_tree_value->data_size < 4 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid page tree value: %" PRIu16 " data size value out of bounds.",
				 function,
				 page_value_index );

				goto on_error;
			}
			byte_stream_copy_to_uint32_little_endian(
			 page_tree_value->data,
			 child_page_number );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: page value: %03" PRIu16 " child page number\t\t: %" PRIu32 "",
				 function,
				 page_value_index,
				 child_page_number );

				if( child_page_number == 0 )
				{
					libcnotify_printf(
					 " (invalid page number)\n" );
				}
				else if( child_page_number > space_tree->page_tree->io_handle->last_page_number )
				{
					libcnotify_printf(
					 " (exceeds last page number: %" PRIu32 ")\n",
					 space_tree->page_tree->io_handle->last_page_number );
				}
				libcnotify_printf(
				 "\n" );
				libcnotify_printf(
				 "\n" );
			}
#endif
#if ( SIZEOF_INT <= 4 )
			if( ( child_page_number < 1 )
			 || ( child_page_number > (uint32_t) INT_MAX ) )
#else
			if( ( child_page_number < 1 )
			 || ( (int) child_page_number > INT_MAX ) )
#endif
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid child page number value out of bounds.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( page_tree_value->data_size > 4 )
				{
					libcnotify_printf(
					 "%s: page value: %03" PRIu16 " trailing data:\n",
					 function,
					 page_value_index );
					libcnotify_print_data(
					 &( page_tree_value->data[ 4 ] ),
					 page_tree_value->data_size - 4,
					 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
				}
			}
#endif
			if( ( child_page_number > 0 )
			 && ( child_page_number <= space_tree->page_tree->io_handle->last_page_number ) )
			{
				if( libfdata_vector_get_element_value_by_index(
				     space_tree->page_tree->pages_vector,
				     (intptr_t *) file_io_handle,
				     (libfdata_cache_t *) child_page_cache,
				     (int) child_page_number - 1,
				     (intptr_t **) &child_page,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve page: %" PRIu32 ".",
					 function,
					 child_page_number );

					goto on_error;
				}
				if( libesedb_page_validate_space_tree_page(
				     child_page,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported page.",
					 function );

					goto on_error;
				}
				if( libesedb_space_tree_read_values_from_page(
				     space_tree,
				     file_io_handle,
				     child_page,
				     current_leaf_value_index,
				     recursion_depth + 1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to read values from page: %" PRIu32 ".",
					 function,
					 child_page_number );

					goto on_error;
				}
			}
		}
		if( libesedb_page_tree_value_free(
		     &page_tree_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free page tree value.",
			 function );

			goto on_error;
		}
	}
	if( libfcache_cache_free(
	     &child_page_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free child page cache.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: total number of pages\t\t: %" PRIu32 "\n",
		 function,
		 total_number_of_pages );

		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );

on_error:
	if( page_tree_value != NULL )
	{
		libesedb_page_tree_value_free(
		 &page_tree_value,
		 NULL );
	}
	if( space_tree_value != NULL )
	{
		libesedb_space_tree_value_free(
		 &space_tree_value,
		 NULL );
	}
	if( child_page_cache != NULL )
	{
		libfcache_cache_free(
		 &child_page_cache,
		 NULL );
	}
	return( -1 );
}

/* Reads the space tree
 * Returns 1 if successful or -1 on error
 */
int libesedb_space_tree_read_file_io_handle(
     libesedb_space_tree_t *space_tree,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libesedb_page_t *root_page         = NULL;
	libfcache_cache_t *root_page_cache = NULL;
	static char *function              = "libesedb_space_tree_read_file_io_handle";
	int current_leaf_value_index       = 0;

	if( space_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid space tree.",
		 function );

		return( -1 );
	}
	if( space_tree->page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid space tree - missing page tree.",
		 function );

		return( -1 );
	}
        /* Use a local cache to prevent cache invalidation of the root page
         * when reading child pages.
         */
	if( libfcache_cache_initialize(
	     &root_page_cache,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create root page cache.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_get_element_value_by_index(
	     space_tree->page_tree->pages_vector,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) root_page_cache,
	     (int) space_tree->page_tree->root_page_number - 1,
	     (intptr_t **) &root_page,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 ".",
		 function,
		 space_tree->page_tree->root_page_number );

		goto on_error;
	}
	if( libesedb_page_validate_root_page(
	     root_page,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported root page.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libesedb_page_tree_read_root_page_header(
	     space_tree->page_tree,
	     root_page,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read root page header.",
		 function );

		goto on_error;
	}
#endif
	if( libesedb_space_tree_read_values_from_page(
	     space_tree,
	     file_io_handle,
	     root_page,
	     &current_leaf_value_index,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to read values from root page.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_free(
	     &root_page_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free root page cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( root_page_cache != NULL )
	{
		libfcache_cache_free(
		 &root_page_cache,
		 NULL );
	}
	return( -1 );
}

