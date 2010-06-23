/*
 * Page tree values functions
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include "libesedb_array_type.h"
#include "libesedb_definitions.h"
#include "libesedb_data_definition.h"
#include "libesedb_page_tree_values.h"

#include "esedb_page_values.h"

/* Creates page tree values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_values_initialize(
     libesedb_page_tree_values_t **page_tree_values,
     liberror_error_t **error )
{
	static char *function = "libesedb_page_tree_values_initialize";

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
	if( *page_tree_values == NULL )
	{
		*page_tree_values = (libesedb_page_tree_values_t *) memory_allocate(
		                                                     sizeof( libesedb_page_tree_values_t ) );

		if( *page_tree_values == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create page tree values.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *page_tree_values,
		     0,
		     sizeof( libesedb_page_tree_values_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear page tree values.",
			 function );

			memory_free(
			 *page_tree_values );

			*page_tree_values = NULL;

			return( -1 );
		}
		if( libesedb_array_initialize(
		     &( ( *page_tree_values )->value_definition_array ),
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value definition array.",
			 function );

			memory_free(
			 *page_tree_values );

			*page_tree_values = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees the page tree values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_values_free(
     intptr_t *page_tree_values,
     liberror_error_t **error )
{
	static char *function = "libesedb_page_tree_values_free";
	int result            = 1;

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
	if( libesedb_array_free(
	     &( ( ( libesedb_page_tree_values_t *) page_tree_values )->value_definition_array ),
	     &libesedb_data_definition_free,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free value definition array.",
		 function );

		result = -1;
	}
	if( ( ( libesedb_page_tree_values_t *) page_tree_values )->common_key != NULL )
	{
		memory_free(
		 ( (libesedb_page_tree_values_t *) page_tree_values )->common_key );
	}
	if( ( ( libesedb_page_tree_values_t *) page_tree_values )->key != NULL )
	{
		memory_free(
		 ( (libesedb_page_tree_values_t *) page_tree_values )->key );
	}
	memory_free(
	 page_tree_values );

	return( result );
}

/* Sets the common key in the page tree values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_values_set_common_key(
     libesedb_page_tree_values_t *page_tree_values,
     uint8_t *common_key,
     size_t common_key_size,
     liberror_error_t **error )
{
	static char *function = "libesedb_page_tree_values_set_common_key";

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
	if( page_tree_values->common_key != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid page tree values - common key already set.",
		 function );

		return( -1 );
	}
	if( common_key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid common key.",
		 function );

		return( -1 );
	}
	if( common_key_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid common key size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( common_key_size > 0 )
	{
		page_tree_values->common_key = (uint8_t *) memory_allocate(
		                                            sizeof( uint8_t ) * common_key_size );

		if( page_tree_values->common_key == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create common key.",
			 function );

			return( -1 );
		}
		if( memory_copy(
		     page_tree_values->common_key,
		     common_key,
		     sizeof( uint8_t ) * common_key_size ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy common key.",
			 function );

			return( -1 );
		}
		page_tree_values->common_key_size = common_key_size;
	}
	return( 1 );
}

/* Sets the common part of the key in the page tree values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_values_set_key_common(
     libesedb_page_tree_values_t *page_tree_values,
     uint8_t *common_key,
     size_t common_key_size,
     liberror_error_t **error )
{
	static char *function = "libesedb_page_tree_values_set_key_common";

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
	if( page_tree_values->key != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid page tree values - key already set.",
		 function );

		return( -1 );
	}
	if( common_key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid common key.",
		 function );

		return( -1 );
	}
	if( common_key_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid common key size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( common_key_size > 0 )
	{
		page_tree_values->key = (uint8_t *) memory_allocate(
						    sizeof( uint8_t ) * common_key_size );

		if( page_tree_values->key == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create key.",
			 function );

			return( -1 );
		}
		if( memory_copy(
		     page_tree_values->key,
		     common_key,
		     sizeof( uint8_t ) * common_key_size ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy common key.",
			 function );

			return( -1 );
		}
		page_tree_values->key_size = common_key_size;
	}
	return( 1 );
}

/* Sets the local part of the key in the page tree values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_values_set_key_local(
     libesedb_page_tree_values_t *page_tree_values,
     uint8_t *local_key,
     size_t local_key_size,
     liberror_error_t **error )
{
	void *reallocation    = NULL;
	static char *function = "libesedb_page_tree_values_set_key_local";

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
	if( local_key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local key.",
		 function );

		return( -1 );
	}
	if( local_key_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid local key size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( local_key_size > 0 )
	{
		reallocation = memory_reallocate(
		                page_tree_values->key,
		                sizeof( uint8_t ) * ( page_tree_values->key_size + local_key_size ) );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize key.",
			 function );

			return( -1 );
		}
		page_tree_values->key = (uint8_t *) reallocation;

		if( memory_copy(
		     &( page_tree_values->key[ page_tree_values->key_size ] ),
		     local_key,
		     sizeof( uint8_t ) * local_key_size ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy local key.",
			 function );

			return( -1 );
		}
		page_tree_values->key_size += local_key_size;
	}
	return( 1 );
}

/* Retrieves a value data definition from the page tree values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_values_get_value_definition(
     libesedb_page_tree_values_t *page_tree_values,
     int value_definition_index,
     libesedb_data_definition_t **data_definition,
     liberror_error_t **error )
{
	static char *function = "libesedb_page_tree_values_get_value_definition";

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
	if( libesedb_array_get_entry(
	     page_tree_values->value_definition_array,
	     value_definition_index,
	     (intptr_t **) data_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data definition: %d from value definition array.",
		 function,
		 value_definition_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a value data definition from the page tree values for a specific key
 * Returns 1 if successful, 0 if no value definition was found or -1 on error
 */
