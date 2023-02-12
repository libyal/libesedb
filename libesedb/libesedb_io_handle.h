/*
 * Input/Output (IO) handle functions
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

#if !defined( _LIBESEDB_IO_HANDLE_H )
#define _LIBESEDB_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libesedb_libbfio.h"
#include "libesedb_libcerror.h"
#include "libesedb_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const uint8_t esedb_file_signature[ 4 ];

typedef struct libesedb_io_handle libesedb_io_handle_t;

struct libesedb_io_handle
{
	/* The file type
	 */
	uint32_t file_type;

	/* The format version
	 */
	uint32_t format_version;

	/* The format revision number
	 */
	uint32_t format_revision;

	/* The creation format version
	 */
	uint32_t creation_format_version;

	/* The creation format revision number
	 */
	uint32_t creation_format_revision;

	/* The file size
	 */
	size64_t file_size;

	/* The pages data offset
	 */
	off64_t pages_data_offset;

	/* The pages data size
	 */
	size64_t pages_data_size;

	/* The page size
	 */
	uint32_t page_size;

	/* The last page number
	 */
	uint32_t last_page_number;

	/* The codepage of the ASCII strings
	 */
	int ascii_codepage;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int libesedb_io_handle_initialize(
     libesedb_io_handle_t **io_handle,
     libcerror_error_t **error );

int libesedb_io_handle_free(
     libesedb_io_handle_t **io_handle,
     libcerror_error_t **error );

int libesedb_io_handle_clear(
     libesedb_io_handle_t *io_handle,
     libcerror_error_t **error );

int libesedb_io_handle_set_pages_data_range(
     libesedb_io_handle_t *io_handle,
     size64_t file_size,
     libcerror_error_t **error );

int libesedb_io_handle_read_page(
     libesedb_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfdata_cache_t *cache,
     int element_index,
     int element_file_index,
     off64_t element_offset,
     size64_t element_size,
     uint32_t element_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_IO_HANDLE_H ) */

