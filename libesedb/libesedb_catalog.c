/*
 * Catalog functions
 *
 * Copyright (C) 2009-2019, Joachim Metz <joachim.metz@gmail.com>
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

#include "libesedb_data_definition.h"
#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_catalog.h"
#include "libesedb_catalog_definition.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcdata.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_libuna.h"
#include "libesedb_page_tree.h"
#include "libesedb_table_definition.h"

/* Creates a catalog
 * Make sure the value catalog is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_initialize(
     libesedb_catalog_t **catalog,
     libesedb_io_handle_t *io_handle,
     uint32_t root_page_number,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     libcerror_error_t **error )
{
	static char *function = "libesedb_catalog_initialize";

	if( catalog == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog.",
		 function );

		return( -1 );
	}
	if( *catalog != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid catalog value already set.",
		 function );

		return( -1 );
	}
	*catalog = memory_allocate_structure(
	            libesedb_catalog_t );

	if( *catalog == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create catalog.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *catalog,
	     0,
	     sizeof( libesedb_catalog_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear catalog.",
		 function );

		memory_free(
		 *catalog );

		*catalog = NULL;

		return( -1 );
	}
	if( libesedb_page_tree_initialize(
	     &( ( *catalog )->page_tree ),
	     io_handle,
	     pages_vector,
	     pages_cache,
	     LIBESEDB_FDP_OBJECT_IDENTIFIER_CATALOG,
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
	if( libcdata_list_initialize(
	     &( ( *catalog )->table_definition_list ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create table definition list.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *catalog != NULL )
	{
		if( ( *catalog )->page_tree != NULL )
		{
			libesedb_page_tree_free(
			 &( ( *catalog )->page_tree ),
			 NULL );
		}
		memory_free(
		 *catalog );

		*catalog = NULL;
	}
	return( -1 );
}

/* Frees a catalog
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_free(
     libesedb_catalog_t **catalog,
     libcerror_error_t **error )
{
	static char *function = "libesedb_catalog_free";
	int result            = 1;

	if( catalog == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog.",
		 function );

		return( -1 );
	}
	if( *catalog != NULL )
	{
		if( libesedb_page_tree_free(
		     &( ( *catalog )->page_tree ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free catalog page tree.",
			 function );

			result = -1;
		}
		if( libcdata_list_free(
		     &( ( *catalog )->table_definition_list ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libesedb_table_definition_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free table definition list.",
			 function );

			result = -1;
		}
		memory_free(
		 *catalog );

		*catalog = NULL;
	}
	return( result );
}

/* Reads a catalog value
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_read_value_data(
     libesedb_catalog_t *catalog,
     const uint8_t *data,
     size_t data_size,
     libesedb_table_definition_t **table_definition,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *catalog_definition = NULL;
	static char *function                             = "libesedb_catalog_read_value_data";

	if( catalog == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog.",
		 function );

		return( -1 );
	}
	if( catalog->page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid catalog - missing page tree.",
		 function );

		return( -1 );
	}
	if( catalog->page_tree->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid catalog - invalid page tree - missing IO handle.",
		 function );

		return( -1 );
	}
	if( table_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_initialize(
	     &catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create catalog definition.",
		 function );

		goto on_error;
	}
	if( libesedb_catalog_definition_read_data(
	     catalog_definition,
	     data,
	     data_size,
	     catalog->page_tree->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read catalog definition.",
		 function );

		goto on_error;
	}
	if( ( catalog_definition->type != LIBESEDB_CATALOG_DEFINITION_TYPE_TABLE )
	 && ( *table_definition == NULL ) )
	{
/* TODO add build-in table 1 support */
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: missing table definition for catalog definition type: %" PRIu16 ".\n",
			 function,
			 catalog_definition->type );
		}
