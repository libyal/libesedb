/*
 * Index functions
 *
 * Copyright (c) 2009-2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include "libesedb_catalog_definition.h"
#include "libesedb_definitions.h"
#include "libesedb_io_handle.h"
#include "libesedb_index.h"
#include "libesedb_page_tree.h"
#include "libesedb_types.h"

/* Creates a index
 * Returns 1 if successful or -1 on error
 */
int libesedb_index_initialize(
     libesedb_index_t **index,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libesedb_table_definition_t *table_definition,
     libesedb_catalog_definition_t *index_catalog_definition,
     uint8_t flags,
     liberror_error_t **error )
{
	libesedb_internal_index_t *internal_index = NULL;
	libesedb_page_tree_t *index_page_tree     = NULL;
	static char *function                     = "libesedb_index_initialize";
	off64_t node_data_offset                  = 0;

	if( index == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBESEDB_ITEM_FLAG_MANAGED_FILE_IO_HANDLE ) ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags: 0x%02" PRIx8 ".",
		 function,
		 flags );

		return( -1 );
	}
	if( *index == NULL )
	{
		internal_index = (libesedb_internal_index_t *) memory_allocate(
		                                                sizeof( libesedb_internal_index_t ) );

		if( internal_index == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create index.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     internal_index,
		     0,
		     sizeof( libesedb_internal_index_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear index.",
			 function );

			memory_free(
			 internal_index );

			return( -1 );
		}
		if( ( flags & LIBESEDB_ITEM_FLAG_MANAGED_FILE_IO_HANDLE ) == 0 )
		{
			internal_index->file_io_handle = file_io_handle;
		}
		else
		{
			if( libbfio_handle_clone(
			     &( internal_index->file_io_handle ),
			     file_io_handle,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy file io handle.",
				 function );

				memory_free(
				 internal_index );

				return( -1 );
			}
			if( libbfio_handle_set_open_on_demand(
			     internal_index->file_io_handle,
			     1,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to set open on demand in file io handle.",
				 function );

				libbfio_handle_free(
				 &( internal_index->file_io_handle ),
				 NULL );
				memory_free(
				 internal_index );

				return( -1 );
			}
		}
		/* TODO clone function ? */
		if( libfdata_vector_initialize(
		     &( internal_index->pages_vector ),
		     (size64_t) io_handle->page_size,
		     io_handle->pages_data_offset,
		     io_handle->pages_data_size,
		     (intptr_t *) io_handle,
		     NULL,
		     NULL,
		     &libesedb_io_handle_read_page,
		     LIBFDATA_FLAG_IO_HANDLE_NON_MANAGED,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create pages vector.",
			 function );

			if( ( flags & LIBESEDB_ITEM_FLAG_MANAGED_FILE_IO_HANDLE ) != 0 )
			{
				libbfio_handle_free(
				 &( internal_index->file_io_handle ),
				 NULL );
			}
			memory_free(
			 internal_index );

			return( -1 );
		}
		if( libfdata_cache_initialize(
		     &( internal_index->pages_cache ),
		     LIBESEDB_MAXIMUM_CACHE_ENTRIES_PAGES,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create pages cache.",
			 function );

			libfdata_vector_free(
			 &( internal_index->pages_vector ),
			 NULL );

			if( ( flags & LIBESEDB_ITEM_FLAG_MANAGED_FILE_IO_HANDLE ) != 0 )
			{
				libbfio_handle_free(
				 &( internal_index->file_io_handle ),
				 NULL );
			}
			memory_free(
			 internal_index );

			return( -1 );
		}
		/* TODO (template) table definition required ? */

		if( libesedb_page_tree_initialize(
		     &index_page_tree,
		     io_handle,
		     internal_index->pages_vector,
		     internal_index->pages_cache,
		     index_catalog_definition->identifier,
		     NULL,
		     NULL,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create index page tree.",
			 function );

			libfdata_cache_free(
			 &( internal_index->pages_cache ),
			 NULL );
			libfdata_vector_free(
			 &( internal_index->pages_vector ),
			 NULL );

			if( ( flags & LIBESEDB_ITEM_FLAG_MANAGED_FILE_IO_HANDLE ) != 0 )
			{
				libbfio_handle_free(
				 &( internal_index->file_io_handle ),
				 NULL );
			}
			memory_free(
			 internal_index );

			return( -1 );
		}
		/* TODO clone function
		 */
		if( libfdata_tree_initialize(
		     &( internal_index->index_values_tree ),
		     (intptr_t *) index_page_tree,
		     &libesedb_page_tree_free,
		     NULL,
		     &libesedb_page_tree_read_node_value,
		     &libesedb_page_tree_read_sub_nodes,
		     LIBFDATA_FLAG_IO_HANDLE_MANAGED,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create index values tree.",
			 function );

			libesedb_page_tree_free(
			 (intptr_t *) index_page_tree,
			 NULL );
			libfdata_cache_free(
			 &( internal_index->pages_cache ),
			 NULL );
			libfdata_vector_free(
			 &( internal_index->pages_vector ),
			 NULL );

			if( ( flags & LIBESEDB_ITEM_FLAG_MANAGED_FILE_IO_HANDLE ) != 0 )
			{
				libbfio_handle_free(
				 &( internal_index->file_io_handle ),
				 NULL );
			}
			memory_free(
			 internal_index );

			return( -1 );
		}
		if( libfdata_cache_initialize(
		     &( internal_index->index_values_cache ),
		     LIBESEDB_MAXIMUM_CACHE_ENTRIES_INDEX_VALUES,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create index values cache.",
			 function );

			libfdata_tree_free(
			 &( internal_index->index_values_tree ),
			 NULL );
			libfdata_cache_free(
			 &( internal_index->pages_cache ),
			 NULL );
			libfdata_vector_free(
			 &( internal_index->pages_vector ),
			 NULL );

			if( ( flags & LIBESEDB_ITEM_FLAG_MANAGED_FILE_IO_HANDLE ) != 0 )
			{
				libbfio_handle_free(
				 &( internal_index->file_io_handle ),
				 NULL );
			}
			memory_free(
			 internal_index );

			return( -1 );
		}
		node_data_offset  = index_catalog_definition->father_data_page_number - 1;
		node_data_offset *= io_handle->page_size;

		if( libfdata_tree_set_root_node(
		     internal_index->index_values_tree,
		     node_data_offset,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set root node in index values tree.",
			 function );

			libfdata_cache_free(
			 &( internal_index->index_values_cache ),
			 NULL );
			libfdata_tree_free(
			 &( internal_index->index_values_tree ),
			 NULL );
			libfdata_cache_free(
			 &( internal_index->pages_cache ),
			 NULL );
			libfdata_vector_free(
			 &( internal_index->pages_vector ),
			 NULL );

			if( ( flags & LIBESEDB_ITEM_FLAG_MANAGED_FILE_IO_HANDLE ) != 0 )
			{
				libbfio_handle_free(
				 &( internal_index->file_io_handle ),
				 NULL );
			}
			memory_free(
			 internal_index );

			return( -1 );
		}
		internal_index->io_handle                = io_handle;
		internal_index->table_definition         = table_definition;
		internal_index->index_catalog_definition = index_catalog_definition;
		internal_index->flags                    = flags;

		*index = (libesedb_index_t *) internal_index;
	}
	return( 1 );
}

