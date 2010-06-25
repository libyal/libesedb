/*
 * Page tree functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>
#include <libnotify.h>

#include "libesedb_catalog_definition.h"
#include "libesedb_data_definition.h"
#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_io_handle.h"
#include "libesedb_libbfio.h"
#include "libesedb_list_type.h"
#include "libesedb_page.h"
#include "libesedb_page_tree.h"
#include "libesedb_table_definition.h"
#include "libesedb_values_tree_value.h"

/* TODO remove */
#include "libesedb_page_tree_values.h"

#include "esedb_page_values.h"

/* Creates a page tree
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_initialize(
     libesedb_page_tree_t **page_tree,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     uint32_t object_identifier,
     libesedb_table_definition_t *table_definition,
     libesedb_table_definition_t *template_table_definition,
     liberror_error_t **error )
{
	libesedb_page_tree_values_t *page_tree_values = NULL;
	static char *function                         = "libesedb_page_tree_initialize";

	if( page_tree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *page_tree == NULL )
	{
		*page_tree = (libesedb_page_tree_t *) memory_allocate(
		                                       sizeof( libesedb_page_tree_t ) );

		if( *page_tree == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create page tree.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *page_tree,
		     0,
		     sizeof( libesedb_page_tree_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear page tree.",
			 function );

			memory_free(
			 *page_tree );

			*page_tree = NULL;

			return( -1 );
		}

		/* TODO remove */

		if( libesedb_tree_node_initialize(
		     &( ( *page_tree )->value_definition_tree_root_node ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value definition tree root node.",
			 function );

			memory_free(
			 *page_tree );

			*page_tree = NULL;

			return( -1 );
		}
		if( libesedb_page_tree_values_initialize(
		     &page_tree_values,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create page tree values.",
			 function );

			libesedb_tree_node_free(
			 &( ( *page_tree )->value_definition_tree_root_node ),
			 NULL,
			 NULL );
			memory_free(
			 *page_tree );

			*page_tree = NULL;

			return( -1 );
		}
		if( libesedb_tree_node_set_value(
		     ( *page_tree )->value_definition_tree_root_node,
		     (intptr_t *) page_tree_values,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set page tree values in value definition root node.",
			 function );

			libesedb_page_tree_values_free(
			 (intptr_t *) page_tree_values,
			 NULL );
			libesedb_tree_node_free(
			 &( ( *page_tree )->value_definition_tree_root_node ),
			 NULL,
			 NULL );
			memory_free(
			 *page_tree );

			*page_tree = NULL;

			return( -1 );
		}
		( *page_tree )->io_handle                 = io_handle;
		( *page_tree )->pages_vector              = pages_vector;
		( *page_tree )->object_identifier         = object_identifier;
		( *page_tree )->table_definition          = table_definition;
		( *page_tree )->template_table_definition = template_table_definition;
	}
	return( 1 );
}

/* Frees page tree
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_free(
     intptr_t *page_tree,
     liberror_error_t **error )
{
	static char *function = "libesedb_page_tree_free";
	int result            = 1;

	if( page_tree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	/* The pages_vector reference
	 * is freed elsewhere
	 */

	/* TODO remove */

	if( libesedb_tree_node_free(
	     &( ( (libesedb_page_tree_t *) page_tree )->value_definition_tree_root_node ),
	     &libesedb_page_tree_values_free,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free value definition tree.",
		 function );

		result = -1;
	}
	memory_free(
	 page_tree );

	return( result );
}

/* Retrieves the value definition of a value definition tree node for the specific index
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_node_get_value_definition_by_index(
     libesedb_tree_node_t *value_definition_tree_node,
     uint32_t value_definition_index,
     libesedb_data_definition_t **value_definition,
     liberror_error_t **error )
{
	libesedb_page_tree_values_t *page_tree_values = NULL;
	libesedb_tree_node_t *child_tree_node         = NULL;
	static char *function                         = "libesedb_page_tree_node_get_value_definition_by_index";
	int child_node_iterator                       = 0;

	if( value_definition_tree_node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value definition tree node.",
		 function );

		return( -1 );
	}
	if( value_definition_tree_node->value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value definition tree node - missing value.",
		 function );

		return( -1 );
	}
	page_tree_values = (libesedb_page_tree_values_t *) value_definition_tree_node->value;
	
	if( value_definition_index < page_tree_values->number_of_value_definitions )
	{
		if( value_definition_tree_node->number_of_child_nodes == 0 )
		{
			if( value_definition == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
				 "%s: invalid value definition.",
				 function );

				return( -1 );
			}
			if( *value_definition != NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
				 "%s: value definition already set.",
				 function );

				return( -1 );
			}
			if( libesedb_page_tree_values_get_value_definition(
			     page_tree_values,
			     value_definition_index,
			     value_definition,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value definition: %d from page tree values.",
				 function,
				 value_definition_index );

				return( -1 );
			}
			return( 1 );
		}
		child_tree_node = value_definition_tree_node->first_child;

		for( child_node_iterator = 0;
		     child_node_iterator < value_definition_tree_node->number_of_child_nodes;
		     child_node_iterator++ )
		{
			if( child_tree_node == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected for child node: %d.",
				 function,
				 child_node_iterator );

				return( -1 );
			}
			if( child_tree_node->value == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected for child node: %d - missing value.",
				 function,
				 child_node_iterator );

				return( -1 );
			}
			page_tree_values = (libesedb_page_tree_values_t *) child_tree_node->value;

			if( value_definition_index < page_tree_values->number_of_value_definitions )
			{
				if( libesedb_page_tree_node_get_value_definition_by_index(
				     child_tree_node,
				     value_definition_index,
				     value_definition,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to traverse child node: %d.",
					 function,
					 child_node_iterator );

					return( -1 );
				}
				return( 1 );
			}
			else
			{
				value_definition_index -= page_tree_values->number_of_value_definitions;
			}
			child_tree_node = child_tree_node->next;
		}
	}
	return( -1 );
}

/* Retrieves the value definition for the specific index
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_get_value_definition(
     libesedb_page_tree_t *page_tree,
     uint32_t value_definition_index,
     libesedb_data_definition_t **value_definition,
     liberror_error_t **error )
{
	static char *function = "libesedb_page_tree_get_value_definition";

	if( page_tree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( page_tree->value_definition_tree_root_node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - missing value definition tree root node.",
		 function );

		return( -1 );
	}
	if( libesedb_page_tree_node_get_value_definition_by_index(
	     page_tree->value_definition_tree_root_node,
	     value_definition_index,
	     value_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value definition: %d.",
		 function,
		 value_definition_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the value definition of a value definition tree node for the specific key
 * Returns 1 if successful, 0 if no value definition was found or -1 on error
 */
int libesedb_page_tree_node_get_value_definition_by_key(
     libesedb_tree_node_t *value_definition_tree_node,
     uint8_t *key,
     size_t key_size,
     libesedb_data_definition_t **value_definition,
     uint8_t flags,
     liberror_error_t **error )
{
	libesedb_page_tree_values_t *page_tree_values = NULL;
	libesedb_tree_node_t *child_tree_node         = NULL;
	static char *function                         = "libesedb_page_tree_node_get_value_definition_by_key";
	size_t compare_size                           = 0;
	size_t data_definition_key_index              = 0;
	size_t key_index                              = 0;
	int child_node_iterator                       = 0;
	int result                                    = 0;
	int16_t compare                               = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *page_key_data                        = NULL;
	size_t page_key_size                          = 0;
#endif

	if( value_definition_tree_node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value definition tree node.",
		 function );

		return( -1 );
	}
	if( key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( key_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid key size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBESEDB_PAGE_KEY_FLAG_REVERSED_KEY ) ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.",
		 function );

		return( -1 );
	}
	if( value_definition_tree_node->number_of_child_nodes == 0 )
	{
		if( value_definition_tree_node->value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid value definition tree node - missing value.",
			 function );

			return( -1 );
		}
		if( value_definition == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid value definition.",
			 function );

			return( -1 );
		}
		if( *value_definition != NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: value definition already set.",
			 function );

			return( -1 );
		}
		result = libesedb_page_tree_values_get_value_definition_by_key(
		          (libesedb_page_tree_values_t *) value_definition_tree_node->value,
		          key,
		          key_size,
		          value_definition,
		          flags,
		          error );

		if( result == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value definition from page tree values by key.",
			 function );

			return( -1 );
		}
		return( result );
	}
	child_tree_node = value_definition_tree_node->first_child;

	for( child_node_iterator = 0;
	     child_node_iterator < value_definition_tree_node->number_of_child_nodes;
	     child_node_iterator++ )
	{
		if( child_tree_node == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected for child node: %d.",
			 function,
			 child_node_iterator );

			return( -1 );
		}
		if( child_tree_node->value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected for child node: %d - missing value.",
			 function,
			 child_node_iterator );

			return( -1 );
		}
		page_tree_values = (libesedb_page_tree_values_t *) child_tree_node->value;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			page_key_data = page_tree_values->key;
			page_key_size = page_tree_values->key_size;

			libnotify_printf(
			 "%s: child page tree values: %d key\t: ",
			 function,
			 child_node_iterator );

			while( page_key_size > 0 )
			{
				libnotify_printf(
				 "%02" PRIx8 " ",
				 *page_key_data );

				page_key_data++;
				page_key_size--;
			}
			libnotify_printf(
			 "\n" );
		}
