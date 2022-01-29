/*
 * Data definition functions
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

#if !defined( _LIBESEDB_DATA_DEFINITION_H )
#define _LIBESEDB_DATA_DEFINITION_H

#include <common.h>
#include <types.h>

#include "libesedb_io_handle.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcdata.h"
#include "libesedb_libcerror.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_table_definition.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_data_definition libesedb_data_definition_t;

struct libesedb_data_definition
{
	/* The page offset
	 */
	off64_t page_offset;

	/* The page number
	 */
	uint32_t page_number;

	/* The page value index
	 */
	uint16_t page_value_index;

	/* The data offset relative to the start of the page
	 */
	uint16_t data_offset;

	/* The data size
	 */
	uint16_t data_size;
};

int libesedb_data_definition_initialize(
     libesedb_data_definition_t **data_definition,
     libcerror_error_t **error );

int libesedb_data_definition_free(
     libesedb_data_definition_t **data_definition,
     libcerror_error_t **error );

int libesedb_data_definition_read_data(
     libesedb_data_definition_t *data_definition,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     uint8_t **data,
     size_t *data_size,
     libcerror_error_t **error );

int libesedb_data_definition_read_record(
     libesedb_data_definition_t *data_definition,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     libesedb_table_definition_t *table_definition,
     libesedb_table_definition_t *template_table_definition,
     libcdata_array_t *values_array,
     uint8_t *record_flags,
     libcerror_error_t **error );

int libesedb_data_definition_read_long_value(
     libesedb_data_definition_t *data_definition,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     libcerror_error_t **error );

int libesedb_data_definition_read_long_value_segment(
     libesedb_data_definition_t *data_definition,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     uint32_t long_value_segment_offset,
     libfdata_list_t *data_segments_list,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_DATA_DEFINITION_H ) */