#endif
		if( libesedb_catalog_definition_free(
		     &catalog_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free catalog definition.",
			 function );

			goto on_error;
		}
		catalog_definition = NULL;
	}
	else switch( catalog_definition->type )
	{
		case LIBESEDB_CATALOG_DEFINITION_TYPE_TABLE:
			*table_definition = NULL;

			if( libesedb_table_definition_initialize(
			     table_definition,
			     catalog_definition,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create table definition.",
				 function );

				libesedb_table_definition_free(
				 table_definition,
				 NULL );

				goto on_error;
			}
			catalog_definition = NULL;

			if( libcdata_list_append_value(
			     catalog->table_definition_list,
			     (intptr_t *) *table_definition,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append table definition to table definition list.",
				 function );

				libesedb_table_definition_free(
				 table_definition,
				 NULL );

				goto on_error;
			}
			break;

		case LIBESEDB_CATALOG_DEFINITION_TYPE_COLUMN:
			if( libesedb_table_definition_append_column_catalog_definition(
			     *table_definition,
			     catalog_definition,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append column catalog definition to table definition.",
				 function );

				goto on_error;
			}
			catalog_definition = NULL;

			break;

		case LIBESEDB_CATALOG_DEFINITION_TYPE_INDEX:
			if( libesedb_table_definition_append_index_catalog_definition(
			     *table_definition,
			     catalog_definition,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append index catalog definition to table definition.",
				 function );

				goto on_error;
			}
			catalog_definition = NULL;

			break;

		case LIBESEDB_CATALOG_DEFINITION_TYPE_LONG_VALUE:
			if( libesedb_table_definition_set_long_value_catalog_definition(
			     *table_definition,
			     catalog_definition,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set long value catalog definition in table definition.",
				 function );

				goto on_error;
			}
			catalog_definition = NULL;

			break;

		case LIBESEDB_CATALOG_DEFINITION_TYPE_CALLBACK:
			if( libesedb_table_definition_set_callback_catalog_definition(
			     *table_definition,
			     catalog_definition,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set callback catalog definition in table definition.",
				 function );

				goto on_error;
			}
			catalog_definition = NULL;

			break;

		default:
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unsupported catalog definition type: %" PRIu16 ".\n",
				 function,
				 catalog_definition->type );
			}
#endif
			if( libesedb_catalog_definition_free(
			     &catalog_definition,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free catalog definition.",
				 function );

				goto on_error;
			}
			catalog_definition = NULL;

			break;
	}
	return( 1 );

on_error:
	if( catalog_definition != NULL )
	{
		libesedb_catalog_definition_free(
		 &catalog_definition,
		 NULL );
	}
	return( -1 );
}

/* Reads the catalog values from a page
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_read_values_from_page(
     libesedb_catalog_t *catalog,
     libbfio_handle_t *file_io_handle,
     libesedb_page_t *page,
     libesedb_table_definition_t **table_definition,
     int recursion_depth,
     libcerror_error_t **error )
{
	libesedb_page_t *child_page                 = NULL;
	libesedb_page_tree_value_t *page_tree_value = NULL;
	libesedb_page_value_t *page_value           = NULL;
	libfcache_cache_t *child_page_cache         = NULL;
	static char *function                       = "libesedb_catalog_read_values_from_page";
	uint32_t child_page_number                  = 0;
	uint32_t page_flags                         = 0;
	uint16_t number_of_page_values              = 0;
	uint16_t page_value_index                   = 0;

	if( catalog == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog.",
		 function );

		return( -1 );
	}
	if( catalog->page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid catalog - missing page tree.",
		 function );

		return( -1 );
	}
	if( catalog->page_tree->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid catalog - invalid page tree - missing IO handle.",
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
			if( libesedb_catalog_read_value_data(
			     catalog,
			     page_tree_value->data,
			     page_tree_value->data_size,
			     table_definition,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read catalog value.",
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
				else if( child_page_number > catalog->page_tree->io_handle->last_page_number )
				{
					libcnotify_printf(
					 " (exceeds last page number: %" PRIu32 ")\n",
					 catalog->page_tree->io_handle->last_page_number );
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
			 && ( child_page_number <= catalog->page_tree->io_handle->last_page_number ) )
			{
				if( libfdata_vector_get_element_value_by_index(
				     catalog->page_tree->pages_vector,
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
				if( libesedb_catalog_read_values_from_page(
				     catalog,
				     file_io_handle,
				     child_page,
				     table_definition,
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
	return( 1 );

on_error:
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

/* Reads the catalog
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_read_file_io_handle(
     libesedb_catalog_t *catalog,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libesedb_page_t *root_page                    = NULL;
	libesedb_table_definition_t *table_definition = NULL;
	libfcache_cache_t *root_page_cache            = NULL;
	static char *function                         = "libesedb_catalog_read_file_io_handle";

	if( catalog == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog.",
		 function );

		return( -1 );
	}
	if( catalog->page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid catalog - missing page tree.",
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
	     catalog->page_tree->pages_vector,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) root_page_cache,
	     (int) catalog->page_tree->root_page_number - 1,
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
		 catalog->page_tree->root_page_number );

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
	     catalog->page_tree,
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
	if( libesedb_catalog_read_values_from_page(
	     catalog,
	     file_io_handle,
	     root_page,
	     &table_definition,
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

/* Retrieves the number of table definitions
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_get_number_of_table_definitions(
     libesedb_catalog_t *catalog,
     int *number_of_table_definitions,
     libcerror_error_t **error )
{
	static char *function = "libesedb_catalog_get_number_of_table_definitions";

	if( catalog == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog.",
		 function );

		return( -1 );
	}
	if( libcdata_list_get_number_of_elements(
	     catalog->table_definition_list,
	     number_of_table_definitions,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of table definitions.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the table definition for the specific index
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_get_table_definition_by_index(
     libesedb_catalog_t *catalog,
     int table_definition_index,
     libesedb_table_definition_t **table_definition,
     libcerror_error_t **error )
{
	static char *function = "libesedb_catalog_get_table_definition_by_index";

	if( catalog == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog.",
		 function );

		return( -1 );
	}
	if( libcdata_list_get_value_by_index(
	     catalog->table_definition_list,
	     table_definition_index,
	     (intptr_t **) table_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table definition: %d.",
		 function,
		 table_definition_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the table definition for the specific name
 * Returns 1 if successful, 0 if no corresponding table definition was found or -1 on error
 */