#endif
		/* Check if the page tree vales key is empty, therefore has no upper bound
		 */
		if( page_tree_values->key_size == 0 )
		{
			compare = -1;
		}
		else
		{
			if( key_size <= page_tree_values->key_size )
			{
				compare_size = key_size;
			}
			else
			{
				compare_size = page_tree_values->key_size;
			}
			data_definition_key_index = 0;

			for( key_index = 0;
			     key_index < compare_size;
			     key_index++ )
			{
				if( ( flags & LIBESEDB_PAGE_KEY_FLAG_REVERSED_KEY ) != 0 )
				{
					compare = (int16_t) key[ key_size - ( key_index + 1 ) ] - (int16_t) page_tree_values->key[ data_definition_key_index ];
				}
				else
				{
					compare = (int16_t) key[ key_index ] - (int16_t) page_tree_values->key[ data_definition_key_index ];
				}
				if( compare != 0 )
				{
					break;
				}
				data_definition_key_index++;
			}
		}
		/* If the key exactly matches the page tree values key,
		 * the value definition is either in the current or the next
		 * page tree values
		 */
		if( compare <= 0 )
		{
			result = libesedb_page_tree_node_get_value_definition_by_key(
				  child_tree_node,
				  key,
				  key_size,
				  value_definition,
				  flags,
				  error );

			if( result == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to traverse child node: %d.",
				 function,
				 child_node_iterator );

				return( -1 );
			}
			else if( result != 0 )
			{
				break;
			}
		}
		child_tree_node = child_tree_node->next;
	}
	return( result );
}

/* Retrieves the value definition for the specific key
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_get_value_definition_by_key(
     libesedb_page_tree_t *page_tree,
     uint8_t *key,
     size_t key_size,
     libesedb_data_definition_t **value_definition,
     uint8_t flags,
     liberror_error_t **error )
{
	static char *function  = "libesedb_page_tree_get_value_definition_by_key";

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *page_key_data = NULL;
	size_t page_key_size   = 0;
#endif

	if( page_tree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( page_tree->value_definition_tree_root_node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - missing value definition tree root node.",
		 function );

		return( -1 );
	}
	if( key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( key_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid key size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		page_key_data = key;
		page_key_size = key_size;

		libnotify_printf(
		 "%s: requested key\t\t\t\t: ",
		 function );

		while( page_key_size > 0 )
		{
			libnotify_printf(
			 "%02" PRIx8 " ",
			 *page_key_data );

			page_key_data++;
			page_key_size--;
		}
		libnotify_printf(
		 "\n" );
	}
#endif
	if( libesedb_page_tree_node_get_value_definition_by_key(
	     page_tree->value_definition_tree_root_node,
	     key,
	     key_size,
	     value_definition,
	     flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value definition by key.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads a page tree and its values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     uint32_t father_data_page_number,
     uint8_t flags,
     liberror_error_t **error )
{
	libesedb_page_t *page                         = NULL;
	libesedb_page_tree_values_t *page_tree_values = NULL;
	static char *function                         = "libesedb_page_tree_read";

	if( page_tree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( page_tree->value_definition_tree_root_node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - missing value definition tree root node.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: reading page tree with FDP number\t\t: %" PRIu32 "\n",
		 function,
		 father_data_page_number );
	}
#endif

	if( libfdata_vector_get_element_value_by_index(
	     page_tree->pages_vector,
	     file_io_handle,
	     (int) father_data_page_number - 1,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 ".",
		 function,
		 father_data_page_number );

		return( -1 );
	}
	if( page == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page.",
		 function );

		return( -1 );
	}
	page_tree->object_identifier = page->father_data_page_object_identifier;

	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) != 0 )
	{
		if( libesedb_tree_node_get_value(
		     page_tree->value_definition_tree_root_node,
		     (intptr_t **) &page_tree_values,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page tree values in value definition tree root node.",
			 function );

			return( -1 );
		}
		if( libesedb_page_tree_read_leaf_page_old(
		     page_tree,
		     file_io_handle,
		     father_data_page_number,
		     page_tree_values,
		     flags,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read leaf page: %" PRIu32 ".",
			 function,
			 father_data_page_number );

			return( -1 );
		}
	}
	else
	{
		if( libesedb_page_tree_read_branch_page_old(
		     page_tree,
		     file_io_handle,
		     father_data_page_number,
		     page_tree->value_definition_tree_root_node,
		     flags,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read branch page: %" PRIu32 ".",
			 function,
			 father_data_page_number );

			return( -1 );
		}
	}
	return( 1 );
}

/* Reads the root page
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_root_page(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     uint32_t page_number,
     liberror_error_t **error )
{
	libesedb_page_t *page             = NULL;
	libesedb_page_value_t *page_value = NULL;
	static char *function             = "libesedb_page_tree_read_root_page";
	uint32_t extent_space             = 0;
	uint32_t space_tree_page_number   = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t initial_number_of_pages  = 0;
	uint32_t value_32bit              = 0;
#endif

	if( page_tree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_by_index(
	     page_tree->pages_vector,
	     file_io_handle,
	     (int) page_number - 1,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 ".",
		 function,
		 page_number );

		return( -1 );
	}
	if( page == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page.",
		 function );

		return( -1 );
	}
	if( libesedb_page_get_value(
	     page,
	     0,
	     &page_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: 0.",
		 function );

		return( -1 );
	}
	if( page_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value.",
		 function );

		return( -1 );
	}
	if( page_value->data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page value - missing data.",
		 function );

		return( -1 );
	}
	if( page_value->size != 16 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported size of page value.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_root_page_header_t *) page_value->data )->space_tree_page_number,
	 space_tree_page_number );

	/* TODO handle the root page header */

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_root_page_header_t *) page_value->data )->initial_number_of_pages,
		 initial_number_of_pages );
		libnotify_printf(
		 "%s: (header) initial number of pages\t: %" PRIu32 "\n",
		 function,
		 initial_number_of_pages );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_root_page_header_t *) page_value->data )->parent_father_data_page_number,
		 value_32bit );
		libnotify_printf(
		 "%s: (header) parent FDP number\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_root_page_header_t *) page_value->data )->extent_space,
		 extent_space );
		libnotify_printf(
		 "%s: (header) extent space\t\t: %" PRIu32 "\n",
		 function,
		 extent_space );

		libnotify_printf(
		 "%s: (header) space tree page number\t: %" PRIu32 " (0x%08" PRIx32 ")\n",
		 function,
		 space_tree_page_number,
		 space_tree_page_number );

		libnotify_printf(
		 "%s: (header) primary extent\t\t: %" PRIu32 "-%c\n",
		 function,
		 initial_number_of_pages,
		 ( extent_space == 0 ? 's' : 'm' ) );

		libnotify_printf(
		 "\n" );
	}
#endif

	/* Read the space tree pages
	 */
	if( extent_space > 0 )
	{
		if( ( space_tree_page_number == 0 )
		 && ( space_tree_page_number >= 0xff000000UL ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported space tree page number.",
			 function,
			 space_tree_page_number );

			return( -1 );
		}
		/* Read the owned pages space tree page
		 */
		if( libesedb_page_tree_read_space_tree_page(
		     page_tree,
		     file_io_handle,
		     space_tree_page_number,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read space tree page: %" PRIu32 ".",
			 function,
			 space_tree_page_number );

			return( -1 );
		}
		/* Read the available pages space tree page
		 */
		space_tree_page_number += 1;

		if( libesedb_page_tree_read_space_tree_page(
		     page_tree,
		     file_io_handle,
		     space_tree_page_number,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read space tree page: %" PRIu32 ".",
			 function,
			 space_tree_page_number );

			return( -1 );
		}
	}
	return( 1 );
}

