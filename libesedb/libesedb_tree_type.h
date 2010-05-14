/*
 * Tree type functions
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
 * Copyright (c) 2008-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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

#if !defined( _LIBESEDB_TREE_TYPE_H )
#define _LIBESEDB_TREE_TYPE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libesedb_list_type.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum LIBESEDB_TREE_NODE_COMPARE_DEFINITIONS
{
	LIBESEDB_TREE_NODE_COMPARE_LESS,
	LIBESEDB_TREE_NODE_COMPARE_EQUAL,
	LIBESEDB_TREE_NODE_COMPARE_GREATER
};

typedef struct libesedb_tree_node libesedb_tree_node_t;

struct libesedb_tree_node
{
	/* The parent node
	 */
	libesedb_tree_node_t *parent;

	/* The previous node list
	 */
	libesedb_tree_node_t *previous;

	/* The next node list
	 */
	libesedb_tree_node_t *next;

	/* The first child node
	 */
	libesedb_tree_node_t *first_child;

	/* The last child node
	 */
	libesedb_tree_node_t *last_child;

	/* The number of child nodes
	 */
	int number_of_child_nodes;

	/* The node value
	 */
	intptr_t *value;
};

int libesedb_tree_node_initialize(
     libesedb_tree_node_t **node,
     liberror_error_t **error );

int libesedb_tree_node_free(
     libesedb_tree_node_t **node,
     int (*value_free_function)(
            intptr_t *value,
            liberror_error_t **error ),
     liberror_error_t **error );

int libesedb_tree_node_clone(
     libesedb_tree_node_t **destination_tree_node,
     libesedb_tree_node_t *source_tree_node,
     int (*value_clone_function)(
            intptr_t **destination,
            intptr_t *source,
            liberror_error_t **error ),
     liberror_error_t **error );

int libesedb_tree_node_get_value(
     libesedb_tree_node_t *node,
     intptr_t **value,
     liberror_error_t **error );

int libesedb_tree_node_set_value(
     libesedb_tree_node_t *node,
     intptr_t *value,
     liberror_error_t **error );

int libesedb_tree_node_append_node(
     libesedb_tree_node_t *parent_node,
     libesedb_tree_node_t *node,
     liberror_error_t **error );

int libesedb_tree_node_append_value(
     libesedb_tree_node_t *parent_node,
     intptr_t *value,
     liberror_error_t **error );

int libesedb_tree_node_insert_node(
     libesedb_tree_node_t *parent_node,
     libesedb_tree_node_t *node,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            liberror_error_t **error ),
     liberror_error_t **error );

int libesedb_tree_node_insert_value(
     libesedb_tree_node_t *parent_node,
     intptr_t *value,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            liberror_error_t **error ),
     liberror_error_t **error );

int libesedb_tree_node_remove_node(
     libesedb_tree_node_t *parent_node,
     libesedb_tree_node_t *node,
     liberror_error_t **error );

int libesedb_tree_node_get_leaf_node_list(
     libesedb_tree_node_t *node,
     libesedb_list_t **leaf_node_list,
     liberror_error_t **error );

int libesedb_tree_node_get_number_of_child_nodes(
     libesedb_tree_node_t *node,
     int *number_of_child_nodes,
     liberror_error_t **error );

int libesedb_tree_node_get_child_node(
     libesedb_tree_node_t *node,
     int child_index,
     libesedb_tree_node_t **child_node,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

