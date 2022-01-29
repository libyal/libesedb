/*
 * Table functions
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
#include <memory.h>
#include <types.h>

#include "libesedb_column.h"
#include "libesedb_data_definition.h"
#include "libesedb_definitions.h"
#include "libesedb_index.h"
#include "libesedb_io_handle.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcdata.h"
#include "libesedb_libcerror.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_page_tree.h"
#include "libesedb_record.h"
#include "libesedb_table.h"
#include "libesedb_table_definition.h"
#include "libesedb_types.h"

/* Creates a table
 * Make sure the value table is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_initialize(
     libesedb_table_t **table,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libesedb_table_definition_t *table_definition,
     libesedb_table_definition_t *template_table_definition,
     libcerror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_initialize";
	int segment_index                         = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( *table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table value already set.",
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
	if( table_definition->table_catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table definition - missing table catalog definition.",
		 function );

		return( -1 );
	}
	internal_table = memory_allocate_structure(
	                  libesedb_internal_table_t );

	if( internal_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_table,
	     0,
	     sizeof( libesedb_internal_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear table.",
		 function );

		memory_free(
		 internal_table );

		return( -1 );
	}
	/* TODO add clone function ?
	 */
	if( libfdata_vector_initialize(
	     &( internal_table->pages_vector ),
	     (size64_t) io_handle->page_size,
	     (intptr_t *) io_handle,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfdata_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libesedb_io_handle_read_page,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create pages vector.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_append_segment(
	     internal_table->pages_vector,
	     &segment_index,
	     0,
	     io_handle->pages_data_offset,
	     io_handle->pages_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append segment to pages vector.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( internal_table->pages_cache ),
	     LIBESEDB_MAXIMUM_CACHE_ENTRIES_PAGES,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create pages cache.",
		 function );

		goto on_error;
	}
	if( libesedb_page_tree_initialize(
	     &( internal_table->table_page_tree ),
	     io_handle,
	     internal_table->pages_vector,
	     internal_table->pages_cache,
	     table_definition->table_catalog_definition->identifier,
	     table_definition->table_catalog_definition->father_data_page_number,
	     table_definition,
	     template_table_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create table page tree.",
		 function );

		goto on_error;
	}
	if( table_definition->long_value_catalog_definition != NULL )
	{
		/* TODO add clone function ?
		 */
		if( libfdata_vector_initialize(
		     &( internal_table->long_values_pages_vector ),
		     (size64_t) io_handle->page_size,
		     (intptr_t *) io_handle,
		     NULL,
		     NULL,
		     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfdata_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libesedb_io_handle_read_page,
		     NULL,
		     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create long values pages vector.",
			 function );

			goto on_error;
		}
		if( libfdata_vector_append_segment(
		     internal_table->long_values_pages_vector,
		     &segment_index,
		     0,
		     io_handle->pages_data_offset,
		     io_handle->pages_data_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append segment to long values pages vector.",
			 function );

			goto on_error;
		}
		if( libfcache_cache_initialize(
		     &( internal_table->long_values_pages_cache ),
		     LIBESEDB_MAXIMUM_CACHE_ENTRIES_PAGES,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create long values pages cache.",
			 function );

			goto on_error;
		}
		if( libesedb_page_tree_initialize(
		     &( internal_table->long_values_page_tree ),
		     io_handle,
		     internal_table->long_values_pages_vector,
		     internal_table->long_values_pages_cache,
		     table_definition->long_value_catalog_definition->identifier,
		     table_definition->long_value_catalog_definition->father_data_page_number,
		     table_definition,
		     template_table_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create long value page tree.",
			 function );

			goto on_error;
		}
	}
	internal_table->io_handle                 = io_handle;
	internal_table->file_io_handle            = file_io_handle;
	internal_table->table_definition          = table_definition;
	internal_table->template_table_definition = template_table_definition;

	*table = (libesedb_table_t *) internal_table;

	return( 1 );

on_error:
	if( internal_table != NULL )
	{
		if( internal_table->long_values_pages_cache != NULL )
		{
			libfcache_cache_free(
			 &( internal_table->long_values_pages_cache ),
			 NULL );
		}
		if( internal_table->long_values_pages_vector != NULL )
		{
			libfdata_vector_free(
			 &( internal_table->long_values_pages_vector ),
			 NULL );
		}
		if( internal_table->long_values_page_tree != NULL )
		{
			libesedb_page_tree_free(
			 &( internal_table->long_values_page_tree ),
			 NULL );
		}
		if( internal_table->table_page_tree != NULL )
		{
			libesedb_page_tree_free(
			 &( internal_table->table_page_tree ),
			 NULL );
		}
		if( internal_table->pages_cache != NULL )
		{
			libfcache_cache_free(
			 &( internal_table->pages_cache ),
			 NULL );
		}
		if( internal_table->pages_vector != NULL )
		{
			libfdata_vector_free(
			 &( internal_table->pages_vector ),
			 NULL );
		}
		memory_free(
		 internal_table );
	}
	return( -1 );
}