int libesedb_catalog_get_table_definition_by_name(
     libesedb_catalog_t *catalog,
     const uint8_t *table_name,
     size_t table_name_size,
     libesedb_table_definition_t **table_definition,
     libcerror_error_t **error )
{
	libcdata_list_element_t *list_element = NULL;
	static char *function                 = "libesedb_catalog_get_table_definition_by_name";
	int element_index                     = 0;
	int number_of_elements                = 0;

	if( catalog == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog.",
		 function );

		return( -1 );
	}
	if( catalog->table_definition_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid catalog - missing table definition list.",
		 function );

		return( -1 );
	}
	if( table_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table name.",
		 function );

		return( -1 );
	}
	if( table_name_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid table name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( table_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table definition.",
		 function );

		return( -1 );
	}
	if( libcdata_list_get_first_element(
	     catalog->table_definition_list,
	     &list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve first element of table definition list.",
		 function );

		return( -1 );
	}
	if( libcdata_list_get_number_of_elements(
	     catalog->table_definition_list,
	     &number_of_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of elements in table definition list.",
		 function );

		return( -1 );
	}
	for( element_index = 0;
	     element_index < number_of_elements;
	     element_index++ )
	{
		if( libcdata_list_element_get_value(
		     list_element,
		     (intptr_t **) table_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
		if( *table_definition == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing table definition: %d.",
			 function,
			 element_index );

			return( -1 );
		}
		if( ( *table_definition )->table_catalog_definition == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid table definition: %d - missing table catalog definition.",
			 function,
			 element_index );

			return( -1 );
		}
		if( ( *table_definition )->table_catalog_definition->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid table catalog definition: %d - missing name.",
			 function,
			 element_index );

			return( -1 );
		}
		if( table_name_size == ( *table_definition )->table_catalog_definition->name_size )
		{
			if( memory_compare(
			     ( *table_definition )->table_catalog_definition->name,
			     table_name,
			     table_name_size ) == 0 )
			{
				return( 1 );
			}
		}
		if( libcdata_list_element_get_next_element(
		     list_element,
		     &list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next element of element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
	}
	*table_definition = NULL;

	return( 0 );
}

/* Retrieves the table definition for the specific UTF-8 encoded name
 * Returns 1 if successful, 0 if no corresponding table definition was found or -1 on error
 */
