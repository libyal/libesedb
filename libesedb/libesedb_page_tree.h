/*
 * Page tree functions
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

#if !defined( _LIBESEDB_PAGE_TREE_H )
#define _LIBESEDB_PAGE_TREE_H

#include <common.h>
#include <types.h>

#include "libesedb_data_definition.h"
#include "libesedb_io_handle.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcdata.h"
#include "libesedb_libcerror.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_page.h"
#include "libesedb_page_tree_key.h"
#include "libesedb_page_tree_value.h"
#include "libesedb_page_value.h"
#include "libesedb_root_page_header.h"
#include "libesedb_table_definition.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_page_tree libesedb_page_tree_t;

struct libesedb_page_tree
{
	/* The IO handle
	 */
	libesedb_io_handle_t *io_handle;

	/* The object identifier
	 */
	uint32_t object_identifier;

	/* The root page number
	 */
	uint32_t root_page_number;

	/* The table definition
	 */
	libesedb_table_definition_t *table_definition;

	/* The template table definition
	 */
	libesedb_table_definition_t *template_table_definition;

	/* The pages vector
	 */
	libfdata_vector_t *pages_vector;

	/* The pages cache
	 */
	libfcache_cache_t *pages_cache;

	/* The root page header
	 */
	libesedb_root_page_header_t *root_page_header;

	/* The leaf page descriptors tree
	 */
	libcdata_btree_t *leaf_page_descriptors_tree;

	/* The number of leaf values
	 */
	int number_of_leaf_values;
};

int libesedb_page_tree_initialize(
     libesedb_page_tree_t **page_tree,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     uint32_t object_identifier,
     uint32_t root_page_number,
     libesedb_table_definition_t *table_definition,
     libesedb_table_definition_t *template_table_definition,
     libcerror_error_t **error );

int libesedb_page_tree_free(
     libesedb_page_tree_t **page_tree,
     libcerror_error_t **error );

int libesedb_page_tree_read_root_page_header(
     libesedb_page_tree_t *page_tree,
     libesedb_page_t *root_page,
     libcerror_error_t **error );

int libesedb_page_tree_read_space_trees(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libesedb_page_tree_get_key(
     libesedb_page_tree_t *page_tree,
     libesedb_page_tree_value_t *page_tree_value,
     libesedb_page_t *page,
     uint32_t page_flags,
     uint16_t page_value_index,
     libesedb_page_tree_key_t **key,
     libcerror_error_t **error );

int libesedb_page_tree_get_first_leaf_page_number_from_page(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     libesedb_page_t *page,
     uint32_t *leaf_page_number,
     int recursion_depth,
     libcerror_error_t **error );

int libesedb_page_tree_get_get_first_leaf_page_number(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     uint32_t *leaf_page_number,
     libcerror_error_t **error );

int libesedb_page_tree_get_number_of_leaf_values_from_leaf_page(
     libesedb_page_tree_t *page_tree,
     libesedb_page_t *page,
     int *number_of_leaf_values,
     libcerror_error_t **error );

int libesedb_page_tree_get_number_of_leaf_values(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     int *number_of_leaf_values,
     libcerror_error_t **error );

int libesedb_page_tree_get_leaf_value_by_index_from_page(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     libesedb_page_t *page,
     int leaf_value_index,
     libesedb_data_definition_t **data_definition,
     int *current_leaf_value_index,
     int recursion_depth,
     libcerror_error_t **error );

int libesedb_page_tree_get_leaf_value_by_index(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     int leaf_value_index,
     libesedb_data_definition_t **data_definition,
     libcerror_error_t **error );

int libesedb_page_tree_get_leaf_value_by_key_from_page(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     libesedb_page_t *page,
     libesedb_page_tree_key_t *leaf_value_key,
     libesedb_data_definition_t **data_definition,
     int recursion_depth,
     libcerror_error_t **error );

int libesedb_page_tree_get_leaf_value_by_key(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     libesedb_page_tree_key_t *leaf_value_key,
     libesedb_data_definition_t **data_definition,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_PAGE_TREE_H ) */

