/*
 * Tree type functions
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

#include "libesedb_tree_type.h"

/* Creates a tree node
 * Returns 1 if successful or -1 on error
 */
int libesedb_tree_node_initialize(
     libesedb_tree_node_t **node,
     liberror_error_t **error )
{
	static char *function = "libesedb_tree_node_initialize";

	if( node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( *node == NULL )
	{
		*node = (libesedb_tree_node_t *) memory_allocate(
		                                  sizeof( libesedb_tree_node_t ) );

		if( *node == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create node.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *node,
		     0,
		     sizeof( libesedb_tree_node_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear node.",
			 function );

			memory_free(
			 *node );

			*node = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees a tree node, its child nodes
 * Uses the value_free_function to free the value
 * Returns 1 if successful or -1 on error
 */
int libesedb_tree_node_free(
     libesedb_tree_node_t **node,
     int (*value_free_function)(
            intptr_t *value,
            liberror_error_t **error ),
     liberror_error_t **error )
{
	libesedb_tree_node_t *tree_node = NULL;
	static char *function           = "libesedb_tree_node_free";
	int number_of_child_nodes       = 0;
	int iterator                    = 0;
	int result                      = 1;

	if( node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( *node != NULL )
	{
		if( ( ( *node )->parent != NULL )
		 || ( ( *node )->previous != NULL )
		 || ( ( *node )->next != NULL ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: invalid node - connected to other nodes.",
			 function );

			return( -1 );
		}
		number_of_child_nodes = ( *node )->number_of_child_nodes;

		for( iterator = 0;
		     iterator < number_of_child_nodes;
		     iterator++ )
		{
			tree_node = ( *node )->first_child;

			if( tree_node->previous != NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
				 "%s: corruption detected.",
				 function );

				return( -1 );
			}
			( *node )->first_child = tree_node->next;

			if( ( *node )->last_child == tree_node )
			{
				( *node )->last_child = tree_node->next;
			}
			( *node )->number_of_child_nodes -= 1;

			if( tree_node->next != NULL )
			{
				tree_node->next->previous = NULL;
			}
			tree_node->parent = NULL;
			tree_node->next   = NULL;

			if( libesedb_tree_node_free(
			     &tree_node,
			     value_free_function,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free child node.",
				 function );

				result = -1;
			}
		}
		if( ( *node )->value != NULL )
		{
			if( value_free_function != NULL )
			{
				if( value_free_function(
				     ( *node )->value,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free value.",
					function );

					result = -1;
				}
				( *node )->value = NULL;
			}
		}
		memory_free(
		 *node );

		*node = NULL;
	}
	return( result );
}

/* Clones the existing tree node and its child nodes
 * This function can return a partially cloned tree node on error
 * Returns 1 if successful or -1 on error
 */
int libesedb_tree_node_clone(
     libesedb_tree_node_t **destination_tree_node,
     libesedb_tree_node_t *source_tree_node,
     int (*value_clone_function)(
            intptr_t **destination,
            intptr_t *source,
            liberror_error_t **error ),
     liberror_error_t **error )
{
	libesedb_tree_node_t *destination_child_node = NULL;
	libesedb_tree_node_t *source_child_node      = NULL;
	static char *function                        = "libesedb_tree_node_clone";
	int iterator                                 = 0;

	if( destination_tree_node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination tree node.",
		 function );

		return( -1 );
	}
	if( *destination_tree_node != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination tree node already set.",
		 function );

		return( -1 );
	}
	if( value_clone_function == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value clone function.",
		 function );

		return( -1 );
	}
	if( source_tree_node == NULL )
	{
		*destination_tree_node = NULL;
	}
	else
	{
		if( libesedb_tree_node_initialize(
		     destination_tree_node,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination tree node.",
			 function );

			return( -1 );
		}
		if( value_clone_function(
		     &( ( *destination_tree_node )->value ),
		     source_tree_node->value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to clone tree node value.",
			 function );

			libesedb_tree_node_free(
			 destination_tree_node,
			 NULL,
			 NULL );

			return( -1 );
		}
		/* Clone the child nodes
		 */
		source_child_node = source_tree_node->first_child;

		for( iterator = 0;
		     iterator < source_tree_node->number_of_child_nodes;
		     iterator++ )
		{
			if( source_child_node == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected in source child node: %d.",
				 function,
				 iterator + 1 );

				return( -1 );
			}
			if( libesedb_tree_node_clone(
			     &destination_child_node,
			     source_child_node,
			     value_clone_function,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to clone child node: %d.",
				 function,
				 iterator + 1 );

				return( -1 );
			}
			if( libesedb_tree_node_append_node(
			     *destination_tree_node,
			     destination_child_node,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append child node: %d.",
				 function,
				 iterator + 1 );

				return( -1 );
			}
			destination_child_node = NULL;
			source_child_node      = source_child_node->next;
		}
	}
	return( 1 );
}

