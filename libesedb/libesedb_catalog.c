/*
 * Catalog functions
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
#include <libnotify.h>

#include "libesedb_definitions.h"
#include "libesedb_catalog.h"
#include "libesedb_catalog_definition.h"
#include "libesedb_libuna.h"
#include "libesedb_page_tree.h"
#include "libesedb_table_definition.h"

/* Initialize a catalog
 * Make sure the value catalog is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_initialize(
     libesedb_catalog_t **catalog,
     liberror_error_t **error )
{
	static char *function = "libesedb_catalog_initialize";

	if( catalog == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog.",
		 function );

		return( -1 );
	}
	if( *catalog == NULL )
	{
		*catalog = (libesedb_catalog_t *) memory_allocate(
		                                   sizeof( libesedb_catalog_t ) );

		if( *catalog == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create catalog.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     ( *catalog ),
		     0,
		     sizeof( libesedb_catalog_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear catalog.",
			 function );

			memory_free(
			 *catalog );

			*catalog = NULL;

			return( -1 );
		}
		if( libesedb_list_initialize(
		     &( ( *catalog )->table_definition_list ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create table definition list.",
			 function );

			memory_free(
			 *catalog );

			*catalog = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees a catalog
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_free(
     libesedb_catalog_t **catalog,
     liberror_error_t **error )
{
	static char *function = "libesedb_catalog_free";
	int result            = 1;

	if( catalog == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog.",
		 function );

		return( -1 );
	}
	if( *catalog != NULL )
	{
		if( libesedb_list_free(
		     &( ( *catalog )->table_definition_list ),
		     &libesedb_table_definition_free,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
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

/* Retrieves the number of table definitions
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_get_number_of_table_definitions(
     libesedb_catalog_t *catalog,
     int *number_of_table_definitions,
     liberror_error_t **error )
{
	static char *function = "libesedb_catalog_get_number_of_table_definitions";

	if( catalog == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog.",
		 function );

		return( -1 );
	}
	if( libesedb_list_get_number_of_elements(
	     catalog->table_definition_list,
	     number_of_table_definitions,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
     liberror_error_t **error )
{
	static char *function = "libesedb_catalog_get_table_definition_by_index";

	if( catalog == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog.",
		 function );

		return( -1 );
	}
	if( libesedb_list_get_value_by_index(
	     catalog->table_definition_list,
	     table_definition_index,
	     (intptr_t **) table_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
     liberror_error_t **error )
{
	libesedb_list_element_t *list_element = NULL;
	static char *function                 = "libesedb_catalog_get_table_definition_by_name";
	int list_element_iterator             = 0;

	if( catalog == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog.",
		 function );

		return( -1 );
	}
	if( catalog->table_definition_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid catalog - missing table definition list.",
		 function );

		return( -1 );
	}
	if( table_name == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table name.",
		 function );

		return( -1 );
	}
	if( table_name_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid table name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( table_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table definition.",
		 function );

		return( -1 );
	}
	list_element = catalog->table_definition_list->first_element;

	for( list_element_iterator = 0;
	     list_element_iterator < catalog->table_definition_list->number_of_elements;
	     list_element_iterator++ )
	{
		if( list_element == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected for element: %d.",
			 function,
			 list_element_iterator );

			return( -1 );
		}
		if( list_element->value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid list element: %d - missing value.",
			 function,
			 list_element_iterator );

			return( -1 );
		}
		*table_definition = (libesedb_table_definition_t *) list_element->value;

		if( ( *table_definition )->table_catalog_definition == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid table definition: %d - missing table catalog definition.",
			 function,
			 list_element_iterator );

			return( -1 );
		}
		if( ( *table_definition )->table_catalog_definition->name == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid table catalog definition: %d - missing name.",
			 function,
			 list_element_iterator );

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
		list_element = list_element->next_element;
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
     liberror_error_t **error )
{
	libesedb_list_element_t *list_element = NULL;
	static char *function                 = "libesedb_catalog_get_table_definition_by_utf8_name";
	int list_element_iterator             = 0;
	int result                            = 0;

	if( catalog == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog.",
		 function );

		return( -1 );
	}
	if( catalog->table_definition_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid catalog - missing table definition list.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_length > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( table_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table definition.",
		 function );

		return( -1 );
	}
	list_element = catalog->table_definition_list->first_element;

	for( list_element_iterator = 0;
	     list_element_iterator < catalog->table_definition_list->number_of_elements;
	     list_element_iterator++ )
	{
		if( list_element == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected for element: %d.",
			 function,
			 list_element_iterator );

			return( -1 );
		}
		if( list_element->value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid list element: %d - missing value.",
			 function,
			 list_element_iterator );

			return( -1 );
		}
		*table_definition = (libesedb_table_definition_t *) list_element->value;

		if( ( *table_definition )->table_catalog_definition == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid table definition: %d - missing table catalog definition.",
			 function,
			 list_element_iterator );

			return( -1 );
		}
		if( ( *table_definition )->table_catalog_definition->name == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid table catalog definition: %d - missing name.",
			 function,
			 list_element_iterator );

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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: unable to compare UTF-8 string with table catalog definition: %d name.",
			 function,
			 list_element_iterator );

			return( -1 );
		}
		else if( result != 0 )
		{
			return( 1 );
		}
		list_element = list_element->next_element;
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
     liberror_error_t **error )
{
	libesedb_list_element_t *list_element = NULL;
	static char *function                 = "libesedb_catalog_get_table_definition_by_utf16_name";
	int list_element_iterator             = 0;
	int result                            = 0;

	if( catalog == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog.",
		 function );

		return( -1 );
	}
	if( catalog->table_definition_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid catalog - missing table definition list.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_length > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( table_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table definition.",
		 function );

		return( -1 );
	}
	list_element = catalog->table_definition_list->first_element;

	for( list_element_iterator = 0;
	     list_element_iterator < catalog->table_definition_list->number_of_elements;
	     list_element_iterator++ )
	{
		if( list_element == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected for element: %d.",
			 function,
			 list_element_iterator );

			return( -1 );
		}
		if( list_element->value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid list element: %d - missing value.",
			 function,
			 list_element_iterator );

			return( -1 );
		}
		*table_definition = (libesedb_table_definition_t *) list_element->value;

		if( ( *table_definition )->table_catalog_definition == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid table definition: %d - missing table catalog definition.",
			 function,
			 list_element_iterator );

			return( -1 );
		}
		if( ( *table_definition )->table_catalog_definition->name == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid table catalog definition: %d - missing name.",
			 function,
			 list_element_iterator );

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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: unable to compare UTF-16 string with table catalog definition: %d name.",
			 function,
			 list_element_iterator );

			return( -1 );
		}
		else if( result != 0 )
		{
			return( 1 );
		}
		list_element = list_element->next_element;
	}
	*table_definition = NULL;

	return( 0 );
}

/* Reads the catalog
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_read(
     libesedb_catalog_t *catalog,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfdata_cache_t *pages_cache,
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *catalog_definition = NULL;
	libesedb_page_tree_t *catalog_page_tree           = NULL;
	libesedb_table_definition_t *table_definition     = NULL;
	libesedb_values_tree_value_t *values_tree_value   = NULL;
	libfdata_cache_t *catalog_values_cache            = NULL;
	libfdata_tree_t *catalog_values_tree              = NULL;
	libfdata_tree_node_t *catalog_values_tree_node    = NULL;
	uint8_t *catalog_definition_data                  = NULL;
	static char *function                             = "libesedb_catalog_read";
	off64_t node_data_offset                          = 0;
	size_t catalog_definition_data_size               = 0;
	int leaf_node_index                               = 0;
	int number_of_leaf_nodes                          = 0;

	if( catalog == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog.",
		 function );

		return( -1 );
	}
	if( libesedb_page_tree_initialize(
	     &catalog_page_tree,
	     io_handle,
	     pages_vector,
	     pages_cache,
	     LIBESEDB_FDP_OBJECT_IDENTIFIER_CATALOG,
	     NULL,
	     NULL,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create catalog page tree.",
		 function );

		return( -1 );
	}
	/* TODO clone function
	 */
	if( libfdata_tree_initialize(
	     &catalog_values_tree,
	     (intptr_t *) catalog_page_tree,
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
		 "%s: unable to create catalog values tree.",
		 function );

		libesedb_page_tree_free(
		 (intptr_t *) catalog_page_tree,
		 NULL );

		return( -1 );
	}
	if( libfdata_cache_initialize(
	     &catalog_values_cache,
	     LIBESEDB_MAXIMUM_CACHE_ENTRIES_TREE_VALUES,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create catalog values cache.",
		 function );

		libfdata_tree_free(
		 &catalog_values_tree,
		 NULL );

		return( -1 );
	}
	node_data_offset  = LIBESEDB_PAGE_NUMBER_CATALOG - 1;
	node_data_offset *= io_handle->page_size;

	if( libfdata_tree_set_root_node(
	     catalog_values_tree,
	     node_data_offset,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set root node in catalog values tree.",
		 function );

		libfdata_cache_free(
		 &catalog_values_cache,
		 NULL );
		libfdata_tree_free(
		 &catalog_values_tree,
		 NULL );

		return( -1 );
	}
	if( libfdata_tree_get_number_of_leaf_nodes(
	     catalog_values_tree,
	     file_io_handle,
	     catalog_values_cache,
	     &number_of_leaf_nodes,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of leaf nodes from catalog values tree.",
		 function );

		libfdata_cache_free(
		 &catalog_values_cache,
		 NULL );
		libfdata_tree_free(
		 &catalog_values_tree,
		 NULL );

		return( -1 );
	}
	for( leaf_node_index = 0;
	     leaf_node_index < number_of_leaf_nodes;
	     leaf_node_index++ )
	{
		if( libfdata_tree_get_leaf_node_by_index(
		     catalog_values_tree,
		     file_io_handle,
		     catalog_values_cache,
		     leaf_node_index,
		     &catalog_values_tree_node,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve leaf node: %d from catalog values tree.",
			 function,
			 leaf_node_index );

			libfdata_cache_free(
			 &catalog_values_cache,
			 NULL );
			libfdata_tree_free(
			 &catalog_values_tree,
			 NULL );

			return( -1 );
		}
		if( libfdata_tree_node_get_node_value(
		     catalog_values_tree_node,
		     file_io_handle,
		     catalog_values_cache,
		     (intptr_t **) &values_tree_value,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve node value from values tree node.",
			 function );

			libfdata_cache_free(
			 &catalog_values_cache,
			 NULL );
			libfdata_tree_free(
			 &catalog_values_tree,
			 NULL );

			return( -1 );
		}
		if( libesedb_values_tree_value_read_data(
		     values_tree_value,
		     file_io_handle,
		     io_handle,
		     pages_vector,
		     pages_cache,
		     &catalog_definition_data,
		     &catalog_definition_data_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read values tree value data.",
			 function );

			libfdata_cache_free(
			 &catalog_values_cache,
			 NULL );
			libfdata_tree_free(
			 &catalog_values_tree,
			 NULL );

			return( -1 );
		}
		if( libesedb_catalog_definition_initialize(
		     &catalog_definition,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create catalog definition.",
			 function );

			libfdata_cache_free(
			 &catalog_values_cache,
			 NULL );
			libfdata_tree_free(
			 &catalog_values_tree,
			 NULL );

			return( -1 );
		}
		if( libesedb_catalog_definition_read(
		     catalog_definition,
		     catalog_definition_data,
		     catalog_definition_data_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read catalog definition.",
			 function );

			libesedb_catalog_definition_free(
			 (intptr_t *) catalog_definition,
			 NULL );
			libfdata_cache_free(
			 &catalog_values_cache,
			 NULL );
			libfdata_tree_free(
			 &catalog_values_tree,
			 NULL );

			return( -1 );
		}
		if( ( catalog_definition->type != LIBESEDB_CATALOG_DEFINITION_TYPE_TABLE )
		 && ( table_definition == NULL ) )
		{
			/* TODO add build-in table 1 support */
#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: missing table definition for catalog definition type: %" PRIu16 ".\n",
				 function,
				 catalog_definition->type );
			}