/* Reads the branch page
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_branch_page_old(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     uint32_t page_number,
     libesedb_tree_node_t *value_definition_tree_node,
     uint8_t flags,
     liberror_error_t **error )
{
	libesedb_page_t *child_page                            = NULL;
	libesedb_page_t *page                                  = NULL;
	libesedb_page_value_t *page_value                      = NULL;
	libesedb_page_tree_values_t *child_page_tree_values    = NULL;
	libesedb_page_tree_values_t *page_tree_values          = NULL;
	libesedb_tree_node_t *child_value_definition_tree_node = NULL;
	uint8_t *page_value_data                               = NULL;
	static char *function                                  = "libesedb_page_tree_read_branch_page_old";
	uint32_t child_page_number                             = 0;
	uint32_t previous_child_page_number                    = 0;
	uint32_t previous_next_child_page_number               = 0;
	uint32_t supported_flags                               = 0;
	uint16_t number_of_page_values                         = 0;
	uint16_t common_key_size                               = 0;
	uint16_t local_key_size                                = 0;
	uint16_t page_value_index                              = 0;
	uint16_t page_value_size                               = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *page_key_data                                 = NULL;
	size_t page_key_size                                   = 0;
#endif

	if( page_tree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( page_tree->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_by_index(
	     page_tree->pages_vector,
	     file_io_handle,
	     (int) page_number - 1,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 ".",
		 function,
		 page_number );

		return( -1 );
	}
	if( page == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page.",
		 function );

		return( -1 );
	}
	if( page_tree->object_identifier != page->father_data_page_object_identifier )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: mismatch in father data page object identifier (tree: %" PRIu32 " != page: %" PRIu32 ").",
		 function,
		 page_tree->object_identifier,
		 page->father_data_page_object_identifier );

		return( -1 );
	}
	supported_flags = LIBESEDB_PAGE_FLAG_IS_ROOT
	                | LIBESEDB_PAGE_FLAG_IS_PARENT
	                | LIBESEDB_PAGE_FLAG_IS_INDEX
	                | LIBESEDB_PAGE_FLAG_IS_LONG_VALUE
	                | LIBESEDB_PAGE_FLAG_0x0400
	                | LIBESEDB_PAGE_FLAG_0x0800
	                | LIBESEDB_PAGE_FLAG_IS_NEW_RECORD_FORMAT;

	if( ( page->flags & ~supported_flags ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported page flags: 0x%08" PRIx32 ".",
		 function,
		 page->flags );

		return( -1 );
	}
	if( page->previous_page_number != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported previous page number: %" PRIu32 ".",
		 function,
		 page->previous_page_number );

		return( -1 );
	}
	if( page->next_page_number != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported next page number: %" PRIu32 ".",
		 function,
		 page->next_page_number );

		return( -1 );
	}
	if( libesedb_page_get_number_of_values(
	     page,
	     &number_of_page_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of page values.",
		 function );

		return( -1 );
	}
	if( number_of_page_values == 0 )
	{
		return( 1 );
	}
	if( libesedb_page_get_value(
	     page,
	     0,
	     &page_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: 0.",
		 function );

		return( -1 );
	}
	if( page_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: value: %03" PRIu16 " value:\n",
		 function,
		 page_value_index );
		libnotify_print_data(
		 page_value->data,
		 (size_t) page_value->size );
		libnotify_printf(
		 "%s: value: %03" PRIu16 " page tag flags\t: 0x%02" PRIx8 "",
		 function,
		 page_value_index,
		 page_value->flags );
		libesedb_debug_print_page_tag_flags(
		 page_value->flags );
		libnotify_printf(
		 "\n" );
	}
#endif
	if( libesedb_tree_node_get_value(
	     value_definition_tree_node,
	     (intptr_t **) &page_tree_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page tree values in value definition tree node.",
		 function );

		return( -1 );
	}
	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_ROOT ) != 0 )
	{
		if( libesedb_page_tree_read_root_page(
		     page_tree,
		     file_io_handle,
		     page_number,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read root page: %" PRIu32 ".",
			 function,
			 page_number );

			return( -1 );
		}
	}
	else
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			page_value_data = page_value->data;
			page_value_size = page_value->size;

			libnotify_printf(
			 "%s: (header) common page key\t\t: ",
			 function );

			while( page_value_size > 0 )
			{
				libnotify_printf(
				 "%02" PRIx8 " ",
				 *page_value_data );

				page_value_data++;
				page_value_size--;
			}
			libnotify_printf(
			 "\n" );
			libnotify_printf(
			 "\n" );
		}
#endif
		if( libesedb_page_tree_values_set_common_key(
		     page_tree_values,
		     page_value->data,
		     (size_t) page_value->size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set common key in page tree values.",
			 function );

			return( -1 );
		}
	}
	for( page_value_index = 1;
	     page_value_index < number_of_page_values;
	     page_value_index++ )
	{
		/* Make sure the page is available
		 */
		if( libfdata_vector_get_element_value_by_index(
		     page_tree->pages_vector,
		     file_io_handle,
		     (int) page_number - 1,
		     (intptr_t **) &page,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page: %" PRIu32 ".",
			 function,
			 page_number );

			return( -1 );
		}
		if( libesedb_page_get_value(
		     page,
		     page_value_index,
		     &page_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page value: %" PRIu16 ".",
			 function,
			 page_value_index );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: value: %03" PRIu16 " value:\n",
			 function,
			 page_value_index );
			libnotify_print_data(
			 page_value->data,
			 (size_t) page_value->size );

			libnotify_printf(
			 "%s: value: %03" PRIu16 " page tag flags\t: 0x%02" PRIx8 "",
			 function,
			 page_value_index,
			 page_value->flags );
			libesedb_debug_print_page_tag_flags(
			 page_value->flags );
			libnotify_printf(
			 "\n" );
		}
#endif
		page_value_data = page_value->data;
		page_value_size = page_value->size;

		if( libesedb_page_tree_values_initialize(
		     &child_page_tree_values,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create child page tree values.",
			 function );

			return( -1 );
		}
		if( ( page_value->flags & LIBESEDB_PAGE_TAG_FLAG_HAS_COMMON_KEY_SIZE ) != 0 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 page_value_data,
			 common_key_size );

			page_value_data += 2;
			page_value_size -= 2;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: value: %03" PRIu16 " common key size\t: %" PRIu16 "\n",
				 function,
				 page_value_index,
				 common_key_size );
			}
#endif
			if( libesedb_page_tree_values_set_key_common(
			     child_page_tree_values,
			     page_tree_values->common_key,
			     common_key_size,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set key common part in child page tree values: %d.",
				 function,
				 page_value_index );

				return( -1 );
			}
		}
		byte_stream_copy_to_uint16_little_endian(
		 page_value_data,
		 local_key_size );

		page_value_data += 2;
		page_value_size -= 2;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: value: %03" PRIu16 " local key size\t: %" PRIu16 "\n",
			 function,
			 page_value_index,
			 local_key_size );
		}
#endif

		if( local_key_size > page_value_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: local key size exceeds page value size.",
			 function );

			return( -1 );
		}
		if( libesedb_page_tree_values_set_key_local(
		     child_page_tree_values,
		     page_value_data,
		     local_key_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set key local part in child page tree values: %d.",
			 function,
			 page_value_index );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: value: %03" PRIu16 " local key value\t: ",
			 function,
			 page_value_index );
		}
		page_key_size = local_key_size;

		while( page_key_size > 0 )
		{
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%02" PRIx8 " ",
				 *page_value_data );
			}
			page_value_data++;
			page_value_size--;
			page_key_size--;
		}
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "\n" );
		}
#else
		page_value_data += local_key_size;
		page_value_size -= local_key_size;
#endif
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: value: %03" PRIu16 " key value\t\t: ",
			 function,
			 page_value_index );

			page_key_data = child_page_tree_values->key;
			page_key_size = child_page_tree_values->key_size;

			while( page_key_size > 0 )
			{
				libnotify_printf(
				 "%02" PRIx8 " ",
				 *page_key_data );

				page_key_data++;
				page_key_size--;
			}
			libnotify_printf(
			 "\n" );
		}
#endif
		byte_stream_copy_to_uint32_little_endian(
		 page_value_data,
		 child_page_number );

		page_value_data += 4;
		page_value_size -= 4;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: value: %03" PRIu16 " child page number\t: %" PRIu32 "\n",
			 function,
			 page_value_index,
			 child_page_number );
			libnotify_printf(
			 "\n" );

			if( page_value_size > 0 )
			{
				libnotify_printf(
				 "%s: value: %03" PRIu16 " trailing data:\n",
				 function,
				 page_value_index );
				libnotify_print_data(
				 page_value_data,
				 page_value_size );
			}
		}
