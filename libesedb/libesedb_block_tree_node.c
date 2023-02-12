/*
 * Block tree node functions
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

#include "libesedb_block_tree_node.h"
#include "libesedb_definitions.h"
#include "libesedb_libcerror.h"

/* Creates a block tree node
 * Make sure the value block_tree_node is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_block_tree_node_initialize(
     libesedb_block_tree_node_t **block_tree_node,
     off64_t offset,
     size64_t size,
     size64_t leaf_value_size,
     libcerror_error_t **error )
{
	static char *function  = "libesedb_block_tree_node_initialize";
	size64_t sub_node_size = 0;

	if( block_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block tree node.",
		 function );

		return( -1 );
	}
	if( *block_tree_node != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid block tree node value already set.",
		 function );

		return( -1 );
	}
	if( offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( size == 0 )
	 || ( size > (size64_t) ( INT64_MAX - offset ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid size value out of bounds.",
		 function );

		return( -1 );
	}
	if( leaf_value_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid leaf value size value out of bounds.",
		 function );

		return( -1 );
	}
	*block_tree_node = memory_allocate_structure(
	                    libesedb_block_tree_node_t );

	if( *block_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create block tree node.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *block_tree_node,
	     0,
	     sizeof( libesedb_block_tree_node_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear block tree node.",
		 function );

		memory_free(
		 *block_tree_node );

		*block_tree_node = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *block_tree_node )->sub_nodes_array ),
	     LIBESEDB_BLOCK_TREE_NUMBER_OF_SUB_NODES,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub nodes array.",
		 function );

		goto on_error;
	}
	sub_node_size = leaf_value_size;

	while( ( size / sub_node_size ) > LIBESEDB_BLOCK_TREE_NUMBER_OF_SUB_NODES )
	{
		sub_node_size *= LIBESEDB_BLOCK_TREE_NUMBER_OF_SUB_NODES;
	}
	( *block_tree_node )->start_offset  = offset;
	( *block_tree_node )->end_offset    = offset + size;
	( *block_tree_node )->size          = size;
	( *block_tree_node )->sub_node_size = sub_node_size;

	if( sub_node_size == leaf_value_size )
	{
		( *block_tree_node )->is_leaf_node = 1;
	}
	return( 1 );

on_error:
	if( *block_tree_node != NULL )
	{
		memory_free(
		 *block_tree_node );

		*block_tree_node = NULL;
	}
	return( -1 );
}

/* Frees a block tree node
 * Returns 1 if successful or -1 on error
 */
