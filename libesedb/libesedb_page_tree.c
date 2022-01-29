/*
 * Page tree functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libesedb_data_definition.h"
#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_io_handle.h"
#include "libesedb_leaf_page_descriptor.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_page.h"
#include "libesedb_page_tree.h"
#include "libesedb_page_tree_key.h"
#include "libesedb_page_tree_value.h"
#include "libesedb_root_page_header.h"
#include "libesedb_space_tree.h"
#include "libesedb_table_definition.h"
#include "libesedb_unused.h"

#include "esedb_page_values.h"

/* Creates a page tree
 * Make sure the value page_tree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_initialize(
     libesedb_page_tree_t **page_tree,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     uint32_t object_identifier,
     uint32_t root_page_number,
     libesedb_table_definition_t *table_definition,
     libesedb_table_definition_t *template_table_definition,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_tree_initialize";

	if( page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( *page_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid page tree value already set.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
#if ( SIZEOF_INT <= 4 )
	if( ( root_page_number < 1 )
	 || ( root_page_number > (uint32_t) INT_MAX ) )
#else
	if( ( root_page_number < 1 )
	 || ( (int) root_page_number > INT_MAX ) )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid root page number value out of bounds.",
		 function );

		return( -1 );
	}
	*page_tree = memory_allocate_structure(
	              libesedb_page_tree_t );

	if( *page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create page tree.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *page_tree,
	     0,
	     sizeof( libesedb_page_tree_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear page tree.",
		 function );

		memory_free(
		 *page_tree );

		*page_tree = NULL;

		return( -1 );
	}
	if( libcdata_btree_initialize(
	     &( ( *page_tree )->leaf_page_descriptors_tree ),
	     257,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create leaf page descriptors B-tree.",
		 function );

		goto on_error;
	}
	( *page_tree )->io_handle                 = io_handle;
	( *page_tree )->pages_vector              = pages_vector;
	( *page_tree )->pages_cache               = pages_cache;
	( *page_tree )->object_identifier         = object_identifier;
	( *page_tree )->root_page_number          = root_page_number;
	( *page_tree )->table_definition          = table_definition;
	( *page_tree )->template_table_definition = template_table_definition;
	( *page_tree )->number_of_leaf_values     = -1;

	return( 1 );

on_error:
	if( *page_tree != NULL )
	{
		memory_free(
		 *page_tree );

		*page_tree = NULL;
	}
	return( -1 );
}

/* Frees a page tree
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_free(
     libesedb_page_tree_t **page_tree,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_tree_free";
	int result            = 1;

	if( page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( *page_tree != NULL )
	{
		/* The io_handle, pages_vector, pages_cache, table_definition and template_table_definition references
		 * are freed elsewhere
		 */
		if( ( *page_tree )->root_page_header != NULL )
		{
			if( libesedb_root_page_header_free(
			     &( ( *page_tree )->root_page_header ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free root page header.",
				 function );

				result = -1;
			}
		}
		if( libcdata_btree_free(
		     &( ( *page_tree )->leaf_page_descriptors_tree ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libesedb_leaf_page_descriptor_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free leaf page descriptors B-tree.",
			 function );

			result = -1;
		}
		memory_free(
		 *page_tree );

		*page_tree = NULL;
	}
	return( result );
}