#endif

		if( child_page_number > page_tree->io_handle->last_page_number )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: value: %03" PRIu16 " ignoring child page: %" PRIu32 " exceeds last page number: %" PRIu32 "\n",
				 function,
				 page_value_index,
				 child_page_number,
				 page_tree->io_handle->last_page_number );
			}
#endif

			libesedb_page_tree_values_free(
			 (intptr_t *) child_page_tree_values,
			 NULL );

			child_page_tree_values = NULL;

			continue;
		}
		if( libfdata_vector_get_element_value_by_index(
		     page_tree->pages_vector,
		     file_io_handle,
		     (int) child_page_number - 1,
		     (intptr_t **) &child_page,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve child page: %" PRIu32 ".",
			 function,
			 child_page_number );

			libesedb_page_tree_values_free(
			 (intptr_t *) child_page_tree_values,
			 NULL );

			return( -1 );
		}
		if( child_page == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing child page.",
			 function );

			libesedb_page_tree_values_free(
			 (intptr_t *) child_page_tree_values,
			 NULL );

			return( -1 );
		}
		if( libesedb_tree_node_initialize(
		     &child_value_definition_tree_node,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create child value definition tree node.",
			 function );

			libesedb_page_tree_values_free(
			 (intptr_t *) child_page_tree_values,
			 NULL );

			return( -1 );
		}
		if( libesedb_tree_node_set_value(
		     child_value_definition_tree_node,
		     (intptr_t *) child_page_tree_values,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set child page tree values in child value definition tree node.",
			 function );

			libesedb_page_tree_values_free(
			 (intptr_t *) child_page_tree_values,
			 NULL );

			return( -1 );
		}
		child_page_tree_values = NULL;

		if( libesedb_tree_node_append_node(
		     value_definition_tree_node,
		     child_value_definition_tree_node,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append child node to value definition tree node.",
			 function );

			libesedb_tree_node_free(
			 &child_value_definition_tree_node,
			 &libesedb_page_tree_values_free,
			 NULL );

			return( -1 );
		}
		if( ( child_page->flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) != 0 )
		{
			if( libnotify_verbose != 0 )
			{
				if( page_value_index > 1 )
				{
					if( child_page->page_number != previous_next_child_page_number )
					{
						libnotify_printf(
						 "%s: mismatch in child page number (%" PRIu32 " != %" PRIu32 ").\n",
						 function,
						 previous_next_child_page_number,
						 child_page->page_number );
					}
					if( child_page->previous_page_number != previous_child_page_number )
					{
						libnotify_printf(
						 "%s: mismatch in previous child page number (%" PRIu32 " != %" PRIu32 ").\n",
						 function,
						 previous_child_page_number,
						 child_page->previous_page_number );
					}
				}
/* TODO need the actual values for the following checks
				if( page_value_index == 1 )
				{
					if( child_page->previous_page_number != 0 )
					{
						libnotify_printf(
						 "%s: mismatch in previous child page number (%" PRIu32 " != %" PRIu32 ").",
						 function,
						 0,
						 child_page->previous_page_number );
					}
				}
				if( page_value_index == ( number_of_page_values - 1 ) )
				{
					if( child_page->next_page_number != 0 )
					{
						libnotify_printf(
						 "%s: mismatch in next child page number (%" PRIu32 " != %" PRIu32 ").",
						 function,
						 0,
						 child_page->previous_page_number );
					}
				}
*/
			}
			previous_child_page_number      = child_page->page_number;
			previous_next_child_page_number = child_page->next_page_number;

			if( libesedb_tree_node_get_value(
			     child_value_definition_tree_node,
			     (intptr_t **) &child_page_tree_values,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve page tree values in value definition tree node.",
				 function );

				return( -1 );
			}
			if( libesedb_page_tree_read_leaf_page_old(
			     page_tree,
			     file_io_handle,
			     child_page_number,
			     child_page_tree_values,
			     flags,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read leaf page: %" PRIu32 ".",
				 function,
				 child_page_number );

				libesedb_tree_node_free(
				 &child_value_definition_tree_node,
				 &libesedb_page_tree_values_free,
				 NULL );

				return( -1 );
			}
			child_page_tree_values = NULL;
		}
		else
		{
			if( libesedb_page_tree_read_branch_page_old(
			     page_tree,
			     file_io_handle,
			     child_page_number,
			     child_value_definition_tree_node,
			     flags,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read branch page: %" PRIu32 ".",
				 function,
				 child_page_number );

				libesedb_tree_node_free(
				 &child_value_definition_tree_node,
				 &libesedb_page_tree_values_free,
				 NULL );

				return( -1 );
			}
		}
		if( child_value_definition_tree_node->value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid child value definition tree node - missing value.",
			 function );

			return( -1 );
		}
		page_tree_values->number_of_value_definitions += ( (libesedb_page_tree_values_t *) child_value_definition_tree_node->value )->number_of_value_definitions;

		child_value_definition_tree_node = NULL;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

/* Reads the space tree page
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_space_tree_page(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     uint32_t page_number,
     liberror_error_t **error )
{
	libesedb_page_t *page             = NULL;
	libesedb_page_value_t *page_value = NULL;
	static char *function             = "libesedb_page_tree_read_space_tree_page";
	uint32_t required_flags           = 0;
	uint32_t supported_flags          = 0;
	uint16_t number_of_page_values    = 0;
	uint16_t key_size                 = 0;
	uint16_t page_value_index         = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit              = 0;
	uint32_t total_number_of_pages    = 0;
#endif

	if( page_tree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_by_index(
	     page_tree->pages_vector,
	     file_io_handle,
	     (int) page_number - 1,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 ".",
		 function,
		 page_number );

		return( -1 );
	}
	if( page == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page.",
		 function );

		return( -1 );
	}
	if( page_tree->object_identifier != page->father_data_page_object_identifier )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: mismatch in father data page object identifier (tree: %" PRIu32 " != page: %" PRIu32 ").",
		 function,
		 page_tree->object_identifier,
		 page->father_data_page_object_identifier );

		return( -1 );
	}
	required_flags = LIBESEDB_PAGE_FLAG_IS_ROOT
	               | LIBESEDB_PAGE_FLAG_IS_SPACE_TREE;

	if( ( page->flags & required_flags ) != required_flags )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing required page flags: 0x%08" PRIx32 ".",
		 function,
		 page->flags );

		return( -1 );
	}
	supported_flags = required_flags
	                | LIBESEDB_PAGE_FLAG_IS_LEAF
	                | LIBESEDB_PAGE_FLAG_IS_PARENT
	                | LIBESEDB_PAGE_FLAG_IS_INDEX
	                | LIBESEDB_PAGE_FLAG_IS_LONG_VALUE
	                | LIBESEDB_PAGE_FLAG_0x0400
	                | LIBESEDB_PAGE_FLAG_0x0800
	                | LIBESEDB_PAGE_FLAG_IS_NEW_RECORD_FORMAT;

	if( ( page->flags & ~supported_flags ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported page flags: 0x%08" PRIx32 ".",
		 function,
		 page->flags );

		return( -1 );
	}
	if( page->previous_page_number != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported previous page number: %" PRIu32 ".",
		 function,
		 page->previous_page_number );

		return( -1 );
	}
	if( page->next_page_number != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported next page number: %" PRIu32 ".",
		 function,
		 page->next_page_number );

		return( -1 );
	}
	if( libesedb_page_get_number_of_values(
	     page,
	     &number_of_page_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of page values.",
		 function );

		return( -1 );
	}
	if( number_of_page_values == 0 )
	{
		return( 1 );
	}
	if( libesedb_page_get_value(
	     page,
	     page_value_index,
	     &page_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: %" PRIu16 ".",
		 function,
		 page_value_index );

		return( -1 );
	}
	if( page_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page value.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: header:\n",
		 function );
		libnotify_print_data(
		 page_value->data,
		 (size_t) page_value->size );
	}
#endif

	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) != 0 )
	{
		if( page_value->size == 16 )
		{
			if( ( page_value->data[  0 ] != 0 )
			 || ( page_value->data[  1 ] != 0 )
			 || ( page_value->data[  2 ] != 0 )
			 || ( page_value->data[  3 ] != 0 )
			 || ( page_value->data[  4 ] != 0 )
			 || ( page_value->data[  5 ] != 0 )
			 || ( page_value->data[  6 ] != 0 )
			 || ( page_value->data[  7 ] != 0 )
			 || ( page_value->data[  8 ] != 0 )
			 || ( page_value->data[  9 ] != 0 )
			 || ( page_value->data[ 10 ] != 0 )
			 || ( page_value->data[ 11 ] != 0 )
			 || ( page_value->data[ 12 ] != 0 )
			 || ( page_value->data[ 13 ] != 0 )
			 || ( page_value->data[ 14 ] != 0 )
			 || ( page_value->data[ 15 ] != 0 ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported header.",
				 function );

				return( -1 );
			}
		}
		else if( page_value->size != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported header size.",
			 function );

			return( -1 );
		}
	}
	/* TODO handle the space tree page header */

	for( page_value_index = 1;
	     page_value_index < number_of_page_values;
	     page_value_index++ )
	{
		if( libesedb_page_get_value(
		     page,
		     page_value_index,
		     &page_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page value: %" PRIu16 ".",
			 function,
			 page_value_index );

			return( -1 );
		}
		if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) != 0 )
		{
			if( ( page_value->flags & 0x05 ) != 0 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported page value flags: 0x%02" PRIx8 ".",
				 function,
				 page_value->flags );

				return( -1 );
			}
			if( (size_t) page_value->size != sizeof( esedb_space_tree_page_entry_t ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported page value size: %" PRIu16 ".",
				 function,
				 page_value->size );

				return( -1 );
			}
			byte_stream_copy_to_uint16_little_endian(
			 ( (esedb_space_tree_page_entry_t *) page_value->data )->key_size,
			 key_size );

			if( key_size != 4 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported key size: %" PRIu16 ".",
				 function,
				 key_size);

				return( -1 );
			}
			/* TODO handle the space tree page values */

