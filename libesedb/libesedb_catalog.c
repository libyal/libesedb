/*
 * Catalog functions
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

#include "libesedb_block_descriptor.h"
#include "libesedb_block_tree.h"
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
	if( libesedb_block_tree_initialize(
	     &( ( *catalog )->page_block_tree ),
	     io_handle->file_size,
	     io_handle->page_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create page block tree.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *catalog )->table_definition_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create table definition array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *catalog != NULL )
	{
		if( ( *catalog )->page_block_tree != NULL )
		{
			libesedb_block_tree_free(
			 &( ( *catalog )->page_block_tree ),
			 (int (*)(intptr_t **, libcerror_error_t **)) &libesedb_block_descriptor_free,
			 NULL );
		}
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
		if( libesedb_block_tree_free(
		     &( ( *catalog )->page_block_tree ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libesedb_block_descriptor_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free page block tree.",
			 function );

			result = -1;
		}
		if( libcdata_array_free(
		     &( ( *catalog )->table_definition_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libesedb_table_definition_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free table definition array.",
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
	int entry_index                                   = 0;

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

			if( libcdata_array_append_entry(
			     catalog->table_definition_array,
			     &entry_index,
			     (intptr_t *) *table_definition,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append table definition to table definition array.",
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

/* Reads the catalog values from a leaf page
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_read_values_from_leaf_page(
     libesedb_catalog_t *catalog,
     libesedb_page_t *page,
     libesedb_table_definition_t **table_definition,
     libcerror_error_t **error )
{
	libesedb_page_tree_value_t *page_tree_value = NULL;
	libesedb_page_value_t *page_value           = NULL;
	static char *function                       = "libesedb_catalog_read_values_from_leaf_page";
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
	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported page - not a leaf page.",
		 function );

		goto on_error;
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
	return( 1 );

on_error:
	if( page_tree_value != NULL )
	{
		libesedb_page_tree_value_free(
		 &page_tree_value,
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
	libesedb_page_t *page                         = NULL;
	libesedb_table_definition_t *table_definition = NULL;
	static char *function                         = "libesedb_catalog_read_file_io_handle";
	off64_t page_offset                           = 0;
	uint32_t leaf_page_number                     = 0;

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
	if( libesedb_page_tree_get_get_first_leaf_page_number(
	     catalog->page_tree,
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

		return( -1 );
	}
	while( leaf_page_number != 0 )
	{
		page_offset = ( leaf_page_number + 1 ) * catalog->page_tree->io_handle->page_size;

		if( libesedb_page_tree_check_if_page_block_first_read(
		     catalog->page_tree,
		     catalog->page_block_tree,
		     leaf_page_number,
		     page_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to check if first read of page number: %" PRIu32 ".",
			 function,
			 leaf_page_number );

			return( -1 );
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

			return( -1 );
		}
		if( libfdata_vector_get_element_value_by_index(
		     catalog->page_tree->pages_vector,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) catalog->page_tree->pages_cache,
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

			return( -1 );
		}
		if( libesedb_catalog_read_values_from_leaf_page(
		     catalog,
		     page,
		     &table_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to read values from page: %" PRIu32 ".",
			 function,
			 leaf_page_number );

			return( -1 );
		}
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

			return( -1 );
		}
	}
	return( 1 );
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
	if( libcdata_array_get_number_of_entries(
	     catalog->table_definition_array,
	     number_of_table_definitions,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from table definition array.",
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
	if( libcdata_array_get_entry_by_index(
	     catalog->table_definition_array,
	     table_definition_index,
	     (intptr_t **) table_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from table definition array.",
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
	libesedb_table_definition_t *safe_table_definition = NULL;
	static char *function                              = "libesedb_catalog_get_table_definition_by_name";
	int entry_index                                    = 0;
	int number_of_entries                              = 0;
	int result                                         = 0;

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
	if( ( table_name_size == 0 )
	 || ( table_name_size > (size_t) SSIZE_MAX ) )
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
	*table_definition = NULL;

	if( libcdata_array_get_number_of_entries(
	     catalog->table_definition_array,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries in table definition array.",
		 function );

		return( -1 );
	}
	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     catalog->table_definition_array,
		     entry_index,
		     (intptr_t **) &safe_table_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %d from table definition array.",
			 function,
			 entry_index );

			return( -1 );
		}
		if( safe_table_definition == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing table definition: %d.",
			 function,
			 entry_index );

			return( -1 );
		}
		result = libesedb_catalog_definition_compare_name(
			  safe_table_definition->table_catalog_definition,
			  table_name,
			  table_name_size,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: unable to compare table name with table catalog definition: %d name.",
			 function,
			 entry_index );

			return( -1 );
		}
		else if( result == 1 )
		{
			break;
		}
	}
	if( result != 0 )
	{
		*table_definition = safe_table_definition;
	}
	return( result );
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
	libesedb_table_definition_t *safe_table_definition = NULL;
	static char *function                              = "libesedb_catalog_get_table_definition_by_utf8_name";
	int entry_index                                    = 0;
	int number_of_entries                              = 0;
	int result                                         = 0;

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
	if( ( utf8_string_length == 0 )
	 || ( utf8_string_length > (size_t) SSIZE_MAX ) )
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
	*table_definition = NULL;

	if( libcdata_array_get_number_of_entries(
	     catalog->table_definition_array,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries in table definition array.",
		 function );

		return( -1 );
	}
	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     catalog->table_definition_array,
		     entry_index,
		     (intptr_t **) &safe_table_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %d from table definition array.",
			 function,
			 entry_index );

			return( -1 );
		}
		if( safe_table_definition == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing table definition: %d.",
			 function,
			 entry_index );

			return( -1 );
		}
		result = libesedb_catalog_definition_compare_name_with_utf8_string(
			  safe_table_definition->table_catalog_definition,
			  utf8_string,
			  utf8_string_length,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: unable to compare UTF-8 string with table catalog definition: %d name.",
			 function,
			 entry_index );

			return( -1 );
		}
		else if( result == LIBUNA_COMPARE_EQUAL )
		{
			result = 1;

			break;
		}
	}
	if( result != 0 )
	{
		*table_definition = safe_table_definition;
	}
	return( result );
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
	libesedb_table_definition_t *safe_table_definition = NULL;
	static char *function                              = "libesedb_catalog_get_table_definition_by_utf16_name";
	int entry_index                                    = 0;
	int number_of_entries                              = 0;
	int result                                         = 0;

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
	if( ( utf16_string_length == 0 )
	 || ( utf16_string_length > (size_t) SSIZE_MAX ) )
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
	*table_definition = NULL;

	if( libcdata_array_get_number_of_entries(
	     catalog->table_definition_array,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries in table definition array.",
		 function );

		return( -1 );
	}
	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     catalog->table_definition_array,
		     entry_index,
		     (intptr_t **) &safe_table_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %d from table definition array.",
			 function,
			 entry_index );

			return( -1 );
		}
		if( safe_table_definition == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing table definition: %d.",
			 function,
			 entry_index );

			return( -1 );
		}
		result = libesedb_catalog_definition_compare_name_with_utf16_string(
			  safe_table_definition->table_catalog_definition,
			  utf16_string,
			  utf16_string_length,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: unable to compare UTF-16 string with table catalog definition: %d name.",
			 function,
			 entry_index );

			return( -1 );
		}
		else if( result == LIBUNA_COMPARE_EQUAL )
		{
			result = 1;

			break;
		}
	}
	if( result != 0 )
	{
		*table_definition = safe_table_definition;
	}
	return( result );
}