/* Reads the root page header
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_root_page_header(
     libesedb_page_tree_t *page_tree,
     libesedb_page_t *root_page,
     libcerror_error_t **error )
{
	libesedb_page_value_t *page_value = NULL;
	static char *function             = "libesedb_page_tree_read_root_page_header";
	uint16_t number_of_page_values    = 0;

	if( page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( libesedb_page_get_number_of_values(
	     root_page,
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
	if( number_of_page_values > 0 )
	{
		if( libesedb_page_get_value_by_index(
		     root_page,
		     0,
		     &page_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root page value: 0.",
			 function );

			goto on_error;
		}
		if( page_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing page value.",
			 function );

			goto on_error;
		}
		if( libesedb_root_page_header_initialize(
		     &( page_tree->root_page_header ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create root page header.",
			 function );

			goto on_error;
		}
		if( libesedb_root_page_header_read_data(
		     page_tree->root_page_header,
		     page_value->data,
		     (size_t) page_value->size,
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
	}
	return( 1 );

on_error:
	if( page_tree->root_page_header != NULL )
	{
		libesedb_root_page_header_free(
		 &( page_tree->root_page_header ),
		 NULL );
	}
	return( -1 );
}

/* Reads the space page trees
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_space_trees(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libesedb_space_tree_t *space_tree = NULL;
	static char *function             = "libesedb_page_tree_read_space_trees";

	if( page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( page_tree->root_page_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - missing root page heder.",
		 function );

		return( -1 );
	}
	/* Read the space tree pages
	 */
	if( page_tree->root_page_header->extent_space > 0 )
	{
		if( page_tree->root_page_header->space_tree_page_number >= 0xff000000UL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported space tree page number.",
			 function,
			 page_tree->root_page_header->space_tree_page_number );

			goto on_error;
		}
		if( page_tree->root_page_header->space_tree_page_number > 0 )
		{
			/* Read the owned pages space tree page
			 */
			if( libesedb_space_tree_initialize(
			     &space_tree,
			     page_tree->io_handle,
			     page_tree->object_identifier,
			     page_tree->root_page_header->space_tree_page_number,
			     page_tree->pages_vector,
			     page_tree->pages_cache,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create owned pages space tree.",
				 function );

				goto on_error;
			}
			if( libesedb_space_tree_read_file_io_handle(
			     space_tree,
			     file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read owned pages space tree.",
				 function );

				goto on_error;
			}
			if( libesedb_space_tree_free(
			     &space_tree,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free space tree.",
				 function );

				goto on_error;
			}
			/* Read the available pages space tree page
			 */
			if( libesedb_space_tree_initialize(
			     &space_tree,
			     page_tree->io_handle,
			     page_tree->object_identifier,
			     page_tree->root_page_header->space_tree_page_number + 1,
			     page_tree->pages_vector,
			     page_tree->pages_cache,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create available pages space tree.",
				 function );

				goto on_error;
			}
			if( libesedb_space_tree_read_file_io_handle(
			     space_tree,
			     file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read available pages space tree.",
				 function );

				goto on_error;
			}
			if( libesedb_space_tree_free(
			     &space_tree,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free space tree.",
				 function );

				goto on_error;
			}
		}
	}
	return( 1 );

