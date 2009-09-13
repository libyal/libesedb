/*
 * libesedb Input/Output (IO) handle
 *
 * Copyright (c) 2008-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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

#if !defined( _LIBESEDB_IO_HANDLE_H )
#define _LIBESEDB_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libesedb_libbfio.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const uint8_t esedb_file_signature[ 4 ];

typedef struct libesedb_io_handle libesedb_io_handle_t;

struct libesedb_io_handle
{
	/* The file io handle
	 */
	libbfio_handle_t *file_io_handle;

	/* Value to indicate if the file io handle was created inside the library
	 */
	uint8_t handle_created_in_library;
};

int libesedb_io_handle_initialize(
     libesedb_io_handle_t **io_handle,
     liberror_error_t **error );

int libesedb_io_handle_free(
     libesedb_io_handle_t **io_handle,
     liberror_error_t **error );

int libesedb_io_handle_open(
     libesedb_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     int flags,
     liberror_error_t **error );

int libesedb_io_handle_close(
     libesedb_io_handle_t *io_handle,
     liberror_error_t **error );

int libesedb_io_handle_read_file_header(
     libesedb_io_handle_t *io_handle,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

