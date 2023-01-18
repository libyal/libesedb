/*
 * Functions for testing
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

#if !defined( _ESEDB_TEST_FUNCTIONS_H )
#define _ESEDB_TEST_FUNCTIONS_H

#include <common.h>
#include <types.h>

#include "esedb_test_libbfio.h"
#include "esedb_test_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

int esedb_test_get_narrow_source(
     const system_character_t *source,
     char *narrow_string,
     size_t narrow_string_size,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

int esedb_test_get_wide_source(
     const system_character_t *source,
     wchar_t *wide_string,
     size_t wide_string_size,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

int esedb_test_open_file_io_handle(
     libbfio_handle_t **file_io_handle,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int esedb_test_close_file_io_handle(
     libbfio_handle_t **file_io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _ESEDB_TEST_FUNCTIONS_H ) */