on_error:
	if( space_tree != NULL )
	{
		libesedb_space_tree_free(
		 &space_tree,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the page tree key of a specific page value
 * This function creates a key
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_get_key(
     libesedb_page_tree_t *page_tree,
     libesedb_page_tree_value_t *page_tree_value,
     libesedb_page_t *page,
     uint32_t page_flags,
     uint16_t page_value_index,
     libesedb_page_tree_key_t **key,
     libcerror_error_t **error )
{
	libesedb_page_tree_key_t *safe_key       = NULL;
	libesedb_page_value_t *header_page_value = NULL;
	static char *function                    = "libesedb_page_tree_get_key";

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *page_key_data                   = NULL;
	size_t page_key_size                     = 0;
#endif

	if( page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
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
	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( libesedb_page_tree_key_initialize(
	     &safe_key,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create key.",
		 function );

		goto on_error;
	}
	if( page_tree_value->common_key_size > 0 )
	{
		if( libesedb_page_get_value_by_index(
		     page,
		     0,
		     &header_page_value,
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
		if( header_page_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing page value: 0.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: page value: 000 page tag flags\t\t: 0x%02" PRIx8 "",
			 function,
			 header_page_value->flags );
			libesedb_debug_print_page_tag_flags(
			 header_page_value->flags );
			libcnotify_printf(
			 "\n" );

			libcnotify_printf(
			 "%s: page value: 000 data:\n",
			 function );
			libcnotify_print_data(
			 header_page_value->data,
			 (size_t) header_page_value->size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
		if( page_tree_value->common_key_size > header_page_value->size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: common key size exceeds header page value size.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: page value: %03" PRIu16 " common key value\t\t: ",
			 function,
			 page_value_index );

			page_key_data = header_page_value->data;
			page_key_size = page_tree_value->common_key_size;

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

		if( libesedb_page_tree_key_set_data(
		     safe_key,
		     header_page_value->data,
		     (size_t) page_tree_value->common_key_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set common key data in key.",
			 function );

			goto on_error;
		}
	}
	if( libesedb_page_tree_key_append_data(
	     safe_key,
	     page_tree_value->local_key,
	     (size_t) page_tree_value->local_key_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to append local key data to key.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: page value: %03" PRIu16 " key value\t\t\t: ",
		 function,
		 page_value_index );

		page_key_data = safe_key->data;
		page_key_size = safe_key->data_size;

		while( page_key_size > 0 )
		{
			libcnotify_printf(
			 "%02" PRIx8 " ",
			 *page_key_data );

			page_key_data++;
			page_key_size--;
		}
		libcnotify_printf(
		 "\n\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) != 0 )
	{
		safe_key->type = LIBESEDB_KEY_TYPE_LEAF;
	}
	else
	{
		safe_key->type = LIBESEDB_KEY_TYPE_BRANCH;
	}
	*key = safe_key;

	return( 1 );

on_error:
	if( safe_key != NULL )
	{
		libesedb_page_tree_key_free(
		 &safe_key,
		 NULL );
	}
	return( -1 );
}

/* Determines the first leaf page number from a page
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_get_first_leaf_page_number_from_page(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     libesedb_page_t *page,
     uint32_t *leaf_page_number,
     int recursion_depth,
     libcerror_error_t **error )
{
	libesedb_page_t *child_page                 = NULL;
	libesedb_page_tree_value_t *page_tree_value = NULL;
	libesedb_page_value_t *page_value           = NULL;
	static char *function                       = "libesedb_page_tree_get_first_leaf_page_number_from_page";
	uint32_t child_page_number                  = 0;
	uint32_t page_flags                         = 0;
	uint16_t number_of_page_values              = 0;
	uint16_t page_value_index                   = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libesedb_page_tree_key_t *page_value_key    = NULL;
#endif

	if( page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( page_tree->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - missing IO handle.",
		 function );

		return( -1 );
	}
	if( leaf_page_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid leaf page number.",
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
	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) != 0 )
	{
		*leaf_page_number = page->page_number;

		return( 1 );
	}
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
			 "%s: page value: %03" PRIu16 " page tag flags\t\t: 0x%02" PRIx8 "",
			 function,
			 page_value_index,
			 page_value->flags );
			libesedb_debug_print_page_tag_flags(
			 page_value->flags );
			libcnotify_printf(
			 "\n" );
		}
#endif
		if( ( page_value->flags & LIBESEDB_PAGE_TAG_FLAG_IS_DEFUNCT ) != 0 )
		{
/* TODO are defunct data definition of any value recovering */
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: page value: %03" PRIu16 " data:\n",
				 function,
				 page_value_index );
				libcnotify_print_data(
				 page_value->data,
				 (size_t) page_value->size,
				 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
			}
#endif
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
#if defined( HAVE_DEBUG_OUTPUT )
		if( libesedb_page_tree_get_key(
		     page_tree,
		     page_tree_value,
		     page,
		     page_flags,
		     page_value_index,
		     &page_value_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve key of page value: %" PRIu16 ".",
			 function,
			 page_value_index );

			goto on_error;
		}
		if( libesedb_page_tree_key_free(
		     &page_value_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free page value key.",
			 function );

			goto on_error;
		}
#endif
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
			 "%s: page value: %03" PRIu16 " child page number\t: %" PRIu32 "",
			 function,
			 page_value_index,
			 child_page_number );

			if( child_page_number == 0 )
			{
				libcnotify_printf(
				 " (invalid page number)\n" );
			}
			else if( child_page_number > page_tree->io_handle->last_page_number )
			{
				libcnotify_printf(
				 " (exceeds last page number: %" PRIu32 ")\n",
				 page_tree->io_handle->last_page_number );
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
		 && ( child_page_number <= page_tree->io_handle->last_page_number ) )
		{
			if( libfdata_vector_get_element_value_by_index(
			     page_tree->pages_vector,
			     (intptr_t *) file_io_handle,
			     (libfdata_cache_t *) page_tree->pages_cache,
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
			if( libesedb_page_validate_page(
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
			if( libesedb_page_tree_get_first_leaf_page_number_from_page(
			     page_tree,
			     file_io_handle,
			     child_page,
			     leaf_page_number,
			     recursion_depth + 1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve first leaf page number from page: %" PRIu32 ".",
				 function,
				 child_page_number );

				goto on_error;
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
		break;
	}
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( page_value_key != NULL )
	{
		libesedb_page_tree_key_free(
		 &page_value_key,
		 NULL );
	}
#endif
	if( page_tree_value != NULL )
	{
		libesedb_page_tree_value_free(
		 &page_tree_value,
		 NULL );
	}
	return( -1 );
}

/* Determines the first leaf page number
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_get_get_first_leaf_page_number(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     uint32_t *leaf_page_number,
     libcerror_error_t **error )
{
	libesedb_page_t *page          = NULL;
	static char *function          = "libesedb_page_tree_get_get_first_leaf_page_number";
	uint32_t last_leaf_page_number = 0;
	uint32_t page_flags            = 0;
	uint32_t safe_leaf_page_number = 0;
	int recursion_depth            = 0;

	if( page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( leaf_page_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid leaf page number.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_by_index(
	     page_tree->pages_vector,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) page_tree->pages_cache,
	     (int) page_tree->root_page_number - 1,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 ".",
		 function,
		 page_tree->root_page_number );

		return( -1 );
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

		return( -1 );
	}
	/* Seen in temp.edb where is root flag is not set
	 */
	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_ROOT ) != 0 )
	{
		if( libesedb_page_validate_root_page(
		     page,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported root page.",
			 function );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( TODO )
	if( page_tree->root_page_header == NULL )
	{
		if( libesedb_page_tree_read_root_page_header(
		     page_tree,
		     page,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read root page header.",
			 function );

			return( -1 );
		}
		if( libesedb_page_tree_read_space_trees(
		     page_tree,
		     file_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read space trees.",
			 function );

			return( -1 );
		}
	}
#endif
	if( libesedb_page_tree_get_first_leaf_page_number_from_page(
	     page_tree,
	     file_io_handle,
	     page,
	     &safe_leaf_page_number,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve first leaf page number from page: %" PRIu32 ".",
		 function,
		 page_tree->root_page_number );

		return( -1 );
	}
	while( safe_leaf_page_number != 0 )
	{
		if( recursion_depth > LIBESEDB_MAXIMUM_LEAF_PAGE_RECURSION_DEPTH )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid recursion depth value out of bounds.",
			 function );

			return( -1 );
		}
#if ( SIZEOF_INT <= 4 )
		if( safe_leaf_page_number > (uint32_t) INT_MAX )
#else
		if( safe_leaf_page_number > (unsigned int) INT_MAX )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid leaf page number value out of bounds.",
			 function );

			return( -1 );
		}
		last_leaf_page_number = safe_leaf_page_number;

		if( libfdata_vector_get_element_value_by_index(
		     page_tree->pages_vector,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) page_tree->pages_cache,
		     (int) safe_leaf_page_number - 1,
		     (intptr_t **) &page,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page: %" PRIu32 ".",
			 function,
			 safe_leaf_page_number );

			return( -1 );
		}
		if( libesedb_page_get_previous_page_number(
		     page,
		     &safe_leaf_page_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve previous page number from page: %" PRIu32 ".",
			 function,
			 safe_leaf_page_number );

			return( -1 );
		}
		recursion_depth++;
	}
	*leaf_page_number = last_leaf_page_number;

	return( 1 );
}

/* Determines the number of leaf values from a page
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_get_number_of_leaf_values_from_leaf_page(
     libesedb_page_tree_t *page_tree,
     libesedb_page_t *page,
     int *number_of_leaf_values,
     libcerror_error_t **error )
{
	libesedb_page_value_t *page_value = NULL;
	static char *function             = "libesedb_page_tree_get_number_of_leaf_values_from_leaf_page";
	uint32_t page_flags               = 0;
	uint16_t number_of_page_values    = 0;
	uint16_t page_value_index         = 0;
	int safe_number_of_leaf_values    = 0;

	if( page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( number_of_leaf_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of leaf values.",
		 function );

		return( -1 );
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

		return( -1 );
	}
	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported page - not a leaf page.",
		 function );

		return( -1 );
	}
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

		return( -1 );
	}
	safe_number_of_leaf_values = *number_of_leaf_values;

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

			return( -1 );
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

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: page value: %03" PRIu16 " page tag flags\t\t: 0x%02" PRIx8 "",
			 function,
			 page_value_index,
			 page_value->flags );
			libesedb_debug_print_page_tag_flags(
			 page_value->flags );
			libcnotify_printf(
			 "\n" );
		}
#endif
		if( ( page_value->flags & LIBESEDB_PAGE_TAG_FLAG_IS_DEFUNCT ) != 0 )
		{
/* TODO are defunct data definition of any value recovering */
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: page value: %03" PRIu16 " data:\n",
				 function,
				 page_value_index );
				libcnotify_print_data(
				 page_value->data,
				 (size_t) page_value->size,
				 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
			}
#endif
			continue;
		}
		if( safe_number_of_leaf_values == INT_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid number of leaf values value out of bounds.",
			 function );

			return( -1 );
		}
		safe_number_of_leaf_values++;
	}
	*number_of_leaf_values = safe_number_of_leaf_values;

	return( 1 );
}

