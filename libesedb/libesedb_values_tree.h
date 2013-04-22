/*
 * Values tree functions
 *
 * Copyright (c) 2010-2013, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBESEDB_VALUES_TREE_H )
#define _LIBESEDB_PAGE_VALUES_H

#include <common.h>
#include <types.h>

#include "libesedb_libbfio.h"
#include "libesedb_libcerror.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_values_tree_value.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libesedb_values_tree_get_leaf_node_by_key(
     libfdata_tree_t *values_tree,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *values_cache,
     uint8_t *key,
     size_t key_size,
     libfdata_tree_node_t **values_tree_leaf_node,
     uint8_t flags,
     libcerror_error_t **error );

int libesedb_values_tree_node_get_leaf_node_by_key(
     libfdata_tree_node_t *values_tree_node,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *values_cache,
     uint8_t *key,
     size_t key_size,
     libfdata_tree_node_t **values_tree_leaf_node,
     uint8_t flags,
     libcerror_error_t **error );

int libesedb_values_tree_get_value_by_key(
     libfdata_tree_t *values_tree,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *values_cache,
     uint8_t *key,
     size_t key_size,
     libesedb_values_tree_value_t **values_tree_value,
     uint8_t flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

