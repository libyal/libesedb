/*
 * Block tree functions
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
#include "libesedb_block_tree_node.h"
#include "libesedb_definitions.h"
#include "libesedb_libcdata.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_unused.h"

/* Creates a block tree
 * Make sure the value block_tree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_block_tree_initialize(
     libesedb_block_tree_t **block_tree,
     size64_t size,
     size64_t leaf_value_size,
     libcerror_error_t **error )
{
	static char *function = "libesedb_block_tree_initialize";

	if( block_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block tree.",
		 function );

		return( -1 );
	}
	if( *block_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid block tree value already set.",
		 function );

		return( -1 );
	}
	*block_tree = memory_allocate_structure(
	               libesedb_block_tree_t );

	if( *block_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create block tree.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *block_tree,
	     0,
	     sizeof( libesedb_block_tree_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear block tree.",
		 function );

		memory_free(
		 *block_tree );

		*block_tree = NULL;

		return( -1 );
	}
	if( libesedb_block_tree_node_initialize(
	     &( ( *block_tree )->root_node ),
	     0,
	     size,
	     leaf_value_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create root node.",
		 function );

		goto on_error;
	}
	( *block_tree )->leaf_value_size = leaf_value_size;

	return( 1 );

on_error:
	if( *block_tree != NULL )
	{
		memory_free(
		 *block_tree );

		*block_tree = NULL;
	}
	return( -1 );
}

/* Frees a block tree
 * Returns 1 if successful or -1 on error
 */
int libesedb_block_tree_free(
     libesedb_block_tree_t **block_tree,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     libcerror_error_t **error )
{
	static char *function = "libesedb_block_tree_free";
	int result            = 1;

	if( block_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block tree.",
		 function );

		return( -1 );
	}
	if( *block_tree != NULL )
	{
		if( ( *block_tree )->root_node != NULL )
		{
			if( libesedb_block_tree_node_free(
			     &( ( *block_tree )->root_node ),
			     value_free_function,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free root node.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *block_tree );

		*block_tree = NULL;
	}
	return( result );
}

/* Retrieves the block descriptor for a specific offset
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libesedb_block_tree_get_block_descriptor_by_offset(
     libesedb_block_tree_t *block_tree,
     off64_t offset,
     libesedb_block_descriptor_t **block_descriptor,
     off64_t *block_offset,
     libcerror_error_t **error )
{
	libesedb_block_descriptor_t *safe_block_descriptor = NULL;
	libesedb_block_tree_node_t *block_tree_node        = NULL;
	static char *function                              = "libesedb_block_tree_get_block_descriptor_by_offset";
	off64_t safe_block_offset                          = 0;

	if( block_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block tree.",
		 function );

		return( -1 );
	}
	if( block_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block descriptor.",
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
	block_tree_node = block_tree->root_node;

	while( block_tree_node->is_leaf_node == 0 )
	{
		if( libesedb_block_tree_node_get_sub_node_at_offset(
		     block_tree_node,
		     offset,
		     &block_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub node at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 offset,
			 offset );

			return( -1 );
		}
		if( block_tree_node == NULL )
		{
			return( 0 );
		}
	}
	if( libesedb_block_tree_node_get_leaf_value_at_offset(
	     block_tree_node,
	     offset,
	     &safe_block_descriptor,
	     &safe_block_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve leaf value at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 offset,
		 offset );

		return( -1 );
	}
	*block_descriptor = safe_block_descriptor;
	*block_offset     = safe_block_offset;

	if( safe_block_descriptor == NULL )
	{
		return( 0 );
	}
	return( 1 );
}

/* Inserts the block descriptor for a specific offset
 * Returns 1 if successful, 0 if already set or -1 on error
 */
int libesedb_block_tree_insert_block_descriptor_by_offset(
     libesedb_block_tree_t *block_tree,
     off64_t offset,
     libesedb_block_descriptor_t *block_descriptor,
     int *leaf_value_index,
     libesedb_block_tree_node_t **leaf_block_tree_node,
     libesedb_block_descriptor_t **existing_block_descriptor,
     libcerror_error_t **error )
{
	libesedb_block_descriptor_t *safe_block_descriptor = NULL;
	libesedb_block_tree_node_t *block_tree_node        = NULL;
	libesedb_block_tree_node_t *safe_block_tree_node   = NULL;
	libesedb_block_tree_node_t *sub_block_tree_node    = NULL;
	static char *function                              = "libesedb_block_tree_insert_block_descriptor_by_offset";
	off64_t block_offset                               = 0;
	off64_t sub_node_index                             = 0;
	off64_t sub_node_offset                            = 0;

	if( block_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block tree.",
		 function );

		return( -1 );
	}
	if( leaf_value_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid leaf value index.",
		 function );

		return( -1 );
	}
	if( leaf_block_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid leaf block tree node.",
		 function );

		return( -1 );
	}
	if( existing_block_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid existing block descriptor.",
		 function );

		return( -1 );
	}
	safe_block_tree_node = block_tree->root_node;

	while( safe_block_tree_node->is_leaf_node == 0 )
	{
		if( libesedb_block_tree_node_get_sub_node_at_offset(
		     safe_block_tree_node,
		     offset,
		     &block_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub node at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 offset,
			 offset );

			goto on_error;
		}
		if( block_tree_node == NULL )
		{
			sub_node_offset = offset - safe_block_tree_node->start_offset;

			sub_node_index  = sub_node_offset / safe_block_tree_node->sub_node_size;
			sub_node_offset = safe_block_tree_node->start_offset + ( sub_node_index * safe_block_tree_node->sub_node_size );

			if( libesedb_block_tree_node_initialize(
			     &sub_block_tree_node,
			     sub_node_offset,
			     safe_block_tree_node->sub_node_size,
			     block_tree->leaf_value_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create sub node.",
				 function );

				goto on_error;
			}
			if( libesedb_block_tree_node_set_sub_node_at_offset(
			     safe_block_tree_node,
			     offset,
			     sub_block_tree_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to set sub node at offset: %" PRIi64 " (0x%08" PRIx64 ").",
				 function,
				 offset,
				 offset );

				goto on_error;
			}
			block_tree_node     = sub_block_tree_node;
			sub_block_tree_node = NULL;
		}
		safe_block_tree_node = block_tree_node;
	}
	*leaf_block_tree_node = safe_block_tree_node;

	if( libesedb_block_tree_node_get_leaf_value_at_offset(
	     safe_block_tree_node,
	     offset,
	     &safe_block_descriptor,
	     &block_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve leaf value at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 offset,
		 offset );

		goto on_error;
	}
	if( safe_block_descriptor != NULL )
	{
		*existing_block_descriptor = safe_block_descriptor;

		return( 0 );
	}
	if( libesedb_block_tree_node_set_leaf_value_at_offset(
	     safe_block_tree_node,
	     offset,
	     block_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set leaf value at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 offset,
		 offset );

		goto on_error;
	}
	*leaf_value_index = (int) ( offset / block_tree->leaf_value_size );

	return( 1 );

on_error:
	if( sub_block_tree_node != NULL )
	{
		libesedb_block_tree_node_free(
		 &sub_block_tree_node,
		 NULL,
		 NULL );
	}
	return( -1 );
}