int libesedb_page_tree_values_get_value_definition_by_key(
     libesedb_page_tree_values_t *page_tree_values,
     uint8_t *key,
     size_t key_size,
     libesedb_data_definition_t **data_definition,
     uint8_t flags,
     liberror_error_t **error )
{
	static char *function            = "libesedb_page_tree_values_get_value_definition_by_key";
	size_t data_definition_key_index = 0;
	size_t key_index                 = 0;
	int value_definition_index       = 0;
	int number_of_entries            = 0;
	int8_t compare                   = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *page_key_data           = NULL;
	size_t page_key_size             = 0;
#endif

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
	if( data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data definition.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_number_of_entries(
	     page_tree_values->value_definition_array,
	     &number_of_entries,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from value definition array.",
		 function );

		return( -1 );
	}
	for( value_definition_index = 0;
	     value_definition_index < number_of_entries;
	     value_definition_index++ )
	{
		if( libesedb_array_get_entry(
		     page_tree_values->value_definition_array,
		     value_definition_index,
		     (intptr_t **) data_definition,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data definition: %d from value definition array.",
			 function,
			 value_definition_index );

			return( -1 );
		}
		if( *data_definition == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing data definition: %d.",
			 function,
			 value_definition_index );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			page_key_data = ( *data_definition )->key;
			page_key_size = ( *data_definition )->key_size;

			libnotify_printf(
			 "%s: value data definition: %d key\t: ",
			 function,
			 value_definition_index );

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
		if( key_size == ( *data_definition )->key_size )
		{
			data_definition_key_index = 0;

			for( key_index = 0;
			     key_index < key_size;
			     key_index++ )
			{
				if( ( flags & LIBESEDB_PAGE_KEY_FLAG_REVERSED_KEY ) != 0 )
				{
					compare = (int8_t) key[ key_size - ( key_index + 1 ) ] - (int8_t) ( *data_definition )->key[ data_definition_key_index ];
				}
				else
				{
					compare = (int8_t) key[ key_index ] - (int8_t) ( *data_definition )->key[ data_definition_key_index ];
				}
				if( compare != 0 )
				{
					break;
				}
				data_definition_key_index++;
			}
			if( compare == 0 )
			{
				return( 1 );
			}
		}
	}
	*data_definition = NULL;

	return( 0 );
}

/* Appends a value data definition to the page tree values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_values_append_value_definition(
     libesedb_page_tree_values_t *page_tree_values,
     libesedb_data_definition_t *data_definition,
     liberror_error_t **error )
{
	static char *function      = "libesedb_page_tree_values_append_value_definition";
	int value_definition_index = 0;

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
	if( libesedb_array_append_entry(
	     page_tree_values->value_definition_array,
	     &value_definition_index,
	     (intptr_t *) data_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append data definition: %d to value definition array.",
		 function,
		 page_tree_values->number_of_value_definitions - 1 );

		return( -1 );
	}
	page_tree_values->number_of_value_definitions += 1;

	return( 1 );
}

