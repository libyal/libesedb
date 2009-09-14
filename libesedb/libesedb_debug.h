/*
 * Debug functions
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

#if !defined( _LIBESEDB_DEBUG_H )
#define _LIBESEDB_DEBUG_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libesedb_libbfio.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_DEBUG_OUTPUT )

int liblnk_debug_print_page_flags(
     uint32_t page_flags,
     liberror_error_t **error );

int libesedb_debug_print_log_time(
     uint8_t *log_time,
     size_t log_time_size,
     const char *description,
     liberror_error_t **error );

int libesedb_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     liberror_error_t **error );

#endif

#if defined( __cplusplus )
}
#endif

#endif