#endif
			if( libesedb_catalog_definition_free(
			     (intptr_t *) catalog_definition,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free catalog definition.",
				 function );

				libfdata_cache_free(
				 &catalog_values_cache,
				 NULL );
				libfdata_tree_free(
				 &catalog_values_tree,
				 NULL );

				return( -1 );
			}
			catalog_definition = NULL;
		}
		else switch( catalog_definition->type )
		{
			case LIBESEDB_CATALOG_DEFINITION_TYPE_TABLE:
				table_definition = NULL;

				if( libesedb_table_definition_initialize(
				     &table_definition,
				     catalog_definition,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create table definition.",
					 function );

					libesedb_table_definition_free(
					 (intptr_t *) table_definition,
					 NULL );
					libesedb_catalog_definition_free(
					 (intptr_t *) catalog_definition,
					 NULL );
					libfdata_cache_free(
					 &catalog_values_cache,
					 NULL );
					libfdata_tree_free(
					 &catalog_values_tree,
					 NULL );

					return( -1 );
				}
				catalog_definition = NULL;

				if( libesedb_list_append_value(
				     catalog->table_definition_list,
				     (intptr_t *) table_definition,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append table definition to table definition list.",
					 function );

					libesedb_table_definition_free(
					 (intptr_t *) table_definition,
					 NULL );
					libfdata_cache_free(
					 &catalog_values_cache,
					 NULL );
					libfdata_tree_free(
					 &catalog_values_tree,
					 NULL );

					return( -1 );
				}
				break;

			case LIBESEDB_CATALOG_DEFINITION_TYPE_COLUMN:
				if( libesedb_table_definition_append_column_catalog_definition(
				     table_definition,
				     catalog_definition,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append column catalog definition to table definition.",
					 function );

					libesedb_catalog_definition_free(
					 (intptr_t *) catalog_definition,
					 NULL );
					libfdata_cache_free(
					 &catalog_values_cache,
					 NULL );
					libfdata_tree_free(
					 &catalog_values_tree,
					 NULL );

					return( -1 );
				}
				catalog_definition = NULL;

				break;

			case LIBESEDB_CATALOG_DEFINITION_TYPE_INDEX:
				if( libesedb_table_definition_append_index_catalog_definition(
				     table_definition,
				     catalog_definition,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append index catalog definition to table definition.",
					 function );

					libesedb_catalog_definition_free(
					 (intptr_t *) catalog_definition,
					 NULL );
					libfdata_cache_free(
					 &catalog_values_cache,
					 NULL );
					libfdata_tree_free(
					 &catalog_values_tree,
					 NULL );

					return( -1 );
				}
				catalog_definition = NULL;

				break;

			case LIBESEDB_CATALOG_DEFINITION_TYPE_LONG_VALUE:
				if( libesedb_table_definition_set_long_value_catalog_definition(
				     table_definition,
				     catalog_definition,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set long value catalog definition in table definition.",
					 function );

					libesedb_catalog_definition_free(
					 (intptr_t *) catalog_definition,
					 NULL );
					libfdata_cache_free(
					 &catalog_values_cache,
					 NULL );
					libfdata_tree_free(
					 &catalog_values_tree,
					 NULL );

					return( -1 );
				}
				catalog_definition = NULL;

				break;

			case LIBESEDB_CATALOG_DEFINITION_TYPE_CALLBACK:
				if( libesedb_table_definition_set_callback_catalog_definition(
				     table_definition,
				     catalog_definition,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set callback catalog definition in table definition.",
					 function );

					libesedb_catalog_definition_free(
					 (intptr_t *) catalog_definition,
					 NULL );
					libfdata_cache_free(
					 &catalog_values_cache,
					 NULL );
					libfdata_tree_free(
					 &catalog_values_tree,
					 NULL );

					return( -1 );
				}
				catalog_definition = NULL;

				break;

			default:
#if defined( HAVE_DEBUG_OUTPUT )
				if( libnotify_verbose != 0 )
				{
					libnotify_printf(
					 "%s: unsupported catalog definition type: %" PRIu16 ".\n",
					 function,
					 catalog_definition->type );
				}
#endif
				if( libesedb_catalog_definition_free(
				     (intptr_t *) catalog_definition,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free catalog definition.",
					 function );

					libfdata_cache_free(
					 &catalog_values_cache,
					 NULL );
					libfdata_tree_free(
					 &catalog_values_tree,
					 NULL );

					return( -1 );
				}
				catalog_definition = NULL;

				break;
		}
	}
	if( libfdata_cache_free(
	     &catalog_values_cache,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free catalog values cache.",
		 function );

		libfdata_tree_free(
		 &catalog_values_tree,
		 NULL );

		return( -1 );
	}
	if( libfdata_tree_free(
	     &catalog_values_tree,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free catalog values tree.",
		 function );

		return( -1 );
	}
	return( 1 );
}