/* Retrieves the value from the tree node
 * Returns 1 if successful or -1 on error
 */
int libesedb_tree_node_get_value(
     libesedb_tree_node_t *node,
     intptr_t **value,
     liberror_error_t **error )
{
	static char *function = "libesedb_tree_node_get_value";

	if( node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	*value = node->value;

	return( 1 );
}

/* Sets the value in the tree node
 * Returns 1 if successful or -1 on error
 */
int libesedb_tree_node_set_value(
     libesedb_tree_node_t *node,
     intptr_t *value,
     liberror_error_t **error )
{
	static char *function = "libesedb_tree_node_set_value";

	if( node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	node->value = value;

	return( 1 );
}

/* Appends a tree node to the parent node
 * Returns 1 if successful or -1 on error
 */
int libesedb_tree_node_append_node(
     libesedb_tree_node_t *parent_node,
     libesedb_tree_node_t *node,
     liberror_error_t **error )
{
	static char *function = "libesedb_tree_node_append_node";

	if( parent_node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid parent node.",
		 function );

		return( -1 );
	}
	if( node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( ( node->parent != NULL )
	 || ( node->previous != NULL )
	 || ( node->next != NULL ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: node already part of another.",
		 function );

		return( -1 );
	}
	node->parent = parent_node;

	if( parent_node->number_of_child_nodes == 0 )
	{
		if( parent_node->first_child != NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: corruption detected - first child already set.",
			 function );

			return( -1 );
		}
		if( parent_node->last_child != NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: corruption detected - last child already set.",
			 function );

			return( -1 );
		}
		parent_node->first_child = node;
		parent_node->last_child  = node;
	}
	else
	{
		if( parent_node->first_child == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing first child.",
			 function );

			return( -1 );
		}
		if( parent_node->last_child == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing last child.",
			 function );

			return( -1 );
		}
		node->previous                = parent_node->last_child;
		parent_node->last_child->next = node;
		parent_node->last_child       = node;
	}
	parent_node->number_of_child_nodes += 1;

	return( 1 );
}

/* Appends a value to the parent node
 * Creates a new tree node
 * Returns 1 if successful or -1 on error
 */
