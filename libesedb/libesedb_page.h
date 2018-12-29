/*
 * Page functions
 *
 * Copyright (C) 2009-2018, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBESEDB_PAGE_H )
#define _LIBESEDB_PAGE_H

#include <common.h>
#include <types.h>

#include "libesedb_io_handle.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcdata.h"
#include "libesedb_libcerror.h"
#include "libesedb_page_header.h"
#include "libesedb_page_value.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_page libesedb_page_t;

struct libesedb_page
{
	/* The page number
	 */
	uint32_t page_number;

	/* The file offset of the page
	 */
	off64_t offset;

	/* The header
	 */
	libesedb_page_header_t *header;

	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	size_t data_size;

	/* The values array
	 */
	libcdata_array_t *values_array;
};

int libesedb_page_initialize(
     libesedb_page_t **page,
     libcerror_error_t **error );

int libesedb_page_free(
     libesedb_page_t **page,
     libcerror_error_t **error );

int libesedb_page_calculate_checksums(
     libesedb_page_t *page,
     libesedb_io_handle_t *io_handle,
     uint32_t *ecc32_checksum,
     uint32_t *xor32_checksum,
     libcerror_error_t **error );

int libesedb_page_read(
     libesedb_page_t *page,
     libesedb_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libesedb_page_read_tags(
     libesedb_page_t *page,
     libesedb_io_handle_t *io_handle,
     uint16_t number_of_page_tags,
     libcdata_array_t **page_tags_array,
     libcerror_error_t **error );

int libesedb_page_read_values(
     libesedb_page_t *page,
     libesedb_io_handle_t *io_handle,
     size_t page_values_data_offset,
     libcerror_error_t **error );

int libesedb_page_get_number_of_values(
     libesedb_page_t *page,
     uint16_t *number_of_values,
     libcerror_error_t **error );

int libesedb_page_get_value(
     libesedb_page_t *page,
     uint16_t value_index,
     libesedb_page_value_t **page_value,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_PAGE_H ) */