#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: value: %03" PRIu16 " key size\t\t: %" PRIu16 "\n",
				 function,
				 page_value_index,
				 key_size );

				byte_stream_copy_to_uint32_little_endian(
				 ( (esedb_space_tree_page_entry_t *) page_value->data )->last_page_number,
				 value_32bit );
				libnotify_printf(
				 "%s: value: %03" PRIu16 " key value\t\t: %" PRIu32 " (0x%08" PRIx32 ")\n",
				 function,
				 page_value_index,
				 value_32bit,
				 value_32bit );

				byte_stream_copy_to_uint32_little_endian(
				 ( (esedb_space_tree_page_entry_t *) page_value->data )->number_of_pages,
				 value_32bit );
				libnotify_printf(
				 "%s: value: %03" PRIu16 " number of pages\t: %" PRIu32 "\n",
				 function,
				 page_value_index,
				 value_32bit );

				libnotify_printf(
				 "\n" );

				if( ( page_value->flags & LIBESEDB_PAGE_TAG_FLAG_IS_DEFUNCT ) == 0 )
				{
					total_number_of_pages += value_32bit;
				}
			}
#endif
		}
		else if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_PARENT ) != 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: data:\n",
				 function );
				libnotify_print_data(
				 page_value->data,
				 (size_t) page_value->size );
			}
#endif
#ifdef TODO
			/* TODO */
			if( libesedb_page_tree_read_space_tree_page(
			     page_tree,
			     file_io_handle,
			     space_tree_page_number,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read space tree page: %" PRIu32 ".",
				 function,
				 space_tree_page_number );

				return( -1 );
			}
#endif
		}
	}

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: total number of pages\t\t: %" PRIu32 "\n",
		 function,
		 total_number_of_pages );

		libnotify_printf(
		 "\n" );
	}
#endif

	return( 1 );
}