int libesedb_tree_node_append_value(
     libesedb_tree_node_t *parent_node,
     intptr_t *value,
     liberror_error_t **error )
{
	libesedb_tree_node_t *node = NULL;
	static char *function      = "libesedb_tree_node_append_value";

	if( value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	node = (libesedb_tree_node_t *) memory_allocate(
	                                 sizeof( libesedb_tree_node_t ) );

	if( node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create node.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     node,
	     0,
	     sizeof( libesedb_tree_node_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear node.",
		 function );

		memory_free(
		 node );

		return( -1 );
	}
	node->value = value;

	if( libesedb_tree_node_append_node(
	     parent_node,
	     node,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append node.",
		 function );

		memory_free(
		 node );

		return( -1 );
	}
	return( 1 );
}

/* Inserts a tree node in the parent node
 * Uses the value_compare_function to determine the order of the child nodes
 * Returns 1 if successful, 0 if the node already exists or -1 on error
 */
int libesedb_tree_node_insert_node(
     libesedb_tree_node_t *parent_node,
     libesedb_tree_node_t *node,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            liberror_error_t **error ),
     liberror_error_t **error )
{
	libesedb_tree_node_t *child_node = NULL;
	static char *function            = "libesedb_tree_node_insert_node";
	int result                       = 0;
	int iterator                     = 0;

	if( parent_node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid parent node.",
		 function );

		return( -1 );
	}
	if( node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( ( node->parent != NULL )
	 || ( node->previous != NULL )
	 || ( node->next != NULL ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: node already part of another.",
		 function );

		return( -1 );
	}
	if( value_compare_function == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value compare function.",
		 function );

		return( -1 );
	}
	node->parent = parent_node;

	if( parent_node->number_of_child_nodes == 0 )
	{
		if( parent_node->first_child != NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: corruption detected - first child already set.",
			 function );

			return( -1 );
		}
		if( parent_node->last_child != NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: corruption detected - last child already set.",
			 function );

			return( -1 );
		}
		parent_node->first_child = node;
		parent_node->last_child  = node;
	}
	else
	{
		if( parent_node->first_child == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing first child.",
			 function );

			return( -1 );
		}
		if( parent_node->last_child == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing last child.",
			 function );

			return( -1 );
		}
		child_node = parent_node->first_child;

		for( iterator = 0;
		     iterator < parent_node->number_of_child_nodes;
		     iterator++ )
		{
			result = value_compare_function(
			          node->value,
			          child_node->value,
			          error );

			if( result == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to compare child node: %d.",
				 function,
				 iterator + 1 );

				return( -1 );
			}
			else if( result == LIBESEDB_TREE_NODE_COMPARE_EQUAL )
			{
				return( 0 );
			}
			else if( result == LIBESEDB_TREE_NODE_COMPARE_LESS )
			{
				node->previous = child_node->previous;
				node->next     = child_node;

				if( child_node == parent_node->first_child )
				{
					parent_node->first_child = node;
				}
				else if( child_node->previous == NULL )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: corruption detected - missing previous in child node: %d.",
					 function,
					 iterator + 1 );

					return( -1 );
				}
				else
				{
					child_node->previous->next = node;
				}
				child_node->previous = node;

				break;
			}
			child_node = child_node->next;
		}
		if( result >= 1 )
		{
			node->previous                = parent_node->last_child;
			parent_node->last_child->next = node;
			parent_node->last_child       = node;
		}
	}
	parent_node->number_of_child_nodes += 1;

	return( 1 );
}

/* Inserts a tree node in the parent node
 * Creates a new tree node
 * Uses the value_compare_function to determine the order of the child nodes
 * Returns 1 if successful, 0 if the node already exists or -1 on error
 */
int libesedb_tree_node_insert_value(
     libesedb_tree_node_t *parent_node,
     intptr_t *value,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            liberror_error_t **error ),
     liberror_error_t **error )
{
	libesedb_tree_node_t *node = NULL;
	static char *function      = "libesedb_tree_node_insert_value";
	int result                 = 0;

	if( value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	node = (libesedb_tree_node_t *) memory_allocate(
	                                 sizeof( libesedb_tree_node_t ) );

	if( node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create node.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     node,
	     0,
	     sizeof( libesedb_tree_node_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear node.",
		 function );

		memory_free(
		 node );

		return( -1 );
	}
	node->value = value;

	result = libesedb_tree_node_insert_node(
	          parent_node,
	          node,
	          value_compare_function,
	          error );

	if( result != 1 )
	{
		memory_free(
		 node );
	}
	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to insert node.",
		 function );
	}
	return( result );
}

/* Removes a tree node from the parent node
 * Returns 1 if successful or -1 on error
 */
int libesedb_tree_node_remove_node(
     libesedb_tree_node_t *parent_node,
     libesedb_tree_node_t *node,
     liberror_error_t **error )
{
	static char *function = "libesedb_tree_node_remove_node";

	if( parent_node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid parent node.",
		 function );

		return( -1 );
	}
	if( node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( parent_node != node->parent )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: parent mismatch node is no child of parent node.",
		 function );

		return( -1 );
	}
	if( parent_node->number_of_child_nodes == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: corruption detected - invalid number of child nodes.",
		 function );

		return( -1 );
	}
	if( parent_node->first_child == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: corruption detected - missing first child.",
		 function );

		return( -1 );
	}
	if( parent_node->last_child == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: corruption detected - missing last child.",
		 function );

		return( -1 );
	}
	if( parent_node->first_child == node )
	{
		parent_node->first_child = node->next;
	}
	if( parent_node->last_child == node )
	{
		parent_node->last_child = node->previous;
	}
	if( node->next != NULL )
	{
		node->next->previous = node->previous;
	}
	if( node->previous != NULL )
	{
		node->previous->next = node->next;
	}
	node->parent   = NULL;
	node->previous = NULL;
	node->next     = NULL;

	parent_node->number_of_child_nodes -= 1;

	return( 1 );
}

