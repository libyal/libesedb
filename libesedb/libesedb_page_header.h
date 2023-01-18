/*
 * Page header functions
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

#if !defined( _LIBESEDB_PAGE_HEADER_H )
#define _LIBESEDB_PAGE_HEADER_H

#include <common.h>
#include <types.h>

#include "libesedb_io_handle.h"
#include "libesedb_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_page_header libesedb_page_header_t;

struct libesedb_page_header
{
	/* The data size
	 */
	size_t data_size;

	/* The previous page number
	 */
	uint32_t previous_page_number;

	/* The next page number
	 */
	uint32_t next_page_number;

	/* The father data page (FPD) object identifier
	 */
	uint32_t father_data_page_object_identifier;

	/* The (first) available page tag
	 */
	uint16_t available_page_tag;

	/* The page flags
	 */
	uint32_t flags;

	/* The page number
	 */
	uint32_t page_number;

	/* The ECC-32 checksum
	 */
	uint32_t ecc32_checksum;

	/* The XOR-32 checksum
	 */
	uint32_t xor32_checksum;
};

int libesedb_page_header_initialize(
     libesedb_page_header_t **page_header,
     libcerror_error_t **error );

int libesedb_page_header_free(
     libesedb_page_header_t **page_header,
     libcerror_error_t **error );

int libesedb_page_header_read_data(
     libesedb_page_header_t *page_header,
     libesedb_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libesedb_page_header_get_previous_page_number(
     libesedb_page_header_t *page_header,
     uint32_t *previous_page_number,
     libcerror_error_t **error );

int libesedb_page_header_get_next_page_number(
     libesedb_page_header_t *page_header,
     uint32_t *next_page_number,
     libcerror_error_t **error );

int libesedb_page_header_get_father_data_page_object_identifier(
     libesedb_page_header_t *page_header,
     uint32_t *father_data_page_object_identifier,
     libcerror_error_t **error );

int libesedb_page_header_get_flags(
     libesedb_page_header_t *page_header,
     uint32_t *flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_PAGE_HEADER_H ) */