/* Determines the number of leaf values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_get_number_of_leaf_values(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     int *number_of_leaf_values,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *upper_node                               = NULL;
	libesedb_leaf_page_descriptor_t *existing_leaf_page_descriptor = NULL;
	libesedb_leaf_page_descriptor_t *leaf_page_descriptor          = NULL;
	libesedb_page_t *page                                          = NULL;
	static char *function                                          = "libesedb_page_tree_get_number_of_leaf_values";
	uint32_t leaf_page_number                                      = 0;
	int number_of_leaf_pages                                       = 0;
	int safe_number_of_leaf_values                                 = 0;
	int value_index                                                = 0;

	if( page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( number_of_leaf_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of leaf values.",
		 function );

		return( -1 );
	}
	if( page_tree->number_of_leaf_values == -1 )
	{
		page_tree->number_of_leaf_values = 0;

		if( libesedb_page_tree_get_get_first_leaf_page_number(
		     page_tree,
		     file_io_handle,
		     &leaf_page_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve first leaf page number from page tree.",
			 function );

			goto on_error;
		}
		while( leaf_page_number != 0 )
		{
			if( number_of_leaf_pages > LIBESEDB_MAXIMUM_NUMBER_OF_LEAF_PAGES )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
				 "%s: invalid number of leaf pages value exceeds maximum.",
				 function );

				goto on_error;
			}
#if ( SIZEOF_INT <= 4 )
			if( leaf_page_number > (uint32_t) INT_MAX )
#else
			if( leaf_page_number > (unsigned int) INT_MAX )
#endif
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid leaf page number value out of bounds.",
				 function );

				goto on_error;
			}
			if( libfdata_vector_get_element_value_by_index(
			     page_tree->pages_vector,
			     (intptr_t *) file_io_handle,
			     (libfdata_cache_t *) page_tree->pages_cache,
			     (int) leaf_page_number - 1,
			     (intptr_t **) &page,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve page: %" PRIu32 ".",
				 function,
				 leaf_page_number );

				goto on_error;
			}
			if( libesedb_page_tree_get_number_of_leaf_values_from_leaf_page(
			     page_tree,
			     page,
			     &safe_number_of_leaf_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine number of leaf values from page: %" PRIu32 ".",
				 function,
				 leaf_page_number );

				goto on_error;
			}
			if( libesedb_leaf_page_descriptor_initialize(
			     &leaf_page_descriptor,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create leaf page descriptor.",
				 function );

				goto on_error;
			}
			leaf_page_descriptor->page_number            = page->page_number;
			leaf_page_descriptor->first_leaf_value_index = page_tree->number_of_leaf_values;
			leaf_page_descriptor->last_leaf_value_index  = safe_number_of_leaf_values - 1;

			if( libcdata_btree_insert_value(
			     page_tree->leaf_page_descriptors_tree,
			     &value_index,
			     (intptr_t *) leaf_page_descriptor,
			     (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libesedb_leaf_page_descriptor_compare,
			     &upper_node,
			     (intptr_t **) &existing_leaf_page_descriptor,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable insert leaf page descriptor into tree.",
				 function );

				goto on_error;
			}
			leaf_page_descriptor = NULL;

			if( libesedb_page_get_next_page_number(
			     page,
			     &leaf_page_number,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve next page number from page: %" PRIu32 ".",
				 function,
				 leaf_page_number );

				goto on_error;
			}
			page_tree->number_of_leaf_values = safe_number_of_leaf_values;

			number_of_leaf_pages++;
		}
	}
	*number_of_leaf_values = page_tree->number_of_leaf_values;

	return( 1 );

on_error:
	if( leaf_page_descriptor != NULL )
	{
		libesedb_leaf_page_descriptor_free(
		 &leaf_page_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a specific leaf value
 * This function creates a new data definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_get_leaf_value_by_index_from_page(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     libesedb_page_t *page,
     int leaf_value_index,
     libesedb_data_definition_t **data_definition,
     int *current_leaf_value_index,
     int recursion_depth,
     libcerror_error_t **error )
{
	libesedb_page_t *child_page                 = NULL;
	libesedb_page_tree_value_t *page_tree_value = NULL;
	libesedb_page_value_t *page_value           = NULL;
	libfcache_cache_t *child_page_cache         = NULL;
	static char *function                       = "libesedb_page_tree_get_leaf_value_by_index_from_page";
	uint32_t child_page_number                  = 0;
	uint32_t page_flags                         = 0;
	uint16_t data_offset                        = 0;
	uint16_t number_of_page_values              = 0;
	uint16_t page_value_index                   = 0;

	if( page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( page_tree->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - missing IO handle.",
		 function );

		return( -1 );
	}
	if( data_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data definition.",
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
			 "%s: page value: %03" PRIu16 " page tag flags\t\t: 0x%02" PRIx8 "",
			 function,
			 page_value_index,
			 page_value->flags );
			libesedb_debug_print_page_tag_flags(
			 page_value->flags );
			libcnotify_printf(
			 "\n" );
		}
#endif
		if( ( page_value->flags & LIBESEDB_PAGE_TAG_FLAG_IS_DEFUNCT ) != 0 )
		{
			continue;
		}
		if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) != 0 )
		{
			if( *current_leaf_value_index == INT_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid current leaf value index value out of bounds.",
				 function );

				goto on_error;
			}
			if( *current_leaf_value_index < leaf_value_index )
			{
				*current_leaf_value_index += 1;

				continue;
			}
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
			if( libesedb_data_definition_initialize(
			     data_definition,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create data definition.",
				 function );

				goto on_error;
			}
			data_offset = page_value->offset + 2 + page_tree_value->local_key_size;

			if( ( page_value->flags & LIBESEDB_PAGE_TAG_FLAG_HAS_COMMON_KEY_SIZE ) != 0 )
			{
				data_offset += 2;
			}
			( *data_definition )->page_value_index = page_value_index;
			( *data_definition )->page_offset      = page->offset - ( 2 * page_tree->io_handle->page_size );
			( *data_definition )->page_number      = page->page_number;
			( *data_definition )->data_offset      = data_offset;
			( *data_definition )->data_size        = page_tree_value->data_size;

			*current_leaf_value_index += 1;
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
				 "%s: page value: %03" PRIu16 " child page number\t: %" PRIu32 "",
				 function,
				 page_value_index,
				 child_page_number );

				if( child_page_number == 0 )
				{
					libcnotify_printf(
					 " (invalid page number)\n" );
				}
				else if( child_page_number > page_tree->io_handle->last_page_number )
				{
					libcnotify_printf(
					 " (exceeds last page number: %" PRIu32 ")\n",
					 page_tree->io_handle->last_page_number );
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
			 && ( child_page_number <= page_tree->io_handle->last_page_number ) )
			{
				if( libfdata_vector_get_element_value_by_index(
				     page_tree->pages_vector,
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
				if( libesedb_page_validate_page(
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
				if( libesedb_page_tree_get_leaf_value_by_index_from_page(
				     page_tree,
				     file_io_handle,
				     child_page,
				     leaf_value_index,
				     data_definition,
				     current_leaf_value_index,
				     recursion_depth + 1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve leaf value: %d from page: %" PRIu32 ".",
					 function,
					 leaf_value_index,
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
		if( *current_leaf_value_index > leaf_value_index )
		{
			break;
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
	return( 1 );

on_error:
	if( *data_definition != NULL )
	{
		libesedb_data_definition_free(
		 data_definition,
		 NULL );
	}
	if( page_tree_value != NULL )
	{
		libesedb_page_tree_value_free(
		 &page_tree_value,
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

/* Retrieves a specific leaf value
 * This function creates a new data definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_get_leaf_value_by_index(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     int leaf_value_index,
     libesedb_data_definition_t **data_definition,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *upper_node                               = NULL;
	libesedb_leaf_page_descriptor_t *existing_leaf_page_descriptor = NULL;
	libesedb_leaf_page_descriptor_t *leaf_page_descriptor          = NULL;
	libesedb_page_t *base_page                                     = NULL;
	libfcache_cache_t *base_page_cache                             = NULL;
	static char *function                                          = "libesedb_page_tree_get_leaf_value_by_index";
	uint32_t base_page_number                                      = 0;
	int current_leaf_value_index                                   = 0;
	int number_of_leaf_values                                      = 0;
	int result                                                     = 0;

	if( page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( leaf_value_index < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid leaf value index value out of bounds.",
		 function );

		goto on_error;
	}
	if( data_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data definition.",
		 function );

		return( -1 );
	}
	if( page_tree->number_of_leaf_values == -1 )
	{
		/* libesedb_page_tree_get_number_of_leaf_values is called to build the leaf_page_descriptors_tree
		 */
		if( libesedb_page_tree_get_number_of_leaf_values(
		     page_tree,
		     file_io_handle,
		     &number_of_leaf_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of leaf values from page: %" PRIu32 ".",
			 function,
			 page_tree->root_page_number );

			goto on_error;
		}
	}
	base_page_number = page_tree->root_page_number;

	if( libesedb_leaf_page_descriptor_initialize(
	     &leaf_page_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create leaf page descriptor.",
		 function );

		goto on_error;
	}
	leaf_page_descriptor->first_leaf_value_index = leaf_value_index;
	leaf_page_descriptor->last_leaf_value_index  = leaf_value_index;

	result = libcdata_btree_get_value_by_value(
	          page_tree->leaf_page_descriptors_tree,
	          (intptr_t *) leaf_page_descriptor,
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libesedb_leaf_page_descriptor_compare,
	          &upper_node,
	          (intptr_t **) &existing_leaf_page_descriptor,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve leaf page descriptor from tree.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( existing_leaf_page_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing existing leaf page descriptor.",
			 function );

			goto on_error;
		}
		current_leaf_value_index = existing_leaf_page_descriptor->first_leaf_value_index;
		base_page_number         = existing_leaf_page_descriptor->page_number;
	}
	if( libesedb_leaf_page_descriptor_free(
	     &leaf_page_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free leaf page descriptor.",
		 function );

		goto on_error;
	}
        /* Use a local cache to prevent cache invalidation of the root page
         * when reading child pages.
         */
	if( libfcache_cache_initialize(
	     &base_page_cache,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create base page cache.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_get_element_value_by_index(
	     page_tree->pages_vector,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) base_page_cache,
	     (int) base_page_number - 1,
	     (intptr_t **) &base_page,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 ".",
		 function,
		 base_page_number );

		goto on_error;
	}
	if( base_page_number == page_tree->root_page_number )
	{
		if( libesedb_page_validate_root_page(
		     base_page,
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
	}
	if( libesedb_page_tree_get_leaf_value_by_index_from_page(
	     page_tree,
	     file_io_handle,
	     base_page,
	     leaf_value_index,
	     data_definition,
	     &current_leaf_value_index,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve leaf value: %d from page: %" PRIu32 ".",
		 function,
		 leaf_value_index,
		 base_page_number );

		goto on_error;
	}
	if( current_leaf_value_index != ( leaf_value_index + 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid current leaf value index value out of bounds.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_free(
	     &base_page_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free base page cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *data_definition != NULL )
	{
		libesedb_data_definition_free(
		 data_definition,
		 NULL );
	}
	if( leaf_page_descriptor != NULL )
	{
		libesedb_leaf_page_descriptor_free(
		 &leaf_page_descriptor,
		 NULL );
	}
	if( base_page_cache != NULL )
	{
		libfcache_cache_free(
		 &base_page_cache,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a specific leaf value
 * This function creates a new data definition
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libesedb_page_tree_get_leaf_value_by_key_from_page(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     libesedb_page_t *page,
     libesedb_page_tree_key_t *leaf_value_key,
     libesedb_data_definition_t **data_definition,
     int recursion_depth,
     libcerror_error_t **error )
{
	libesedb_page_tree_key_t *page_value_key    = NULL;
	libesedb_page_t *child_page                 = NULL;
	libesedb_page_tree_value_t *page_tree_value = NULL;
	libesedb_page_value_t *page_value           = NULL;
	libfcache_cache_t *child_page_cache         = NULL;
	static char *function                       = "libesedb_page_tree_get_leaf_value_by_key_from_page";
	uint32_t child_page_number                  = 0;
	uint32_t page_flags                         = 0;
	uint16_t data_offset                        = 0;
	uint16_t number_of_page_values              = 0;
	uint16_t page_value_index                   = 0;
	int compare_result                          = 0;
	int result                                  = 0;

	if( page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( page_tree->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - missing IO handle.",
		 function );

		return( -1 );
	}
	if( data_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data definition.",
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
			 "%s: page value: %03" PRIu16 " page tag flags\t\t: 0x%02" PRIx8 "",
			 function,
			 page_value_index,
			 page_value->flags );
			libesedb_debug_print_page_tag_flags(
			 page_value->flags );
			libcnotify_printf(
			 "\n" );
		}
#endif
		if( ( page_value->flags & LIBESEDB_PAGE_TAG_FLAG_IS_DEFUNCT ) != 0 )
		{
			continue;
		}
		if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_ROOT ) != 0 )
		{
			if( ( page_value->flags & LIBESEDB_PAGE_TAG_FLAG_HAS_COMMON_KEY_SIZE ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported page - root flag is set and page value: %" PRIu16 " has common key.",
				 function,
				 page_value_index );

				goto on_error;
			}
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
		if( libesedb_page_tree_get_key(
		     page_tree,
		     page_tree_value,
		     page,
		     page_flags,
		     page_value_index,
		     &page_value_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve key of page value: %" PRIu16 ".",
			 function,
			 page_value_index );

			goto on_error;
		}
		compare_result = libesedb_page_tree_key_compare(
		                  leaf_value_key,
		                  page_value_key,
		                  error );

		if( compare_result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare page value: %" PRIu16 " and leaf value keys.",
			 function,
			 page_value_index );

			goto on_error;
		}
		if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) != 0 )
		{
			if( compare_result == LIBFDATA_COMPARE_EQUAL )
			{
				if( libesedb_data_definition_initialize(
				     data_definition,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create data definition.",
					 function );

					goto on_error;
				}
				data_offset = page_value->offset + 2 + page_tree_value->local_key_size;

				if( ( page_value->flags & LIBESEDB_PAGE_TAG_FLAG_HAS_COMMON_KEY_SIZE ) != 0 )
				{
					data_offset += 2;
				}
				( *data_definition )->page_value_index = page_value_index;
				( *data_definition )->page_offset      = page->offset - ( 2 * page_tree->io_handle->page_size );
				( *data_definition )->page_number      = page->page_number;
				( *data_definition )->data_offset      = data_offset;
				( *data_definition )->data_size        = page_tree_value->data_size;

				result = 1;
			}
		}
		else if( compare_result != LIBFDATA_COMPARE_GREATER )
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
				 "%s: page value: %03" PRIu16 " child page number\t: %" PRIu32 "",
				 function,
				 page_value_index,
				 child_page_number );

				if( child_page_number == 0 )
				{
					libcnotify_printf(
					 " (invalid page number)\n" );
				}
				else if( child_page_number > page_tree->io_handle->last_page_number )
				{
					libcnotify_printf(
					 " (exceeds last page number: %" PRIu32 ")\n",
					 page_tree->io_handle->last_page_number );
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
			 && ( child_page_number <= page_tree->io_handle->last_page_number ) )
			{
				if( libfdata_vector_get_element_value_by_index(
				     page_tree->pages_vector,
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
				if( libesedb_page_validate_page(
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
				result = libesedb_page_tree_get_leaf_value_by_key_from_page(
				          page_tree,
				          file_io_handle,
				          child_page,
				          leaf_value_key,
				          data_definition,
				          recursion_depth + 1,
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve leaf value from page: %" PRIu32 ".",
					 function,
					 child_page_number );

					goto on_error;
				}
			}
		}
		if( libesedb_page_tree_key_free(
		     &page_value_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free page value key.",
			 function );

			goto on_error;
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
		if( result != 0 )
		{
			break;
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
	return( result );

on_error:
	if( *data_definition != NULL )
	{
		libesedb_data_definition_free(
		 data_definition,
		 NULL );
	}
	if( page_value_key != NULL )
	{
		libesedb_page_tree_key_free(
		 &page_value_key,
		 NULL );
	}
	if( page_tree_value != NULL )
	{
		libesedb_page_tree_value_free(
		 &page_tree_value,
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

/* Retrieves a specific leaf value
 * This function creates a new data definition
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libesedb_page_tree_get_leaf_value_by_key(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     libesedb_page_tree_key_t *leaf_value_key,
     libesedb_data_definition_t **data_definition,
     libcerror_error_t **error )
{
	libesedb_page_t *root_page         = NULL;
	libfcache_cache_t *root_page_cache = NULL;
	static char *function              = "libesedb_page_tree_get_leaf_value_by_key";
	int result                         = 0;

	if( page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
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
	     page_tree->pages_vector,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) root_page_cache,
	     (int) page_tree->root_page_number - 1,
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
		 page_tree->root_page_number );

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
	if( page_tree->root_page_header == NULL )
	{
		if( libesedb_page_tree_read_root_page_header(
		     page_tree,
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
	}
#endif
	result = libesedb_page_tree_get_leaf_value_by_key_from_page(
	          page_tree,
	          file_io_handle,
	          root_page,
	          leaf_value_key,
	          data_definition,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve leaf value from page: %" PRIu32 ".",
		 function,
		 page_tree->root_page_number );

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
	return( result );

on_error:
	if( root_page_cache != NULL )
	{
		libfcache_cache_free(
		 &root_page_cache,
		 NULL );
	}
	return( -1 );
}

#ifdef TODO
/* TODO refactor */

	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) != 0 )
	{
			if( libcnotify_verbose != 0 )
			{
				if( page_value_index > 1 )
				{
					if( child_page->page_number != previous_next_child_page_number )
					{
						libcnotify_printf(
						 "%s: mismatch in child page number (%" PRIu32 " != %" PRIu32 ").\n",
						 function,
						 previous_next_child_page_number,
						 child_page->page_number );
					}
					if( child_page->header->previous_page_number != previous_child_page_number )
					{
						libcnotify_printf(
						 "%s: mismatch in previous child page number (%" PRIu32 " != %" PRIu32 ").\n",
						 function,
						 previous_child_page_number,
						 child_page->header->previous_page_number );
					}
				}
/* TODO need the actual values for the following checks
				if( page_value_index == 1 )
				{
					if( child_page->header->previous_page_number != 0 )
					{
						libcnotify_printf(
						 "%s: mismatch in previous child page number (%" PRIu32 " != %" PRIu32 ").",
						 function,
						 0,
						 child_page->header->previous_page_number );
					}
				}
				if( page_value_index == ( number_of_page_values - 1 ) )
				{
					if( child_page->header->next_page_number != 0 )
					{
						libcnotify_printf(
						 "%s: mismatch in next child page number (%" PRIu32 " != %" PRIu32 ").",
						 function,
						 0,
						 child_page->header->previous_page_number );
					}
				}
*/
			}
			previous_child_page_number      = child_page->page_number;
			previous_next_child_page_number = child_page->header->next_page_number;
	}
#endif /* TODO */