/* Retrieves a list of all the leaf nodes
 * Returns 1 if successful or -1 on error
 */
int libesedb_tree_node_get_leaf_node_list(
     libesedb_tree_node_t *node,
     libesedb_list_t **leaf_node_list,
     liberror_error_t **error )
{
	libesedb_tree_node_t *child_node = NULL;
	static char *function            = "libesedb_tree_node_get_leaf_node_list";
	int iterator                     = 0;

	if( node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( leaf_node_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid leaf node list.",
		 function );

		return( -1 );
	}
	if( *leaf_node_list == NULL )
	{
		*leaf_node_list = (libesedb_list_t *) memory_allocate(
		                                       sizeof( libesedb_list_t ) );

		if( *leaf_node_list == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create leaf node list.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *leaf_node_list,
		     0,
		     sizeof( libesedb_list_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear leaf node list.",
			 function );

			memory_free(
			 *leaf_node_list );

			*leaf_node_list = NULL;

			return( -1 );
		}
	}
	/* Traverse the child nodes
	 */
	if( node->number_of_child_nodes > 0 )
	{
		child_node = node->first_child;

		for( iterator = 0;
		     iterator < node->number_of_child_nodes;
		     iterator++ )
		{
			if( child_node == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected for child node: %d.",
				 function,
				 iterator + 1 );

				return( -1 );
			}
			if( libesedb_tree_node_get_leaf_node_list(
			     child_node,
			     leaf_node_list,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: unable to traverse child node: %d.",
				 function,
				 iterator + 1 );

				return( -1 );
			}
			child_node = child_node->next;
		}
	}
	else if( node->value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid node - missing value.",
		 function );

		return( -1 );
	}
	else if( libesedb_list_append_value(
	          *leaf_node_list,
	          node->value,
	          error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append tree node to leaf node list.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of child nodes in the tree node
 * Returns 1 if successful or -1 on error
 */
int libesedb_tree_node_get_number_of_child_nodes(
     libesedb_tree_node_t *node,
     int *number_of_child_nodes,
     liberror_error_t **error )
{
	static char *function = "libesedb_tree_node_get_number_of_child_nodes";

	if( node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( number_of_child_nodes == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of child nodes.",
		 function );

		return( -1 );
	}
	*number_of_child_nodes = node->number_of_child_nodes;

	return( 1 );
}

/* Retrieves a child node by index
 * Returns 1 if successful or -1 on error
 */
int libesedb_tree_node_get_child_node(
     libesedb_tree_node_t *node,
     int child_node_index,
     libesedb_tree_node_t **child_node,
     liberror_error_t **error )
{
	static char *function = "libesedb_tree_node_get_child_node";
	int iterator          = 0;
	int result            = -1;

	if( node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid node.",
		 function );

		return( -1 );
	}
	if( ( child_node_index < 0 )
	 || ( child_node_index >= node->number_of_child_nodes ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid child node index.",
		 function );

		return( -1 );
	}
	if( child_node == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid child node.",
		 function );

		return( -1 );
	}
	/* Check if the child nodes should be searched front to back
	 * or back to front
	 */
	if( child_node_index < ( node->number_of_child_nodes / 2 ) )
	{
		*child_node = node->first_child;

		for( iterator = 0;
		     iterator < node->number_of_child_nodes;
		     iterator++ )
		{
			if( *child_node == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected for child node: %d.",
				 function,
				 iterator + 1 );

				return( -1 );
			}
			if( iterator == child_node_index )
			{
				result = 1;

				break;
			}	
			*child_node = ( *child_node )->next;
		}
	}
	else
	{
		*child_node = node->last_child;

		for( iterator = ( node->number_of_child_nodes - 1 );
		     iterator >= 0;
		     iterator-- )
		{
			if( *child_node == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected for child node: %d.",
				 function,
				 iterator + 1 );

				return( -1 );
			}
			if( iterator == child_node_index )
			{
				result = 1;

				break;
			}	
			*child_node = ( *child_node )->previous;
		}
	}
	return( result );
}

