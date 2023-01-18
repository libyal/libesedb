/*
 * Data segment functions
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

#if !defined( _LIBESEDB_DATA_SEGMENT_H )
#define _LIBESEDB_DATA_SEGMENT_H

#include <common.h>
#include <types.h>

#include "libesedb_libbfio.h"
#include "libesedb_libcerror.h"
#include "libesedb_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_data_segment libesedb_data_segment_t;

struct libesedb_data_segment
{
	/* The (segment) data
	 */
	uint8_t *data;

	/* The (segment) data size
	 */
	size_t data_size;
};

int libesedb_data_segment_initialize(
     libesedb_data_segment_t **data_segment,
     size_t data_size,
     libcerror_error_t **error );

int libesedb_data_segment_free(
     libesedb_data_segment_t **data_segment,
     libcerror_error_t **error );

int libesedb_data_segment_get_data_size(
     libesedb_data_segment_t *data_segment,
     size_t *data_size,
     libcerror_error_t **error );

int libesedb_data_segment_get_data(
     libesedb_data_segment_t *data_segment,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libesedb_data_segment_read_element_data(
     intptr_t *data_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *element,
     libfdata_cache_t *cache,
     int element_file_index,
     off64_t element_offset,
     size64_t element_size,
     uint32_t element_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_DATA_SEGMENT_H ) */