/* Frees a table
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_free(
     libesedb_table_t **table,
     libcerror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_free";
	int result                                = 1;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( *table != NULL )
	{
		internal_table = (libesedb_internal_table_t *) *table;
		*table         = NULL;

		/* The io_handle, file_io_handle and table_definition references
		 * are freed elsewhere
		 */
		if( libfdata_vector_free(
		     &( internal_table->pages_vector ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free pages vector.",
			 function );

			result = -1;
		}
		if( libfcache_cache_free(
		     &( internal_table->pages_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free pages cache.",
			 function );

			result = -1;
		}
		if( internal_table->long_values_pages_vector != NULL )
		{
			if( libfdata_vector_free(
			     &( internal_table->long_values_pages_vector ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free long values pages vector.",
				 function );

				result = -1;
			}
		}
		if( internal_table->long_values_pages_cache != NULL )
		{
			if( libfcache_cache_free(
			     &( internal_table->long_values_pages_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free long values pages cache.",
				 function );

				result = -1;
			}
		}
		if( internal_table->long_values_page_tree != NULL )
		{
			if( libesedb_page_tree_free(
			     &( internal_table->long_values_page_tree ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free long values page tree.",
				 function );

				result = -1;
			}
		}
		if( libesedb_page_tree_free(
		     &( internal_table->table_page_tree ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free table page tree.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_table );
	}
	return( result );
}

/* Retrieves the table identifier or Father Data Page (FDP) object identifier
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_identifier(
     libesedb_table_t *table,
     uint32_t *identifier,
     libcerror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_identifier";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( internal_table->table_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing table definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_identifier(
	     internal_table->table_definition->table_catalog_definition,
	     identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded string of the table name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_utf8_name_size(
     libesedb_table_t *table,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_utf8_name_size";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( internal_table->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_table->table_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing table definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_utf8_name_size(
	     internal_table->table_definition->table_catalog_definition,
	     utf8_string_size,
	     internal_table->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded string of the table name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_utf8_name(
     libesedb_table_t *table,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_utf8_name";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( internal_table->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_table->table_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing table definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_utf8_name(
	     internal_table->table_definition->table_catalog_definition,
	     utf8_string,
	     utf8_string_size,
	     internal_table->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded string of the table name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_utf16_name_size(
     libesedb_table_t *table,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_utf16_name_size";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( internal_table->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_table->table_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing table definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_utf16_name_size(
	     internal_table->table_definition->table_catalog_definition,
	     utf16_string_size,
	     internal_table->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded string of the table name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_utf16_name(
     libesedb_table_t *table,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_utf16_name";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( internal_table->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_table->table_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing table definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_utf16_name(
	     internal_table->table_definition->table_catalog_definition,
	     utf16_string,
	     utf16_string_size,
	     internal_table->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded string of the template name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_utf8_template_name_size(
     libesedb_table_t *table,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_utf8_template_name_size";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( internal_table->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_table->table_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing table definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_utf8_template_name_size(
	     internal_table->table_definition->table_catalog_definition,
	     utf8_string_size,
	     internal_table->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded string of the template name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_utf8_template_name(
     libesedb_table_t *table,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_utf8_template_name";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( internal_table->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_table->table_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing table definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_utf8_template_name(
	     internal_table->table_definition->table_catalog_definition,
	     utf8_string,
	     utf8_string_size,
	     internal_table->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded string of the template name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_utf16_template_name_size(
     libesedb_table_t *table,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_utf16_template_name_size";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( internal_table->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_table->table_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing table definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_utf16_template_name_size(
	     internal_table->table_definition->table_catalog_definition,
	     utf16_string_size,
	     internal_table->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded string of the template name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_utf16_template_name(
     libesedb_table_t *table,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_utf16_template_name";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( internal_table->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_table->table_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing table definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_utf16_template_name(
	     internal_table->table_definition->table_catalog_definition,
	     utf16_string,
	     utf16_string_size,
	     internal_table->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of columns in the table
 *
 * Use the flag LIBESEDB_GET_COLUMN_FLAG_IGNORE_TEMPLATE_TABLE to retrieve the number of columns
 * ignoring the template table
 *
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_number_of_columns(
     libesedb_table_t *table,
     int *number_of_columns,
     uint8_t flags,
     libcerror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_number_of_columns";
	int template_table_number_of_columns      = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( number_of_columns == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of columns.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBESEDB_GET_COLUMN_FLAG_IGNORE_TEMPLATE_TABLE ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.",
		 function );

		return( -1 );
	}
	if( ( ( flags & LIBESEDB_GET_COLUMN_FLAG_IGNORE_TEMPLATE_TABLE ) == 0 )
	 && ( internal_table->template_table_definition != NULL ) )
	{
		if( libesedb_table_definition_get_number_of_column_catalog_definitions(
		     internal_table->template_table_definition,
		     &template_table_number_of_columns,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of columns from template table.",
			 function );

			return( -1 );
		}
	}
	if( libesedb_table_definition_get_number_of_column_catalog_definitions(
	     internal_table->table_definition,
	     number_of_columns,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of columns from table.",
		 function );

		return( -1 );
	}
	*number_of_columns += template_table_number_of_columns;

	return( 1 );
}

/* Retrieves a specific column
 *
 * Use the flag LIBESEDB_GET_COLUMN_FLAG_IGNORE_TEMPLATE_TABLE to retrieve the column
 * ignoring the template table
 *
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_column(
     libesedb_table_t *table,
     int column_entry,
     libesedb_column_t **column,
     uint8_t flags,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_table_t *internal_table                = NULL;
	static char *function                                    = "libesedb_table_get_column";
	int template_table_number_of_columns                     = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( column == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid column.",
		 function );

		return( -1 );
	}
	if( *column != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid column value already set.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBESEDB_GET_COLUMN_FLAG_IGNORE_TEMPLATE_TABLE ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.",
		 function );

		return( -1 );
	}
	if( ( flags & LIBESEDB_GET_COLUMN_FLAG_IGNORE_TEMPLATE_TABLE ) == 0 )
	{
		if( internal_table->template_table_definition != NULL )
		{
			if( libesedb_table_definition_get_number_of_column_catalog_definitions(
			     internal_table->template_table_definition,
			     &template_table_number_of_columns,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve number of columns from template table.",
				 function );

				return( -1 );
			}
		}
	}
	if( column_entry < template_table_number_of_columns )
	{
		if( libesedb_table_definition_get_column_catalog_definition_by_index(
		     internal_table->template_table_definition,
		     column_entry,
		     &column_catalog_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column catalog definition from template table.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( libesedb_table_definition_get_column_catalog_definition_by_index(
		     internal_table->table_definition,
		     column_entry - template_table_number_of_columns,
		     &column_catalog_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column catalog definition from table.",
			 function );

			return( -1 );
		}
	}
	if( column_catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_column_initialize(
	     column,
	     internal_table->io_handle,
	     column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create column.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of indexes
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_number_of_indexes(
     libesedb_table_t *table,
     int *number_of_indexes,
     libcerror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_number_of_indexes";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( libesedb_table_definition_get_number_of_index_catalog_definitions(
	     internal_table->table_definition,
	     number_of_indexes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of indexes.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific index
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_index(
     libesedb_table_t *table,
     int index_entry,
     libesedb_index_t **index,
     libcerror_error_t **error )
{
	libesedb_internal_table_t *internal_table               = NULL;
	libesedb_catalog_definition_t *index_catalog_definition = NULL;
	static char *function                                   = "libesedb_table_get_index";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	if( *index != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index value already set.",
		 function );

		return( -1 );
	}
	if( libesedb_table_definition_get_index_catalog_definition_by_index(
	     internal_table->table_definition,
	     index_entry,
	     &index_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve index catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_index_initialize(
	     index,
	     internal_table->file_io_handle,
	     internal_table->io_handle,
	     internal_table->table_definition,
	     internal_table->template_table_definition,
	     index_catalog_definition,
	     internal_table->pages_vector,
	     internal_table->pages_cache,
	     internal_table->long_values_pages_vector,
	     internal_table->long_values_pages_cache,
	     internal_table->table_page_tree,
	     internal_table->long_values_page_tree,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of records in the table
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_number_of_records(
     libesedb_table_t *table,
     int *number_of_records,
     libcerror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_number_of_records";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( libesedb_page_tree_get_number_of_leaf_values(
	     internal_table->table_page_tree,
	     internal_table->file_io_handle,
	     number_of_records,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of leaf values from table page tree.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific record
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_record(
     libesedb_table_t *table,
     int record_entry,
     libesedb_record_t **record,
     libcerror_error_t **error )
{
	libesedb_data_definition_t *record_data_definition = NULL;
	libesedb_internal_table_t *internal_table          = NULL;
	static char *function                              = "libesedb_table_get_record";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( *record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid record value already set.",
		 function );

		return( -1 );
	}
	if( libesedb_page_tree_get_leaf_value_by_index(
	     internal_table->table_page_tree,
	     internal_table->file_io_handle,
	     record_entry,
	     &record_data_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve leaf value: %d from table values tree.",
		 function,
		 record_entry );

		goto on_error;
	}
	if( libesedb_record_initialize(
	     record,
	     internal_table->file_io_handle,
	     internal_table->io_handle,
	     internal_table->table_definition,
	     internal_table->template_table_definition,
	     internal_table->pages_vector,
	     internal_table->pages_cache,
	     internal_table->long_values_pages_vector,
	     internal_table->long_values_pages_cache,
	     record_data_definition,
	     internal_table->long_values_page_tree,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create record: %d.",
		 function,
		 record_entry );

		goto on_error;
	}
	return( 1 );

on_error:
	if( record_data_definition != NULL )
	{
		libesedb_data_definition_free(
		 &record_data_definition,
		 NULL );
	}
	return( -1 );
}