/* Frees index
 * Returns 1 if successful or -1 on error
 */
int libesedb_index_free(
     libesedb_index_t **index,
     liberror_error_t **error )
{
	libesedb_internal_index_t *internal_index = NULL;
	static char *function                     = "libesedb_index_free";
	int result                                = 1;

	if( index == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	if( *index != NULL )
	{
		internal_index = (libesedb_internal_index_t *) *index;
		*index         = NULL;

		/* The io_handle, table_definition and index_catalog_definition references
		 * are freed elsewhere
		 */
		if( ( internal_index->flags & LIBESEDB_ITEM_FLAG_MANAGED_FILE_IO_HANDLE ) != 0 )
		{
			if( internal_index->file_io_handle != NULL )
			{
				if( libbfio_handle_close(
				     internal_index->file_io_handle,
				     error ) != 0 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_CLOSE_FAILED,
					 "%s: unable to close file io handle.",
					 function );

					result = -1;
				}
				if( libbfio_handle_free(
				     &( internal_index->file_io_handle ),
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free file io handle.",
					 function );

					result = -1;
				}
			}
		}
		if( libfdata_vector_free(
		     &( internal_index->pages_vector ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free pages vector.",
			 function );

			result = -1;
		}
		if( libfdata_cache_free(
		     &( internal_index->pages_cache ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free pages cache.",
			 function );

			result = -1;
		}
		if( libfdata_tree_free(
		     &( internal_index->index_values_tree ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free index values tree.",
			 function );

			result = -1;
		}
		if( libfdata_cache_free(
		     &( internal_index->index_values_cache ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free index values cache.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_index );
	}
	return( result );
}

/* Retrieves the index identifier or Father Data Page (FDP) object identifier
 * Returns 1 if successful or -1 on error
 */
int libesedb_index_get_identifier(
     libesedb_index_t *index,
     uint32_t *identifier,
     liberror_error_t **error )
{
	libesedb_internal_index_t *internal_index = NULL;
	static char *function                     = "libesedb_index_get_identifier";

	if( index == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	internal_index = (libesedb_internal_index_t *) index;

	if( libesedb_catalog_definition_get_identifier(
	     internal_index->index_catalog_definition,
	     identifier,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string size of the index name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_index_get_utf8_name_size(
     libesedb_index_t *index,
     size_t *utf8_string_size,
     liberror_error_t **error )
{
	libesedb_internal_index_t *internal_index = NULL;
	static char *function                     = "libesedb_index_get_utf8_string_size";

	if( index == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	internal_index = (libesedb_internal_index_t *) index;

	if( internal_index->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal index - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_utf8_name_size(
	     internal_index->index_catalog_definition,
	     utf8_string_size,
	     internal_index->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string of the index name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_index_get_utf8_name(
     libesedb_index_t *index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     liberror_error_t **error )
{
	libesedb_internal_index_t *internal_index = NULL;
	static char *function                     = "libesedb_index_get_utf8_string";

	if( index == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	internal_index = (libesedb_internal_index_t *) index;

	if( internal_index->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal index - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_utf8_name(
	     internal_index->index_catalog_definition,
	     utf8_string,
	     utf8_string_size,
	     internal_index->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string size of the index name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_index_get_utf16_name_size(
     libesedb_index_t *index,
     size_t *utf16_string_size,
     liberror_error_t **error )
{
	libesedb_internal_index_t *internal_index = NULL;
	static char *function                     = "libesedb_index_get_utf16_string_size";

	if( index == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	internal_index = (libesedb_internal_index_t *) index;

	if( internal_index->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal index - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_utf16_name_size(
	     internal_index->index_catalog_definition,
	     utf16_string_size,
	     internal_index->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string of the index name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_index_get_utf16_name(
     libesedb_index_t *index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     liberror_error_t **error )
{
	libesedb_internal_index_t *internal_index = NULL;
	static char *function                     = "libesedb_index_get_utf16_string";

	if( index == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	internal_index = (libesedb_internal_index_t *) index;

	if( internal_index->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal index - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_utf16_name(
	     internal_index->index_catalog_definition,
	     utf16_string,
	     utf16_string_size,
	     internal_index->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