int libesedb_block_tree_node_free(
     libesedb_block_tree_node_t **block_tree_node,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     libcerror_error_t **error )
{
	libesedb_block_tree_node_t *sub_block_tree_node = NULL;
	static char *function                           = "libesedb_block_tree_node_free";
	int result                                      = 1;
	int sub_node_index                              = 0;

	if( block_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block tree node.",
		 function );

		return( -1 );
	}
	if( *block_tree_node != NULL )
	{
		if( ( *block_tree_node )->is_leaf_node != 0 )
		{
			if( libcdata_array_free(
			     &( ( *block_tree_node )->sub_nodes_array ),
			     value_free_function,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free sub nodes array.",
				 function );

				result = -1;
			}
		}
		else
		{
			for( sub_node_index = 0;
			     sub_node_index < LIBESEDB_BLOCK_TREE_NUMBER_OF_SUB_NODES;
			     sub_node_index++ )
			{
				if( libcdata_array_get_entry_by_index(
				     ( *block_tree_node )->sub_nodes_array,
				     sub_node_index,
				     (intptr_t **) &sub_block_tree_node,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve sub node: %d.",
					 function,
					 sub_node_index );

					result = -1;
				}
				if( libesedb_block_tree_node_free(
				     &sub_block_tree_node,
				     value_free_function,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free sub node: %d.",
					 function,
					 sub_node_index );

					result = -1;
				}
			}
			if( libcdata_array_free(
			     &( ( *block_tree_node )->sub_nodes_array ),
			     NULL,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free sub nodes array.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *block_tree_node );

		*block_tree_node = NULL;
	}
	return( result );
}

/* Retrieves the sub node for a specific offset
 * Returns 1 if successful or -1 on error
 */
int libesedb_block_tree_node_get_sub_node_at_offset(
     libesedb_block_tree_node_t *block_tree_node,
     off64_t offset,
     libesedb_block_tree_node_t **sub_block_tree_node,
     libcerror_error_t **error )
{
	static char *function  = "libesedb_block_tree_node_get_sub_node_at_offset";
	off64_t sub_node_index = 0;

	if( block_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block tree node.",
		 function );

		return( -1 );
	}
	if( block_tree_node->is_leaf_node != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block tree node - not a branch node.",
		 function );

		return( -1 );
	}
	if( ( offset < block_tree_node->start_offset )
	 || ( offset >= block_tree_node->end_offset ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
	sub_node_index = ( offset - block_tree_node->start_offset ) / block_tree_node->sub_node_size;

	if( ( sub_node_index < 0 )
	 || ( sub_node_index > (off64_t) INT_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid sub node index value out of bounds.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     block_tree_node->sub_nodes_array,
	     (int) sub_node_index,
	     (intptr_t **) sub_block_tree_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub node: %" PRIi64 ".",
		 function,
		 sub_node_index );

		return( -1 );
	}
	return( 1 );
}

/* Sets the sub node for a specific offset
 * Returns 1 if successful or -1 on error
 */
int libesedb_block_tree_node_set_sub_node_at_offset(
     libesedb_block_tree_node_t *block_tree_node,
     off64_t offset,
     libesedb_block_tree_node_t *sub_block_tree_node,
     libcerror_error_t **error )
{
	static char *function  = "libesedb_block_tree_node_set_sub_node_at_offset";
	off64_t sub_node_index = 0;

	if( block_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block tree node.",
		 function );

		return( -1 );
	}
	if( block_tree_node->is_leaf_node != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block tree node - not a branch node.",
		 function );

		return( -1 );
	}
	if( ( offset < block_tree_node->start_offset )
	 || ( offset >= block_tree_node->end_offset ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
	sub_node_index = ( offset - block_tree_node->start_offset ) / block_tree_node->sub_node_size;

	if( ( sub_node_index < 0 )
	 || ( sub_node_index > (off64_t) INT_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid sub node index value out of bounds.",
		 function );

		return( -1 );
	}
	if( libcdata_array_set_entry_by_index(
	     block_tree_node->sub_nodes_array,
	     (int) sub_node_index,
	     (intptr_t *) sub_block_tree_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set sub node: %" PRIi64 ".",
		 function,
		 sub_node_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the leaf value for a specific offset
 * Returns 1 if successful or -1 on error
 */
int libesedb_block_tree_node_get_leaf_value_at_offset(
     libesedb_block_tree_node_t *block_tree_node,
     off64_t offset,
     libesedb_block_descriptor_t **block_descriptor,
     off64_t *block_offset,
     libcerror_error_t **error )
{
	static char *function     = "libesedb_block_tree_node_get_leaf_value_at_offset";
	off64_t leaf_value_index  = 0;
	off64_t safe_block_offset = 0;

	if( block_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block tree node.",
		 function );

		return( -1 );
	}
	if( block_tree_node->is_leaf_node == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block tree node - not a leaf node.",
		 function );

		return( -1 );
	}
	if( ( offset < block_tree_node->start_offset )
	 || ( offset >= block_tree_node->end_offset ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( block_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block offset.",
		 function );

		return( -1 );
	}
	safe_block_offset = offset - block_tree_node->start_offset;

	leaf_value_index = safe_block_offset / block_tree_node->sub_node_size;

	if( ( leaf_value_index < 0 )
	 || ( leaf_value_index > (off64_t) INT_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid leaf value index value out of bounds.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     block_tree_node->sub_nodes_array,
	     (int) leaf_value_index,
	     (intptr_t **) block_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve leaf value: %" PRIi64 ".",
		 function,
		 leaf_value_index );

		return( -1 );
	}
	*block_offset = safe_block_offset - ( leaf_value_index * block_tree_node->sub_node_size );

	return( 1 );
}

/* Sets the leaf value for a specific offset
 * Returns 1 if successful or -1 on error
 */
int libesedb_block_tree_node_set_leaf_value_at_offset(
     libesedb_block_tree_node_t *block_tree_node,
     off64_t offset,
     libesedb_block_descriptor_t *block_descriptor,
     libcerror_error_t **error )
{
	static char *function    = "libesedb_block_tree_node_set_leaf_value_at_offset";
	off64_t leaf_value_index = 0;

	if( block_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block tree node.",
		 function );

		return( -1 );
	}
	if( block_tree_node->is_leaf_node == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block tree node - not a leaf node.",
		 function );

		return( -1 );
	}
	if( ( offset < block_tree_node->start_offset )
	 || ( offset >= block_tree_node->end_offset ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
	leaf_value_index = ( offset - block_tree_node->start_offset ) / block_tree_node->sub_node_size;

	if( ( leaf_value_index < 0 )
	 || ( leaf_value_index > (off64_t) INT_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid leaf value index value out of bounds.",
		 function );

		return( -1 );
	}
	if( libcdata_array_set_entry_by_index(
	     block_tree_node->sub_nodes_array,
	     (int) leaf_value_index,
	     (intptr_t *) block_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set leaf value: %" PRIi64 ".",
		 function,
		 leaf_value_index );

		return( -1 );
	}
	return( 1 );
}