/* Reads the leaf page
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_leaf_page_old(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     uint32_t page_number,
     libesedb_page_tree_values_t *page_tree_values,
     uint8_t flags,
     liberror_error_t **error )
{
	libesedb_data_definition_t *data_definition                    = NULL;
	libesedb_data_definition_t *long_value_data_definition         = NULL;
	libesedb_list_t *template_table_column_catalog_definition_list = NULL;
	libesedb_page_t *page                                          = NULL;
	libesedb_page_value_t *page_value                              = NULL;
	libesedb_table_definition_t *table_definition                  = NULL;
	uint8_t *page_value_data                                       = NULL;
	static char *function                                          = "libesedb_page_tree_read_leaf_page_old";
	uint32_t long_value_data_segment_offset                        = 0;
	uint32_t required_flags                                        = 0;
	uint32_t supported_flags                                       = 0;
	uint16_t common_key_size                                       = 0;
	uint16_t local_key_size                                        = 0;
	uint16_t number_of_page_values                                 = 0;
	uint16_t page_value_index                                      = 0;
	uint16_t page_value_offset                                     = 0;
	uint16_t page_value_size                                       = 0;
	int result                                                     = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *page_key_data                                         = NULL;
	uint16_t page_key_size                                         = 0;
#endif

	if( page_tree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( page_tree_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree values.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_by_index(
	     page_tree->pages_vector,
	     file_io_handle,
	     (int) page_number - 1,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 ".",
		 function,
		 page_number );

		return( -1 );
	}
	if( page == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page.",
		 function );

		return( -1 );
	}
	if( page_tree->object_identifier != page->father_data_page_object_identifier )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: mismatch in father data page object identifier (tree: %" PRIu32 " != page: %" PRIu32 ").",
		 function,
		 page_tree->object_identifier,
		 page->father_data_page_object_identifier );

		return( -1 );
	}
	required_flags = LIBESEDB_PAGE_FLAG_IS_LEAF;

	if( ( page->flags & required_flags ) != required_flags )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing required page flags: 0x%08" PRIx32 ".",
		 function,
		 page->flags );

		return( -1 );
	}
	supported_flags = required_flags
	                | LIBESEDB_PAGE_FLAG_IS_ROOT
	                | LIBESEDB_PAGE_FLAG_IS_INDEX
	                | LIBESEDB_PAGE_FLAG_IS_LONG_VALUE
	                | LIBESEDB_PAGE_FLAG_0x0400
	                | LIBESEDB_PAGE_FLAG_0x0800
	                | LIBESEDB_PAGE_FLAG_IS_NEW_RECORD_FORMAT;

	if( ( page->flags & ~supported_flags ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported page flags: 0x%08" PRIx32 ".",
		 function,
		 page->flags );

		return( -1 );
	}
	if( libesedb_page_get_number_of_values(
	     page,
	     &number_of_page_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of page values.",
		 function );

		return( -1 );
	}
	if( number_of_page_values == 0 )
	{
		return( 1 );
	}
	if( libesedb_page_get_value(
	     page,
	     page_value_index,
	     &page_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: %" PRIu16 ".",
		 function,
		 page_value_index );

		return( -1 );
	}
	if( page_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: value: %03" PRIu16 " value:\n",
		 function,
		 page_value_index );
		libnotify_print_data(
		 page_value->data,
		 (size_t) page_value->size );
		libnotify_printf(
		 "%s: value: %03" PRIu16 " page tag flags\t\t: 0x%02" PRIx8 "",
		 function,
		 page_value_index,
		 page_value->flags );
		libesedb_debug_print_page_tag_flags(
		 page_value->flags );
		libnotify_printf(
		 "\n" );
	}
#endif
	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_ROOT ) != 0 )
	{
		if( libesedb_page_tree_read_root_page(
		     page_tree,
		     file_io_handle,
		     page_number,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read root page: %" PRIu32 ".",
			 function,
			 page_number );

			return( -1 );
		}
	}
	else
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			page_value_data = page_value->data;
			page_value_size = page_value->size;

			libnotify_printf(
			 "%s: (header) common page key\t\t: ",
			 function );

			while( page_value_size > 0 )
			{
				libnotify_printf(
				 "%02" PRIx8 " ",
				 *page_value_data );

				page_value_data++;
				page_value_size--;
			}
			libnotify_printf(
			 "\n" );
			libnotify_printf(
			 "\n" );
		}
#endif
		if( libesedb_page_tree_values_set_common_key(
		     page_tree_values,
		     page_value->data,
		     (size_t) page_value->size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set common key in page tree values.",
			 function );

			return( -1 );
		}
	}
	for( page_value_index = 1;
	     page_value_index < number_of_page_values;
	     page_value_index++ )
	{
		if( libesedb_page_get_value(
		     page,
		     page_value_index,
		     &page_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page value: %" PRIu16 ".",
			 function,
			 page_value_index );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: value: %03" PRIu16 " value:\n",
			 function,
			 page_value_index );
			libnotify_print_data(
			 page_value->data,
			 (size_t) page_value->size );

			libnotify_printf(
			 "%s: value: %03" PRIu16 " page tag flags\t\t\t: 0x%02" PRIx8 "",
			 function,
			 page_value_index,
			 page_value->flags );
			libesedb_debug_print_page_tag_flags(
			 page_value->flags );
			libnotify_printf(
			 "\n" );
		}
#endif

		page_value_data   = page_value->data;
		page_value_offset = 0;
		page_value_size   = page_value->size;

		if( libesedb_data_definition_initialize(
		     &data_definition,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data definition.",
			 function );

			return( -1 );
		}
		if( ( page_value->flags & LIBESEDB_PAGE_TAG_FLAG_HAS_COMMON_KEY_SIZE ) != 0 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 page_value_data,
			 common_key_size );

			page_value_data   += 2;
			page_value_offset += 2;
			page_value_size   -= 2;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: value: %03" PRIu16 " common key size\t\t\t: %" PRIu16 "\n",
				 function,
				 page_value_index,
				 common_key_size );
			}
#endif
			if( common_key_size > page_tree_values->common_key_size )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
				 "%s: common key size exceeds maximum.",
				 function );

				libesedb_data_definition_free(
				 (intptr_t *) data_definition,
				 NULL );

				return( -1 );
			}
			if( libesedb_data_definition_set_key_common(
			     data_definition,
			     page_tree_values->common_key,
			     common_key_size,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set key common part in data definition.",
				 function );

				libesedb_data_definition_free(
				 (intptr_t *) data_definition,
				 NULL );

				return( -1 );
			}
		}
		byte_stream_copy_to_uint16_little_endian(
		 page_value_data,
		 local_key_size );

		page_value_data   += 2;
		page_value_offset += 2;
		page_value_size   -= 2;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: value: %03" PRIu16 " local key size\t\t\t: %" PRIu16 "\n",
			 function,
			 page_value_index,
			 local_key_size );
		}
#endif
		if( local_key_size > page_value_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: local key size exceeds page value size.",
			 function );

			libesedb_data_definition_free(
			 (intptr_t *) data_definition,
			 NULL );

			return( -1 );
		}
		if( libesedb_data_definition_set_key_local(
		     data_definition,
		     page_value_data,
		     local_key_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set key local part in data definition.",
			 function );

			libesedb_data_definition_free(
			 (intptr_t *) data_definition,
			 NULL );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: value: %03" PRIu16 " local key value\t\t\t: ",
			 function,
			 page_value_index );
		}
		page_key_size = local_key_size;

		while( page_key_size > 0 )
		{
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%02" PRIx8 " ",
				 *page_value_data );
			}
			page_value_data++;
			page_value_offset++;
			page_value_size--;
			page_key_size--;
		}
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "\n" );
		}
#else
		page_value_data   += local_key_size;
		page_value_offset += local_key_size;
		page_value_size   -= local_key_size;
#endif

#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: value: %03" PRIu16 " key value\t\t\t\t: ",
			 function,
			 page_value_index );

			page_key_data = data_definition->key;
			page_key_size = data_definition->key_size;

			while( page_key_size > 0 )
			{
				libnotify_printf(
				 "%02" PRIx8 " ",
				 *page_key_data );

				page_key_data++;
				page_key_size--;
			}
			libnotify_printf(
			 "\n" );
		}
#endif
		if( page_tree->table_definition == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid page tree - missing table definition.",
			 function );

			libesedb_data_definition_free(
			 (intptr_t *) data_definition,
			 NULL );

			return( -1 );
		}
		if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_INDEX ) != 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: value: %03" PRIu16 " index value\t\t\t: ",
				 function,
				 page_value_index );
			}
			while( page_value_size > 0 )
			{
				if( libnotify_verbose != 0 )
				{
					libnotify_printf(
					 "%02" PRIx8 " ",
					 *page_value_data );
				}
				page_value_data++;
				page_value_offset++;
				page_value_size--;
			}
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "\n" );
				libnotify_printf(
				 "\n" );
			}
#endif
		}
		else if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_LONG_VALUE ) != 0 )
		{
			if( data_definition->key_size == 4 )
			{
				if( libesedb_data_definition_read_long_value(
				     data_definition,
				     page_value_data,
				     page_value_size,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read page value: %d long value data definition.",
					 function,
					 page_value_index );

					libesedb_data_definition_free(
					 (intptr_t *) data_definition,
					 NULL );

					return( -1 );
				}
				long_value_data_definition = data_definition;

				if( ( page_value->flags & LIBESEDB_PAGE_TAG_FLAG_IS_DEFUNCT ) == 0 )
				{
					if( libesedb_page_tree_values_append_value_definition(
					     page_tree_values,
					     data_definition,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
						 "%s: unable to append value data definition to page tree values.",
						 function );

						libesedb_data_definition_free(
						 (intptr_t *) data_definition,
						 NULL );

						return( -1 );
					}
					data_definition = NULL;
				}
				/* TODO are defunct data definition of any value recovering
				 */
			}
			else if( data_definition->key_size == 8 )
			{
				if( ( page_value->flags & LIBESEDB_PAGE_TAG_FLAG_IS_DEFUNCT ) == 0 )
				{
					if( long_value_data_definition != NULL )
					{
						if( long_value_data_definition->key_size != 4 )
						{
							long_value_data_definition = NULL;
						}
						else if( memory_compare(
							  long_value_data_definition->key,
							  data_definition->key,
							  4 ) != 0 )
						{
							long_value_data_definition = NULL;
						}
					}
					if( long_value_data_definition == NULL )
					{
						if( libesedb_page_tree_get_value_definition_by_key(
						     page_tree,
						     data_definition->key,
						     data_definition->key_size - 4,
						     &long_value_data_definition,
						     0,
						     error ) != 1 )
						{
							liberror_error_set(
							 error,
							 LIBERROR_ERROR_DOMAIN_RUNTIME,
							 LIBERROR_RUNTIME_ERROR_GET_FAILED,
							 "%s: unable to retrieve long value data definition.",
							 function );

							libesedb_data_definition_free(
							 (intptr_t *) data_definition,
							 NULL );

							return( -1 );
						}
						if( long_value_data_definition == NULL )
						{
							liberror_error_set(
							 error,
							 LIBERROR_ERROR_DOMAIN_RUNTIME,
							 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
							 "%s: missing long value data definition.",
							 function );

							libesedb_data_definition_free(
							 (intptr_t *) data_definition,
							 NULL );

							return( -1 );
						}
					}
					byte_stream_copy_to_uint32_big_endian(
					 &( data_definition->key[ 4 ] ),
					 long_value_data_segment_offset );

					if( libesedb_data_definition_read_long_value_segment(
					     long_value_data_definition,
					     long_value_data_segment_offset,
					     page_value_data,
					     page_value_size,
					     page->offset + page_value->offset + page_value_offset,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_IO,
						 LIBERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read page value: %d long value data definition.",
						 function,
						 page_value_index );

						libesedb_data_definition_free(
						 (intptr_t *) data_definition,
						 NULL );

						return( -1 );
					}
				}

			}
			else
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
				 "%s: unsupported long value page key size.",
				 function );

				libesedb_data_definition_free(
				 (intptr_t *) data_definition,
				 NULL );

				return( -1 );
			}
		}
		else
		{
			if( page_tree->template_table_definition != NULL )
			{
				template_table_column_catalog_definition_list = page_tree->template_table_definition->column_catalog_definition_list;
			}
			if( libesedb_data_definition_read_record(
			     data_definition,
			     page_tree->table_definition->column_catalog_definition_list,
			     template_table_column_catalog_definition_list,
			     page_tree->io_handle,
			     page_value_data,
			     page_value_size,
			     page->offset + page_value->offset + page_value_offset,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read page value: %d record data definition.",
				 function,
				 page_value_index );

				libesedb_data_definition_free(
				 (intptr_t *) data_definition,
				 NULL );

				return( -1 );
			}
			if( ( page_value->flags & LIBESEDB_PAGE_TAG_FLAG_IS_DEFUNCT ) == 0 )
			{
				if( libesedb_page_tree_values_append_value_definition(
				     page_tree_values,
				     data_definition,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append value data definition to page tree values.",
					 function );

					libesedb_data_definition_free(
					 (intptr_t *) data_definition,
					 NULL );

					return( -1 );
				}
				data_definition = NULL;
			}
			/* TODO are defunct data definition of any value recovering
			 */
		}
		if( data_definition != NULL )
		{
			if( libesedb_data_definition_free(
			     (intptr_t *) data_definition,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free data definition.",
				 function );

				return( -1 );
			}
			data_definition = NULL;
		}
	}
	return( 1 );
}

