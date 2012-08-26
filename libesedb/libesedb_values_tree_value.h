/*
 * Values tree value functions
 *
 * Copyright (c) 2010, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBESEDB_VALUES_TREE_VALUE_H )
#define _LIBESEDB_VALUES_TREE_VALUE_H

#include <common.h>
#include <types.h>

#include "libesedb_array_type.h"
#include "libesedb_io_handle.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcerror.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_table_definition.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum LIBESEDB_VALUES_TREE_VALUE_TYPES
{
	LIBESEDB_VALUES_TREE_VALUE_TYPE_INDEX		= (uint8_t) 'i',
	LIBESEDB_VALUES_TREE_VALUE_TYPE_LONG_VALUE	= (uint8_t) 'l',
	LIBESEDB_VALUES_TREE_VALUE_TYPE_NODE		= (uint8_t) 'n',
	LIBESEDB_VALUES_TREE_VALUE_TYPE_RECORD		= (uint8_t) 'r',
};

typedef struct libesedb_values_tree_value libesedb_values_tree_value_t;

struct libesedb_values_tree_value
{
	/* The type
	 */
	uint8_t type;

	/* The key data
	 */
	uint8_t *key;

	/* The key data size
	 */
	size_t key_size;

	/* The page offset
	 */
	off64_t page_offset;

	/* The page number
	 */
	uint32_t page_number;

	/* The page value index
	 */
	uint16_t page_value_index;

	/* The data offset
	 */
	uint16_t data_offset;

	/* The data size
	 */
	uint16_t data_size;
};

int libesedb_values_tree_value_initialize(
     libesedb_values_tree_value_t **values_tree_value,
     libcerror_error_t **error );

int libesedb_values_tree_value_free(
     libesedb_values_tree_value_t **values_tree_value,
     libcerror_error_t **error );

int libesedb_values_tree_value_set_key_common(
     libesedb_values_tree_value_t *values_tree_value,
     uint8_t *common_key,
     size_t common_key_size,
     libcerror_error_t **error );

int libesedb_values_tree_value_set_key_local(
     libesedb_values_tree_value_t *values_tree_value,
     uint8_t *local_key,
     size_t local_key_size,
     libcerror_error_t **error );

int libesedb_values_tree_value_read_data(
     libesedb_values_tree_value_t *values_tree_value,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     uint8_t **data,
     size_t *data_size,
     libcerror_error_t **error );

int libesedb_values_tree_value_read_record(
     libesedb_values_tree_value_t *values_tree_value,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     libesedb_table_definition_t *table_definition,
     libesedb_table_definition_t *template_table_definition,
     libesedb_array_t *values_array,
     libcerror_error_t **error );

int libesedb_values_tree_value_read_long_value(
     libesedb_values_tree_value_t *values_tree_value,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     libcerror_error_t **error );

int libesedb_values_tree_value_read_long_value_segment(
     libesedb_values_tree_value_t *values_tree_value,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     uint32_t long_value_segment_offset,
     libfdata_block_t *data_block,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

