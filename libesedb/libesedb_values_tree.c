/*
 * Values tree functions
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
#include <types.h>

#include <liberror.h>
#include <libnotify.h>

#include "libesedb_definitions.h"
#include "libesedb_libbfio.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_values_tree.h"
#include "libesedb_values_tree_value.h"

/* Retrieves the values tree leaf node for the specific key
 * Returns 1 if successful, 0 if no value was found or -1 on error
 */
int libesedb_values_tree_get_leaf_node_by_key(
     libfdata_tree_t *values_tree,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *values_cache,
     uint8_t *key,
     size_t key_size,
     libfdata_tree_node_t **values_tree_leaf_node,
     uint8_t flags,
     liberror_error_t **error )
{
	libfdata_tree_node_t *values_tree_root_node = NULL;
	static char *function                       = "libesedb_values_tree_get_leaf_node_by_key";
	int result                                  = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *page_key_data                      = NULL;
	size_t page_key_size                        = 0;
#endif

	if( values_tree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values tree.",
		 function );

		return( -1 );
	}
	if( values_tree_leaf_node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values tree leaf node.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
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
	if( libnotify_verbose != 0 )
	{
		page_key_data = key;
		page_key_size = key_size;

		libnotify_printf(
		 "%s: requested key\t: ",
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
	if( libfdata_tree_get_root_node(
	     values_tree,
	     &values_tree_root_node,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root node from values tree.",
		 function );

		return( -1 );
	}
	*values_tree_leaf_node = NULL;

	result = libesedb_values_tree_node_get_leaf_node_by_key(
		  values_tree_root_node,
		  file_io_handle,
		  values_cache,
		  key,
		  key_size,
		  values_tree_leaf_node,
		  flags,
		  error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve leaf node by key in root node.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the values tree leaf node for the specific key
 * Returns 1 if successful, 0 if no value was found or -1 on error
 */
int libesedb_values_tree_node_get_leaf_node_by_key(
     libfdata_tree_node_t *values_tree_node,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *values_cache,
     uint8_t *key,
     size_t key_size,
     libfdata_tree_node_t **values_tree_leaf_node,
     uint8_t flags,
     liberror_error_t **error )
{
	libesedb_values_tree_value_t *values_tree_sub_node_value = NULL;
	libfdata_tree_node_t *values_tree_sub_node               = NULL;
	static char *function                                    = "libesedb_values_tree_node_get_leaf_node_by_key";
	size_t compare_size                                      = 0;
	size_t value_key_index                                   = 0;
	size_t key_index                                         = 0;
	int16_t compare                                          = 0;
	int is_leaf_node                                         = 0;
	int number_of_sub_nodes                                  = 0;
	int result                                               = 0;
	int sub_node_index                                       = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *page_key_data                                   = NULL;
	size_t page_key_size                                     = 0;
#endif

	if( values_tree_node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values tree node.",
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
	if( libfdata_tree_node_get_number_of_sub_nodes(
	     values_tree_node,
	     file_io_handle,
	     values_cache,
	     &number_of_sub_nodes,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub nodes from values tree node.",
		 function );

		return( -1 );
	}
	for( sub_node_index = 0;
	     sub_node_index < number_of_sub_nodes;
	     sub_node_index++ )
	{
		if( libfdata_tree_node_get_sub_node_by_index(
		     values_tree_node,
		     file_io_handle,
		     values_cache,
		     sub_node_index,
		     &values_tree_sub_node,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub node: %d from values tree node.",
			 function,
			 sub_node_index );

			return( -1 );
		}
		if( libfdata_tree_node_get_node_value(
		     values_tree_sub_node,
		     file_io_handle,
		     values_cache,
		     (intptr_t **) &values_tree_sub_node_value,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve values tree sub node value: %d.",
			 function,
			 sub_node_index );

			return( -1 );
		}
		if( values_tree_sub_node_value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing values tree sub node value: %d.",
			 function,
			 sub_node_index );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			page_key_data = values_tree_sub_node_value->key;
			page_key_size = values_tree_sub_node_value->key_size;

			libnotify_printf(
			 "%s: values tree sub node value: %d key\t: ",
			 function,
			 sub_node_index );

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
		/* Check if the values tree sub node value key is empty, therefore has no upper bound
		 */
		if( values_tree_sub_node_value->key_size == 0 )
		{
			compare = -1;
		}
		else
		{
			if( key_size <= values_tree_sub_node_value->key_size )
			{
				compare_size = key_size;
			}
			else
			{
				compare_size = values_tree_sub_node_value->key_size;
			}
			value_key_index = 0;

			for( key_index = 0;
			     key_index < compare_size;
			     key_index++ )
			{
				if( ( flags & LIBESEDB_PAGE_KEY_FLAG_REVERSED_KEY ) != 0 )
				{
					compare = (int16_t) key[ key_size - ( key_index + 1 ) ] - (int16_t) values_tree_sub_node_value->key[ value_key_index ];
				}
				else
				{
					compare = (int16_t) key[ key_index ] - (int16_t) values_tree_sub_node_value->key[ value_key_index ];
				}
				if( compare != 0 )
				{
					break;
				}
				value_key_index++;
			}
		}
		is_leaf_node = libfdata_tree_node_is_leaf(
				values_tree_sub_node,
				file_io_handle,
				values_cache,
				0,
				error );

		if( is_leaf_node == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if values tree sub node: %d is a leaf node.",
			 function,
			 sub_node_index );

			return( -1 );
		}
		else if( is_leaf_node != 0 )
		{
			if( ( compare == 0 )
			 && ( values_tree_sub_node_value->key_size == key_size ) )
			{
				if( values_tree_leaf_node == NULL )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
					 "%s: invalid values tree leaf_node.",
					 function );

					return( -1 );
				}
				if( *values_tree_leaf_node != NULL )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
					 "%s: values tree leaf node already set.",
					 function );

					return( -1 );
				}
				*values_tree_leaf_node = values_tree_sub_node;

				result = 1;

				break;
			}
			/* If the values tree sub node key is greater than the requested key
			 * (compare will be negative) stop looking in this branch
			 */
			else if( compare < 0 )
			{
				break;
			}
		}
		else
		{
			/* If the key exactly matches the node values tree value key,
			 * the leaf values tree value is either in the current or the next
			 * node values tree value
			 */
			if( compare <= 0 )
			{
				result = libesedb_values_tree_node_get_leaf_node_by_key(
					  values_tree_sub_node,
					  file_io_handle,
					  values_cache,
					  key,
					  key_size,
					  values_tree_leaf_node,
					  flags,
					  error );

				if( result == -1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve leaf node by key in values tree sub node: %d.",
					 function,
					 sub_node_index );

					return( -1 );
				}
				else if( result != 0 )
				{
					break;
				}
				/* If the values tree sub node key is greater than the requested key
				 * (compare will be negative) stop looking in this branch
				 */
				else if( compare < 0 )
				{
					break;
				}
			}
		}
	}
	return( result );
}

/* Retrieves the values tree value for the specific key
 * Returns 1 if successful, 0 if no value was found or -1 on error
 */
int libesedb_values_tree_get_value_by_key(
     libfdata_tree_t *values_tree,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *values_cache,
     uint8_t *key,
     size_t key_size,
     libesedb_values_tree_value_t **values_tree_value,
     uint8_t flags,
     liberror_error_t **error )
{
	libfdata_tree_node_t *values_tree_leaf_node = NULL;
	static char *function                       = "libesedb_values_tree_get_value_by_key";
	int result                                  = 0;

	result = libesedb_values_tree_get_leaf_node_by_key(
		  values_tree,
		  file_io_handle,
		  values_cache,
		  key,
		  key_size,
		  &values_tree_leaf_node,
		  flags,
		  error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve leaf node by key.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( values_tree_leaf_node == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: midding values tree leaf node.",
			 function );

			return( -1 );
		}
		if( libfdata_tree_node_get_node_value(
		     values_tree_leaf_node,
		     file_io_handle,
		     values_cache,
		     (intptr_t **) values_tree_value,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve values leaf sub node value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