/* Reads a page
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_page(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     off64_t page_offset,
     uint32_t page_number,
     libfdata_tree_node_t *value_tree_node,
     libesedb_values_tree_value_t *values_tree_value,
     liberror_error_t **error )
{
	libesedb_page_t *page             = NULL;
	libesedb_page_value_t *page_value = NULL;
	static char *function             = "libesedb_page_tree_read_page";
	off64_t node_data_offset          = 0;
	uint32_t supported_flags          = 0;
	uint16_t number_of_page_values    = 0;
	uint16_t page_value_index         = 0;
	int sub_node_index                = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *page_key_data            = NULL;
	size_t page_key_size              = 0;
#endif

	if( page_tree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( page_tree->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - missing IO handle.",
		 function );

		return( -1 );
	}
	if( values_tree_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values tree value.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_at_offset(
	     page_tree->pages_vector,
	     file_io_handle,
	     page_offset,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 " at offset %" PRIi64 ".",
		 function,
		 page_number,
		 page_offset );

		return( -1 );
	}
	if( page == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page.",
		 function );

		return( -1 );
	}
	supported_flags = LIBESEDB_PAGE_FLAG_IS_ROOT
	                | LIBESEDB_PAGE_FLAG_IS_LEAF
	                | LIBESEDB_PAGE_FLAG_IS_PARENT
	                | LIBESEDB_PAGE_FLAG_IS_INDEX
	                | LIBESEDB_PAGE_FLAG_IS_LONG_VALUE
	                | LIBESEDB_PAGE_FLAG_0x0400
	                | LIBESEDB_PAGE_FLAG_0x0800
	                | LIBESEDB_PAGE_FLAG_IS_NEW_RECORD_FORMAT;

	if( ( page->flags & ~supported_flags ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported page flags: 0x%08" PRIx32 ".",
		 function,
		 page->flags );

		return( -1 );
	}
	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) != 0 )
	{
		/* TODO */
	}
	else
	{
		if( page->previous_page_number != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported previous page number: %" PRIu32 ".",
			 function,
			 page->previous_page_number );

			return( -1 );
		}
		if( page->next_page_number != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported next page number: %" PRIu32 ".",
			 function,
			 page->next_page_number );

			return( -1 );
		}
	}
	if( libesedb_page_get_number_of_values(
	     page,
	     &number_of_page_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of page values.",
		 function );

		return( -1 );
	}
	if( number_of_page_values == 0 )
	{
		return( 1 );
	}
	if( libesedb_page_get_value(
	     page,
	     page_value_index,
	     &page_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: %" PRIu16 ".",
		 function,
		 page_value_index );

		return( -1 );
	}
	if( page_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value: %" PRIu16 ".",
		 function,
		 page_value_index );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: page value: %03" PRIu16 " value:\n",
		 function,
		 page_value_index );
		libnotify_print_data(
		 page_value->data,
		 (size_t) page_value->size );

		libnotify_printf(
		 "%s: page value: %03" PRIu16 " page tag flags\t\t: 0x%02" PRIx8 "",
		 function,
		 page_value_index,
		 page_value->flags );
		libesedb_debug_print_page_tag_flags(
		 page_value->flags );
		libnotify_printf(
		 "\n" );
	}
#endif
	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_ROOT ) != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "\n" );
		}
#endif
		/* TODO */
	}
#if defined( HAVE_DEBUG_OUTPUT )
	else if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: page value: %03" PRIu16 " common page key\t\t: ",
		 function,
		 page_value_index );

		page_key_data = page_value->data;
		page_key_size = page_value->size;

		while( page_key_size > 0 )
		{
			libnotify_printf(
			 "%02" PRIx8 " ",
			 *page_key_data );

			page_key_data++;
			page_key_size--;
		}
		libnotify_printf(
		 "\n" );
		libnotify_printf(
		 "\n" );
	}
#endif
	for( page_value_index = 1;
	     page_value_index < number_of_page_values;
	     page_value_index++ )
	{
		if( libesedb_page_get_value(
		     page,
		     page_value_index,
		     &page_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page value: %" PRIu16 ".",
			 function,
			 page_value_index );

			return( -1 );
		}
		if( ( page_value->flags & LIBESEDB_PAGE_TAG_FLAG_IS_DEFUNCT ) == 0 )
		{
			node_data_offset = page_offset + page_value->offset;

			if( libfdata_tree_node_append_sub_node(
			     value_tree_node,
			     &sub_node_index,
			     node_data_offset,
			     (size64_t) page_value_index,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append page: %" PRIu32 " value: %" PRIu16 " as sub node.",
				 function,
				 page_number,
				 page_value_index );

				return( -1 );
			}
		}
		/* TODO are defunct data definition of any value recovering
		 */
	}
	return( 1 );
}

/* Reads a page value
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_page_value(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     off64_t page_offset,
     uint32_t page_number,
     uint16_t page_value_index,
     libfdata_tree_node_t *value_tree_node,
     libesedb_values_tree_value_t *values_tree_value,
     liberror_error_t **error )
{
	libesedb_page_value_t *header_page_value = NULL;
	libesedb_page_t *page                    = NULL;
	libesedb_page_value_t *page_value        = NULL;
	uint8_t *page_value_data                 = NULL;
	static char *function                    = "libesedb_page_tree_read_page_value";
	off64_t sub_nodes_offset                 = 0;
	uint32_t child_page_number               = 0;
	uint16_t common_key_size                 = 0;
	uint16_t local_key_size                  = 0;
	uint16_t number_of_page_values           = 0;
	uint16_t page_value_offset               = 0;
	uint16_t page_value_size                 = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *page_key_data                   = NULL;
	size_t page_key_size                     = 0;
#endif

	if( page_tree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( page_tree->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - missing IO handle.",
		 function );

		return( -1 );
	}
	if( values_tree_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree values.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_at_offset(
	     page_tree->pages_vector,
	     file_io_handle,
	     page_offset,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 " at offset: %" PRIi64 ".",
		 function,
		 page_number,
		 page_offset );

		return( -1 );
	}
	if( page == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page.",
		 function );

		return( -1 );
	}
	if( libesedb_page_get_number_of_values(
	     page,
	     &number_of_page_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of page values.",
		 function );

		return( -1 );
	}
	if( number_of_page_values == 0 )
	{
		return( 1 );
	}
	if( libesedb_page_get_value(
	     page,
	     0,
	     &header_page_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: 0.",
		 function );

		return( -1 );
	}
	if( header_page_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value: 0.",
		 function );

		return( -1 );
	}
	if( libesedb_page_get_value(
	     page,
	     page_value_index,
	     &page_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: %" PRIu16 ".",
		 function,
		 page_value_index );

		return( -1 );
	}
	if( page_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value: %" PRIu16 ".",
		 function,
		 page_value_index );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: page value: %03" PRIu16 " data:\n",
		 function,
		 page_value_index );
		libnotify_print_data(
		 page_value->data,
		 (size_t) page_value->size );

		libnotify_printf(
		 "%s: page value: %03" PRIu16 " page tag flags\t\t: 0x%02" PRIx8 "",
		 function,
		 page_value_index,
		 page_value->flags );
		libesedb_debug_print_page_tag_flags(
		 page_value->flags );

		libnotify_printf(
		 "\n" );
	}
#endif
	page_value_data = page_value->data;
	page_value_size = page_value->size;

	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_ROOT ) != 0 )
	{
		/* TODO */
	}
	else if( ( page_value->flags & LIBESEDB_PAGE_TAG_FLAG_HAS_COMMON_KEY_SIZE ) != 0 )
	{
		byte_stream_copy_to_uint16_little_endian(
		 page_value_data,
		 common_key_size );

		page_value_data   += 2;
		page_value_offset += 2;
		page_value_size   -= 2;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: page value: %03" PRIu16 " common key size\t\t: %" PRIu16 "\n",
			 function,
			 page_value_index,
			 common_key_size );
		}
#endif
		if( common_key_size > header_page_value->size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: common key size exceeds header page value size.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: page value: %03" PRIu16 " common key value\t\t: ",
			 function,
			 page_value_index );

			page_key_data = header_page_value->data;
			page_key_size = common_key_size;

			while( page_key_size > 0 )
			{
				if( libnotify_verbose != 0 )
				{
					libnotify_printf(
					 "%02" PRIx8 " ",
					 *page_key_data );
				}
				page_key_data++;
				page_key_size--;
			}
			libnotify_printf(
			 "\n" );
		}
