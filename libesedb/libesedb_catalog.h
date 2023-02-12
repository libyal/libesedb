/*
 * Catalog functions
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

#if !defined( _LIBESEDB_CATALOG_H )
#define _LIBESEDB_CATALOG_H

#include <common.h>
#include <types.h>

#include "libesedb_block_tree.h"
#include "libesedb_io_handle.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcdata.h"
#include "libesedb_libcerror.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_page_tree.h"
#include "libesedb_table_definition.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_catalog libesedb_catalog_t;

struct libesedb_catalog
{
	/* The page tree
	 */
	libesedb_page_tree_t *page_tree;

	/* The page block tree
	 */
	libesedb_block_tree_t *page_block_tree;

	/* The table definition array
	 */
	libcdata_array_t *table_definition_array;
};

int libesedb_catalog_initialize(
     libesedb_catalog_t **catalog,
     libesedb_io_handle_t *io_handle,
     uint32_t root_page_number,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     libcerror_error_t **error );

int libesedb_catalog_free(
     libesedb_catalog_t **catalog,
     libcerror_error_t **error );

int libesedb_catalog_read_value_data(
     libesedb_catalog_t *catalog,
     const uint8_t *data,
     size_t data_size,
     libesedb_table_definition_t **table_definition,
     libcerror_error_t **error );

int libesedb_catalog_read_values_from_leaf_page(
     libesedb_catalog_t *catalog,
     libesedb_page_t *page,
     libesedb_table_definition_t **table_definition,
     libcerror_error_t **error );

int libesedb_catalog_read_file_io_handle(
     libesedb_catalog_t *catalog,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libesedb_catalog_get_number_of_table_definitions(
     libesedb_catalog_t *catalog,
     int *number_of_table_definitions,
     libcerror_error_t **error );

int libesedb_catalog_get_table_definition_by_index(
     libesedb_catalog_t *catalog,
     int table_definition_index,
     libesedb_table_definition_t **table_definition,
     libcerror_error_t **error );

int libesedb_catalog_get_table_definition_by_name(
     libesedb_catalog_t *catalog,
     const uint8_t *table_name,
     size_t table_name_size,
     libesedb_table_definition_t **table_definition,
     libcerror_error_t **error );

int libesedb_catalog_get_table_definition_by_utf8_name(
     libesedb_catalog_t *catalog,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libesedb_table_definition_t **table_definition,
     libcerror_error_t **error );

int libesedb_catalog_get_table_definition_by_utf16_name(
     libesedb_catalog_t *catalog,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libesedb_table_definition_t **table_definition,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_CATALOG_H ) */

