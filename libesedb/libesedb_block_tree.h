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

#if !defined( _LIBESEDB_BLOCK_TREE_H )
#define _LIBESEDB_BLOCK_TREE_H

#include <common.h>
#include <types.h>

#include "libesedb_block_descriptor.h"
#include "libesedb_block_tree_node.h"
#include "libesedb_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_block_tree libesedb_block_tree_t;

struct libesedb_block_tree
{
	/* The block tree root node
	 */
	libesedb_block_tree_node_t *root_node;

	/* The leaf value size
	 */
	size64_t leaf_value_size;
};

int libesedb_block_tree_initialize(
     libesedb_block_tree_t **block_tree,
     size64_t size,
     size64_t leaf_value_size,
     libcerror_error_t **error );

int libesedb_block_tree_free(
     libesedb_block_tree_t **block_tree,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     libcerror_error_t **error );

int libesedb_block_tree_get_block_descriptor_by_offset(
     libesedb_block_tree_t *block_tree,
     off64_t offset,
     libesedb_block_descriptor_t **block_descriptor,
     off64_t *block_offset,
     libcerror_error_t **error );

int libesedb_block_tree_insert_block_descriptor_by_offset(
     libesedb_block_tree_t *block_tree,
     off64_t offset,
     libesedb_block_descriptor_t *block_descriptor,
     int *leaf_value_index,
     libesedb_block_tree_node_t **leaf_block_tree_node,
     libesedb_block_descriptor_t **existing_block_descriptor,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_BLOCK_TREE_H ) */