#endif
		if( libesedb_values_tree_value_set_key_common(
		     values_tree_value,
		     header_page_value->data,
		     common_key_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set common key in page tree values.",
			 function );

			return( -1 );
		}
	}
	byte_stream_copy_to_uint16_little_endian(
	 page_value_data,
	 local_key_size );

	page_value_data   += 2;
	page_value_offset += 2;
	page_value_size   -= 2;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: page value: %03" PRIu16 " local key size\t\t: %" PRIu16 "\n",
		 function,
		 page_value_index,
		 local_key_size );
	}
#endif
	if( local_key_size > page_value_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: local key size exceeds page value size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: page value: %03" PRIu16 " local key value\t\t: ",
		 function,
		 page_value_index );

		page_key_data = page_value_data;
		page_key_size = local_key_size;

		while( page_key_size > 0 )
		{
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%02" PRIx8 " ",
				 *page_key_data );
			}
			page_key_data++;
			page_key_size--;
		}
		libnotify_printf(
		 "\n" );
	}
#endif
	if( libesedb_values_tree_value_set_key_local(
	     values_tree_value,
	     page_value_data,
	     local_key_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set local key in page tree values.",
		 function );

		return( -1 );
	}
	page_value_data   += local_key_size;
	page_value_offset += local_key_size;
	page_value_size   -= local_key_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: page value: %03" PRIu16 " key value\t\t\t: ",
		 function,
		 page_value_index );

		page_key_data = values_tree_value->key;
		page_key_size = values_tree_value->key_size;

		while( page_key_size > 0 )
		{
			libnotify_printf(
			 "%02" PRIx8 " ",
			 *page_key_data );

			page_key_data++;
			page_key_size--;
		}
		libnotify_printf(
		 "\n" );
	}
#endif
	values_tree_value->data_offset = page_value_offset;
	values_tree_value->data_size   = page_value_size;

	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) != 0 )
	{
		if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_INDEX ) != 0 )
		{
			values_tree_value->type = LIBESEDB_VALUES_TREE_VALUE_TYPE_INDEX;
		}
		else if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_LONG_VALUE ) != 0 )
		{
			values_tree_value->type = LIBESEDB_VALUES_TREE_VALUE_TYPE_LONG_VALUE;
		}
		else
		{
			values_tree_value->type = LIBESEDB_VALUES_TREE_VALUE_TYPE_RECORD;
		}
		if( libfdata_tree_node_make_leaf(
		     value_tree_node,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to make value tree node a leaf.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "\n" );
		}
#endif
	}
	else
	{
		byte_stream_copy_to_uint32_little_endian(
		 page_value_data,
		 child_page_number );

		page_value_data += 4;
		page_value_size -= 4;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: page value: %03" PRIu16 " child page number\t\t: %" PRIu32 "",
			 function,
			 page_value_index,
			 child_page_number );

			if( child_page_number == 0 )
			{
				libnotify_printf(
				 " (invalid page number)\n" );
			}
			else if( child_page_number > page_tree->io_handle->last_page_number )
			{
				libnotify_printf(
				 " (exceeds last page number: %" PRIu32 ")\n",
				 page_tree->io_handle->last_page_number );
			}
			libnotify_printf(
			 "\n" );
			libnotify_printf(
			 "\n" );
		}
#endif
		if( ( child_page_number > 0 )
		 && ( child_page_number <= page_tree->io_handle->last_page_number ) )
		{
			sub_nodes_offset  = child_page_number - 1;
			sub_nodes_offset *= page_tree->io_handle->page_size;

			if( libfdata_tree_node_set_sub_nodes_range(
			     value_tree_node,
			     sub_nodes_offset,
			     0,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set child page number: %" PRIu32 " as sub nodes range.",
				 function,
				 child_page_number );

				return( -1 );
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( ( libnotify_verbose != 0 )
		 && ( page_value_size > 0 ) )
		{
			libnotify_printf(
			 "%s: page value: %03" PRIu16 " trailing data:\n",
			 function,
			 page_value_index );
			libnotify_print_data(
			 page_value_data,
			 page_value_size );
		}
#endif
		values_tree_value->type = LIBESEDB_VALUES_TREE_VALUE_TYPE_NODE;
	}
	return( 1 );
}

/* Reads a page tree node values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_node_values(
     intptr_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_tree_node_t *node,
     off64_t node_data_offset,
     size64_t node_data_size,
     uint8_t read_flags,
     liberror_error_t **error )
{
	libesedb_page_tree_t *page_tree                 = NULL;
	libesedb_values_tree_value_t *values_tree_value = NULL;
	static char *function                           = "libesedb_page_tree_read_node_values";
	uint64_t page_number                            = 0;
	int result                                      = 0;

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree IO handle.",
		 function );

		return( -1 );
	}
	page_tree = (libesedb_page_tree_t *) io_handle;

	if( page_tree->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - missing IO handle.",
		 function );

		return( -1 );
	}
	if( page_tree->io_handle->page_size == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - invalid IO handle - missing page size.",
		 function );

		return( -1 );
	}
	if( node_data_offset < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid node data offset value less than zero.",
		 function );

		return( -1 );
	}
	if( node_data_size > (off64_t) UINT16_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid node data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	page_number = (uint64_t)( ( node_data_offset / page_tree->io_handle->page_size ) + 1 );

	if( page_number > (uint64_t) UINT32_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid page number value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libesedb_values_tree_value_initialize(
	     &values_tree_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create values tree value.",
		 function );

		return( -1 );
	}
	values_tree_value->page_offset      = node_data_offset;
	values_tree_value->page_number      = (uint32_t) page_number;
	values_tree_value->page_value_index = (uint16_t) node_data_size;

	result = libfdata_tree_node_is_root(
	          node,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if node is the root.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		/* The values tree root node is virtual
		 */
		if( libfdata_tree_node_set_sub_nodes_range(
		     node,
		     node_data_offset,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set page: %" PRIu64 " as sub node range.",
			 function,
			 page_number );

			return( -1 );
		}
		values_tree_value->type = LIBESEDB_VALUES_TREE_VALUE_TYPE_NODE;
	}
	else
	{
		/* node_data_size contains the page value index
		 */
		if( libesedb_page_tree_read_page_value(
		     page_tree,
		     file_io_handle,
		     node_data_offset,
		     (uint32_t) page_number,
		     (uint16_t) node_data_size,
		     node,
		     values_tree_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read page: %" PRIu64 " value: %" PRIu64 ".",
			 function,
			 page_number,
			 node_data_size );

			libesedb_values_tree_value_free(
			 (intptr_t *) values_tree_value,
			NULL );

			return( -1 );
		}
	}
	if( libfdata_tree_node_set_node_value(
	     node,
	     (intptr_t *) values_tree_value,
	     &libesedb_values_tree_value_free,
	     LIBFDATA_TREE_NODE_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set values tree value as node value.",
		 function );

		libesedb_values_tree_value_free(
		 (intptr_t *) values_tree_value,
		NULL );

		return( -1 );
	}
	return( 1 );
}

/* Reads a page tree sub nodes
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_sub_nodes(
     intptr_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_tree_node_t *node,
     off64_t sub_nodes_offset,
     size64_t sub_nodes_size,
     uint8_t read_flags,
     liberror_error_t **error )
{
	libesedb_page_tree_t *page_tree                 = NULL;
	libesedb_values_tree_value_t *values_tree_value = NULL;
	static char *function                           = "libesedb_page_tree_read_sub_nodes";
	uint64_t page_number                            = 0;

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree IO handle.",
		 function );

		return( -1 );
	}
	page_tree = (libesedb_page_tree_t *) io_handle;

	if( page_tree->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - missing IO handle.",
		 function );

		return( -1 );
	}
	if( page_tree->io_handle->page_size == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - invalid IO handle - missing page size.",
		 function );

		return( -1 );
	}
	if( sub_nodes_offset < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid sub nodes offset value less than zero.",
		 function );

		return( -1 );
	}
	page_number = (uint64_t) ( ( sub_nodes_offset / page_tree->io_handle->page_size ) + 1 );

	if( page_number > (uint64_t) UINT32_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid page number value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libfdata_tree_node_get_node_value(
	     node,
	     file_io_handle,
	     (intptr_t **) &values_tree_value,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve node value.",
		 function );

		return( -1 );
	}
	if( values_tree_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing values tree value.",
		 function );

		return( -1 );
	}
	if( libesedb_page_tree_read_page(
	     page_tree,
	     file_io_handle,
	     sub_nodes_offset,
	     (uint32_t) page_number,
	     node,
	     values_tree_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read page: %" PRIu64 " at offset: %" PRIi64 ".",
		 function,
		 page_number,
		 sub_nodes_offset );

		return( -1 );
	}
	return( 1 );
}

