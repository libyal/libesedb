/*
 * Space tree functions
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

#if !defined( _LIBESEDB_SPACE_TREE_H )
#define _LIBESEDB_SPACE_TREE_H

#include <common.h>
#include <types.h>

#include "libesedb_io_handle.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcerror.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_page_tree.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_space_tree libesedb_space_tree_t;

struct libesedb_space_tree
{
	/* The page tree
	 */
	libesedb_page_tree_t *page_tree;
};

int libesedb_space_tree_initialize(
     libesedb_space_tree_t **space_tree,
     libesedb_io_handle_t *io_handle,
     uint32_t object_identifier,
     uint32_t root_page_number,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     libcerror_error_t **error );

int libesedb_space_tree_free(
     libesedb_space_tree_t **space_tree,
     libcerror_error_t **error );

int libesedb_space_tree_read_values_from_page(
     libesedb_space_tree_t *space_tree,
     libbfio_handle_t *file_io_handle,
     libesedb_page_t *page,
     int *current_leaf_value_index,
     int recursion_depth,
     libcerror_error_t **error );

int libesedb_space_tree_read_file_io_handle(
     libesedb_space_tree_t *space_tree,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_SPACE_TREE_H ) */