int libesedb_catalog_get_table_definition_by_utf8_name(
     libesedb_catalog_t *catalog,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libesedb_table_definition_t **table_definition,
     libcerror_error_t **error )
{
	libcdata_list_element_t *list_element = NULL;
	static char *function                 = "libesedb_catalog_get_table_definition_by_utf8_name";
	int element_index                     = 0;
	int number_of_elements                = 0;
	int result                            = 0;

	if( catalog == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog.",
		 function );

		return( -1 );
	}
	if( catalog->table_definition_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid catalog - missing table definition list.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( table_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table definition.",
		 function );

		return( -1 );
	}
	if( libcdata_list_get_first_element(
	     catalog->table_definition_list,
	     &list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve first element of table definition list.",
		 function );

		return( -1 );
	}
	if( libcdata_list_get_number_of_elements(
	     catalog->table_definition_list,
	     &number_of_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of elements in table definition list.",
		 function );

		return( -1 );
	}
	for( element_index = 0;
	     element_index < number_of_elements;
	     element_index++ )
	{
		if( libcdata_list_element_get_value(
		     list_element,
		     (intptr_t **) table_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
		if( *table_definition == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing table definition: %d.",
			 function,
			 element_index );

			return( -1 );
		}
		if( ( *table_definition )->table_catalog_definition == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid table definition: %d - missing table catalog definition.",
			 function,
			 element_index );

			return( -1 );
		}
		if( ( *table_definition )->table_catalog_definition->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid table catalog definition: %d - missing name.",
			 function,
			 element_index );

			return( -1 );
		}
		result = libuna_utf8_string_compare_with_byte_stream(
			  utf8_string,
			  utf8_string_length,
			  ( *table_definition )->table_catalog_definition->name,
			  ( *table_definition )->table_catalog_definition->name_size,
			  LIBUNA_CODEPAGE_WINDOWS_1252,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: unable to compare UTF-8 string with table catalog definition: %d name.",
			 function,
			 element_index );

			return( -1 );
		}
		else if( result == LIBUNA_COMPARE_EQUAL )
		{
			return( 1 );
		}
		if( libcdata_list_element_get_next_element(
		     list_element,
		     &list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next element of element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
	}
	*table_definition = NULL;

	return( 0 );
}

/* Retrieves the table definition for the specific UTF-16 encoded name
 * Returns 1 if successful, 0 if no corresponding table definition was found or -1 on error
 */
int libesedb_catalog_get_table_definition_by_utf16_name(
     libesedb_catalog_t *catalog,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libesedb_table_definition_t **table_definition,
     libcerror_error_t **error )
{
	libcdata_list_element_t *list_element = NULL;
	static char *function                 = "libesedb_catalog_get_table_definition_by_utf16_name";
	int element_index                     = 0;
	int number_of_elements                = 0;
	int result                            = 0;

	if( catalog == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog.",
		 function );

		return( -1 );
	}
	if( catalog->table_definition_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid catalog - missing table definition list.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( table_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table definition.",
		 function );

		return( -1 );
	}
	if( libcdata_list_get_first_element(
	     catalog->table_definition_list,
	     &list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve first element of table definition list.",
		 function );

		return( -1 );
	}
	if( libcdata_list_get_number_of_elements(
	     catalog->table_definition_list,
	     &number_of_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of elements in table definition list.",
		 function );

		return( -1 );
	}
	for( element_index = 0;
	     element_index < number_of_elements;
	     element_index++ )
	{
		if( libcdata_list_element_get_value(
		     list_element,
		     (intptr_t **) table_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value from element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
		if( *table_definition == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing table definition: %d.",
			 function,
			 element_index );

			return( -1 );
		}
		if( ( *table_definition )->table_catalog_definition == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid table definition: %d - missing table catalog definition.",
			 function,
			 element_index );

			return( -1 );
		}
		if( ( *table_definition )->table_catalog_definition->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid table catalog definition: %d - missing name.",
			 function,
			 element_index );

			return( -1 );
		}
		result = libuna_utf16_string_compare_with_byte_stream(
			  utf16_string,
			  utf16_string_length,
			  ( *table_definition )->table_catalog_definition->name,
			  ( *table_definition )->table_catalog_definition->name_size,
			  LIBUNA_CODEPAGE_WINDOWS_1252,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: unable to compare UTF-16 string with table catalog definition: %d name.",
			 function,
			 element_index );

			return( -1 );
		}
		else if( result == LIBUNA_COMPARE_EQUAL )
		{
			return( 1 );
		}
		if( libcdata_list_element_get_next_element(
		     list_element,
		     &list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next element of element: %d.",
			 function,
			 element_index );

			return( -1 );
		}
	}
	*table_definition = NULL;

	return( 0 );
}

